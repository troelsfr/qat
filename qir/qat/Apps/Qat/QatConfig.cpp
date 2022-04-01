// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "Apps/Qat/QatConfig.hpp"
#include "Commandline/ConfigurationManager.hpp"

namespace microsoft
{
namespace quantum
{

    void QatConfig::setup(ConfigurationManager& config)
    {

        config.setSectionName(
            "Base configuration", "Configuration of the quantum adoption tool to execute a specific behaviour.");
        config.addExperimentalParameter(load_, static_cast<String>(""), "load", "Load component.");
        config.addParameter(
            generate_, "apply", "Applies a profile to transform the IR in correspondence with the profile.");
        config.addParameter(validate_, false, "validate", "Executes the validation procedure.");
        config.addParameter(profile_, static_cast<String>("generic"), "profile", "Sets the profile.");
        config.addParameter(emit_llvm_, false, "S", "Emits LLVM IR to the standard output.");
        config.addParameter(opt0_, false, "O0", "Optimization level 0.");
        config.addParameter(opt1_, false, "O1", "Optimization level 1.");
        config.addParameter(opt2_, false, "O2", "Optimization level 2.");
        config.addParameter(opt3_, false, "O3", "Optimization level 3.");

        config.addParameter(verify_module_, "verify-module", "Verifies the module after transformation.");

        config.addParameter(experimental_, "experimental", "Enables experimental features.");
        config.addParameter(dump_config_, "dump-config", "Prints the configuration to the standard output.");
        config.addParameter(add_ir_debug_info_, "add-ir-debug", "Annotates the IR with debug information.");
        config.addParameter(strip_existing_debug_, "strip-existing-dbg", "Strips existing debug symbols.");

        config.addParameter(
            save_report_to_, "save-logs", "Saves the logs report to specified filename in JSON format.");
    }

    bool QatConfig::shouldGenerate() const
    {
        return generate_;
    }

    bool QatConfig::shouldValidate() const
    {
        return validate_;
    }

    String QatConfig::profile() const
    {
        return profile_;
    }

    bool QatConfig::shouldEmitLlvm() const
    {
        return emit_llvm_;
    }

    bool QatConfig::isOpt0Enabled() const
    {
        return opt0_;
    }

    bool QatConfig::isOpt1Enabled() const
    {
        return opt1_;
    }

    bool QatConfig::isOpt2Enabled() const
    {
        return opt2_;
    }

    bool QatConfig::isOpt3Enabled() const
    {
        return opt3_;
    }

    bool QatConfig::verifyModule() const
    {
        return verify_module_;
    }

    bool QatConfig::isDebugMode() const
    {
        return debug_;
    }

    bool QatConfig::shouldDumpConfig() const
    {
        return dump_config_;
    }

    String QatConfig::load() const
    {
        return load_;
    }

    bool QatConfig::isExperimental() const
    {
        return experimental_;
    }

    bool QatConfig::addIrDebugInfo() const
    {
        return add_ir_debug_info_;
    }

    bool QatConfig::stripExistingDebugInfo() const
    {
        return strip_existing_debug_;
    }

    String const& QatConfig::saveReportTo() const
    {
        return save_report_to_;
    }
} // namespace quantum
} // namespace microsoft
