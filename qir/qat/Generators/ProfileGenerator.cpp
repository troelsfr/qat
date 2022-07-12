// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "Generators/LlvmPassesConfiguration.hpp"
#include "Generators/PostTransformConfig.hpp"
#include "Generators/ProfileGenerator.hpp"
#include "GroupingPass/GroupingAnalysisPass.hpp"
#include "GroupingPass/GroupingPass.hpp"
#include "GroupingPass/GroupingPassConfiguration.hpp"
#include "PostTransformValidation/PostTransformValidationPass.hpp"
#include "PreTransformTrimming/PreTransformTrimmingPass.hpp"
#include "RecordPullBackPass/RecordPullBackPass.hpp"
#include "Rules/Factory.hpp"
#include "Rules/RuleSet.hpp"
#include "StaticResourceComponent/AllocationAnalysisPass.hpp"
#include "StaticResourceComponent/QubitRemapPass.hpp"
#include "StaticResourceComponent/ReplaceQubitOnResetPass.hpp"
#include "StaticResourceComponent/ResourceAnnotationPass.hpp"
#include "StaticResourceComponent/StaticResourceComponentConfiguration.hpp"
#include "TransformationRulesPass/TransformationRulesPass.hpp"
#include "TransformationRulesPass/TransformationRulesPassConfiguration.hpp"
#include "Utils/FunctionToModule.hpp"
#include "ValidationPass/ValidationPassConfiguration.hpp"
#include "ZExtTransformPass/ZExtTransformPass.hpp"

#include "Llvm/Llvm.hpp"

namespace microsoft::quantum
{
Profile ProfileGenerator::newProfile(String const& name, OptimizationLevel const& optimization_level, bool debug)
{
    auto qubit_allocation_manager  = BasicAllocationManager::createNew();
    auto result_allocation_manager = BasicAllocationManager::createNew();

    auto cfg = configuration_manager_.get<TransformationRulesPassConfiguration>();
    qubit_allocation_manager->setReuseRegisters(cfg.shouldReuseQubits());
    result_allocation_manager->setReuseRegisters(cfg.shouldReuseResults());

    // Creating profile
    // TODO(issue-12): Set target machine
    Profile ret{name, logger_, debug, nullptr, qubit_allocation_manager, result_allocation_manager};

    auto module_pass_manager = createGenerationModulePassManager(ret, optimization_level, debug);

    for (auto& c : components_)
    {
        llvm::FunctionPassManager function_pass_manager;
        function_pass_manager_ = &function_pass_manager;
        if (debug)
        {
            llvm::outs() << "Setting " << c.first << " up\n";
        }

        c.second(this, ret);
        module_pass_manager.addPass(FunctionToModule(std::move(function_pass_manager)));
    }

    ret.setModulePassManager(std::move(module_pass_manager));

    // Creating validator
    auto validator =
        std::make_unique<Validator>(configuration_manager_.get<ValidationPassConfiguration>(), logger_, debug);

    ret.setValidator(std::move(validator));

    return ret;
}

llvm::ModulePassManager ProfileGenerator::createGenerationModulePassManager(
    Profile&                 profile,
    OptimizationLevel const& optimization_level,
    bool                     debug)
{
    auto&                   pass_builder = profile.passBuilder();
    llvm::ModulePassManager ret{};

    module_pass_manager_ = &ret;
    pass_builder_        = &pass_builder;
    optimization_level_  = optimization_level;
    debug_               = debug;

    return ret;
}

llvm::ModulePassManager ProfileGenerator::createValidationModulePass(PassBuilder&, OptimizationLevel const&, bool)
{
    throw std::runtime_error("Validation is not supported yet.");
}

llvm::ModulePassManager& ProfileGenerator::modulePassManager()
{
    assert(module_pass_manager_ != nullptr);
    return *module_pass_manager_;
}

llvm::FunctionPassManager& ProfileGenerator::functionPassManager()
{
    assert(function_pass_manager_ != nullptr);
    return *function_pass_manager_;
}

llvm::PassBuilder& ProfileGenerator::passBuilder()
{
    return *pass_builder_;
}

ConfigurationManager& ProfileGenerator::configurationManager()
{
    return configuration_manager_;
}

ConfigurationManager const& ProfileGenerator::configurationManager() const
{
    return configuration_manager_;
}

ProfileGenerator::OptimizationLevel ProfileGenerator::optimizationLevel() const
{
    return optimization_level_;
}

bool ProfileGenerator::isDebugMode() const
{
    return debug_;
}

void ProfileGenerator::replicateProfileComponent(String const& id)
{
    for (auto& c : components_)
    {
        if (c.first == id)
        {
            auto setup_wrapper = c.second;
            components_.push_back({"__unnamed__", std::move(setup_wrapper)});
            return;
        }
    }

    throw std::runtime_error("Component " + id + " not found.");
}

void ProfileGenerator::setupDefaultComponentPipeline()
{
    using namespace llvm;
    ILoggerPtr logger = logger_;

    registerProfileComponent<LlvmPassesConfiguration>(
        "llvm-optimization",
        [](LlvmPassesConfiguration const& cfg, ProfileGenerator* ptr, Profile& /*profile*/)
        {
            auto& mpm = ptr->modulePassManager();

            // Eliminating intrinsic functions
            if (cfg.eliminateConstants())
            {
                llvm::FunctionPassManager early_fpm;
                mpm.addPass(llvm::ForceFunctionAttrsPass());
                mpm.addPass(llvm::InferFunctionAttrsPass());

                early_fpm.addPass(llvm::SimplifyCFGPass());
                early_fpm.addPass(llvm::EarlyCSEPass(false));
                mpm.addPass(FunctionToModule(std::move(early_fpm)));
            }

            auto& fpm = ptr->functionPassManager();

            // Always inline
            if (cfg.alwaysInline())
            {

                auto& pass_builder = ptr->passBuilder();
                mpm.addPass(llvm::AlwaysInlinerPass());
                auto                           inline_param = getInlineParams(cfg.inlineParameter());
                llvm::ModuleInlinerWrapperPass inliner_pass = ModuleInlinerWrapperPass(inline_param);
                mpm.addPass(std::move(inliner_pass));
            }

            // Unroll loop
            if (cfg.unrollLoops())
            {
                auto& pass_builder = ptr->passBuilder();

                /// More unroll parameters
                /// https://llvm.org/doxygen/LoopUnrollPass_8cpp.html

                /// Header
                /// https://llvm.org/doxygen/LoopUnrollPass_8h.html

                llvm::LoopUnrollOptions loop_config(
                    cfg.unrollOptLevel(), cfg.unrollOnlyWhenForced(), cfg.unrollForgeScev());

                loop_config.setPartial(cfg.unrollAllowPartial())
                    .setPeeling(cfg.unrollAllowPeeling())
                    .setRuntime(cfg.unrollAllowRuntime())
                    .setUpperBound(cfg.unrollAllowUpperBound())
                    .setProfileBasedPeeling(cfg.unrollAllowProfilBasedPeeling())
                    .setFullUnrollMaxCount(cfg.unrolFullUnrollCount());

                fpm.addPass(llvm::LoopUnrollPass(loop_config));
            }

            if (cfg.eliminateMemory())
            {
                fpm.addPass(llvm::PromotePass());
            }

            if (cfg.eliminateConstants())
            {
                fpm.addPass(llvm::SCCPPass());
            }

            if (cfg.eliminateDeadCode())
            {
                fpm.addPass(llvm::ADCEPass());
            }
        });

    registerProfileComponent<PreTransformTrimmingPassConfiguration>(
        "pre-transform-trimming",
        [logger](PreTransformTrimmingPassConfiguration const& cfg, ProfileGenerator* ptr, Profile& /*profile*/)
        {
            auto& mpm = ptr->modulePassManager();

            mpm.addPass(PreTransformTrimmingPass(cfg, logger));
        });

    registerProfileComponent<TransformationRulesPassConfiguration>(
        "transformation-rules",
        [logger](TransformationRulesPassConfiguration const& cfg, ProfileGenerator* ptr, Profile& profile)
        {
            auto& ret = ptr->modulePassManager();

            // Defining the mapping
            RuleSet rule_set;
            auto    factory = RuleFactory(
                   rule_set, profile.getQubitAllocationManager(), profile.getResultAllocationManager(), logger);
            factory.usingConfiguration(ptr->configurationManager().get<FactoryConfiguration>());

            // Creating profile pass
            auto pass = TransformationRulesPass(std::move(rule_set), cfg, &profile);
            pass.setLogger(logger);
            ret.addPass(std::move(pass));
        });

    registerProfileComponent<PostTransformConfig>(
        "post-transform",
        [logger](PostTransformConfig const& cfg, ProfileGenerator* ptr, Profile& /*profile*/)
        {
            auto& mpm = ptr->modulePassManager();
            auto& fpm = ptr->functionPassManager();

            if (cfg.shouldAddInstCombinePass())
            {
                fpm.addPass(llvm::InstCombinePass(1000));
            }

            if (cfg.shouldAddAggressiveInstCombinePass())
            {
                fpm.addPass(llvm::AggressiveInstCombinePass());
            }

            if (cfg.shouldAddSccpPass())
            {
                fpm.addPass(llvm::SCCPPass());
            }

            if (cfg.shouldAddSimplifyCfgPass())
            {
                fpm.addPass(llvm::SimplifyCFGPass());
            }

            if (cfg.shouldLowerSwitch())
            {
                fpm.addPass(llvm::LowerSwitchPass());
            }

            if (cfg.shouldEliminateZExtI1())
            {
                fpm.addPass(ZExtTransformPass());
            }

            if (cfg.shouldPullRecordsBack())
            {
                fpm.addPass(RecordPullBackPass());
            }
        });

    registerProfileComponent<PostTransformValidationPassConfiguration>(
        "post-transform-validation",
        [logger](
            PostTransformValidationPassConfiguration const& cfg, ProfileGenerator* ptr, Profile&
            /*profile*/)
        {
            auto& mpm = ptr->modulePassManager();
            mpm.addPass(PostTransformValidationPass(cfg, logger));
        });

    registerProfileComponent<StaticResourceComponentConfiguration>(
        "static-resource",
        [logger](StaticResourceComponentConfiguration const& cfg, ProfileGenerator* ptr, Profile& profile)
        {
            auto& fam = profile.functionAnalysisManager();
            fam.registerPass([&] { return AllocationAnalysisPass(logger); });

            auto& fpm = ptr->functionPassManager();

            fpm.addPass(ReplaceQubitOnResetPass(cfg, logger));
            fpm.addPass(QubitRemapPass(cfg, logger));

            if (cfg.shouldInlineAfterIdChange() && cfg.isChangingIds())
            {
                fpm.addPass(llvm::InstCombinePass(1000));
                fpm.addPass(llvm::AggressiveInstCombinePass());
                fpm.addPass(llvm::SCCPPass());
                fpm.addPass(llvm::SimplifyCFGPass());
                fpm.addPass(llvm::LowerSwitchPass());
                fpm.addPass(ZExtTransformPass());
            }

            fpm.addPass(ResourceAnnotationPass(cfg, logger));
        });

    registerProfileComponent<GroupingPassConfiguration>(
        "grouping",
        [logger](GroupingPassConfiguration const& cfg, ProfileGenerator* ptr, Profile& profile)
        {
            if (cfg.circuitSeparation())
            {
                auto& mam = profile.moduleAnalysisManager();
                mam.registerPass([&] { return GroupingAnalysisPass(cfg, logger); });
                auto& ret = ptr->modulePassManager();

                auto pass = GroupingPass(cfg);
                pass.setLogger(logger);
                ret.addPass(std::move(pass));
            }
        });
}

void ProfileGenerator::setLogger(ILoggerPtr const& logger)
{
    logger_ = logger;
}

} // namespace microsoft::quantum
