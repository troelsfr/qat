#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/qat/Commandline/ConfigurationManager.hpp"
#include "qir/qat/QatTypes/QatTypes.hpp"

namespace microsoft::quantum
{

class GroupingPassConfiguration // TODO(unknown): Rename GroupingPass -> CircuitSeparation
{
  public:
    using Set              = std::unordered_set<std::string>;
    using DeferredValuePtr = DeferredValue::DeferredValuePtr;

    // Setup and construction
    //

    /// Setup function that adds the configuration flags to the ConfigurationManager. See the
    /// ConfigurationManager documentation for more details on how the setup process is implemented.
    void setup(ConfigurationManager& config)
    {
        config.setSectionName("Circuit separation", "Separation of quantum and classical circuits");
        config.addExperimentalParameter(
            circuit_separation_, true, false, "separate-circuits",
            "Whether or not to separate quantum and classical circuits");

        irreversible_operations_ = config.getParameter("irreversible-operations");
        qir_runtime_prefix_      = config.getParameter("qir-runtime-prefix");
    }

    static GroupingPassConfiguration createDisabled()
    {
        GroupingPassConfiguration ret;
        ret.circuit_separation_ = false;

        return ret;
    }

    bool circuitSeparation() const
    {
        return circuit_separation_;
    }

    Set irreversibleOperations() const
    {
        assert(irreversible_operations_ != nullptr);
        if (irreversible_operations_ == nullptr)
        {
            throw std::runtime_error("Configuration 'GroupingPassConfiguration' was not initialized.");
        }

        if (!irreversible_operations_->isDereferenceable())
        {
            throw std::runtime_error("Target QIS configuration was not loaded.");
        }

        return irreversible_operations_->value<Set>();
    }

    String qirRuntimePrefix() const
    {
        return qir_runtime_prefix_->value<String>();
    }

  private:
    bool             circuit_separation_{true};
    DeferredValuePtr irreversible_operations_{};
    DeferredValuePtr qir_runtime_prefix_{};
};

} // namespace microsoft::quantum
