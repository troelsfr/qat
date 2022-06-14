// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "Logging/ILogger.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

namespace microsoft
{
namespace quantum
{
    ILogger::~ILogger()
    {
        // Ensuring that the captured function is deleted to release
        // shared memory.
        location_resolver_ = nullptr;
    }

    void ILogger::setLocationFromValue(llvm::Value const* value)
    {
        if (location_resolver_)
        {
            auto loc = location_resolver_(value);
            setLocation(loc);
        }
    }

    void ILogger::setLocationResolver(LocationResolver const& r)
    {
        location_resolver_ = r;
    }

    ILogger::Messages const& ILogger::messages() const
    {
        throw std::runtime_error("messages() is not supported by logger.");
    }

    void ILogger::dump(std::ostream& /*out*/) const
    {
        throw std::runtime_error("dump() is not supported by logger.");
    }

    SourceLocation ILogger::resolveLocation(llvm::Value const* value)
    {
        if (location_resolver_)
        {
            return location_resolver_(value);
        }
        return SourceLocation::InvalidPosition();
    }

    bool ILogger::hadErrors() const
    {
        return had_errors_;
    }

    bool ILogger::hadWarnings() const
    {
        return had_warnings_;
    }

    void ILogger::errorWithLocation(String const& message, llvm::Value* ptr)
    {
        errorWithLocation(message, ptr);
    }

    void ILogger::errorCouldNotDeleteNode(llvm::Value* ptr)
    {
        errorWithLocation("Could not delete node.", ptr);
    }

    void ILogger::errorExpectedStraightLineCodeMultipleFunctions(llvm::Value* ptr)
    {
        errorWithLocation("Expected straight line code, but multiple functions present.", ptr);
    }

    void ILogger::errorExpectedStraightLineCodeMultipleBlocks(llvm::Value* ptr)
    {
        errorWithLocation("Expected straight line code, but multiple blocks present.", ptr);
    }

    void ILogger::errorReleaseFailNonStandardAlloc(llvm::Value* ptr)
    {
        errorWithLocation("Cannot release qubit from non-standard allocation.", ptr);
    }

    void ILogger::warningReleasePhiNodeQubit(llvm::Value* ptr)
    {
        if (ptr)
        {
            setLocationFromValue(ptr);
        }
        warning("Cannot release qubit arising from phi node.");
    }

    void ILogger::errorFunctionInliningMaxRecursion(uint64_t n, llvm::Value* ptr)
    {
        errorWithLocation("Function inlining exceeded maximum recursion depth of " + std::to_string(n), ptr);
    }

    void ILogger::errorNoQubitsPresent(llvm::Value* ptr)
    {
        errorWithLocation("No qubits present in function (required by profile).", ptr);
    }

    void ILogger::errorNoResultsPresent(llvm::Value* ptr)
    {
        errorWithLocation("No results present in function (required by profile).", ptr);
    }

    void ILogger::errorOpcodeNotAllowed(String const& code, String const& profile_name, llvm::Value* ptr)
    {
        if (ptr)
        {
            setLocationFromValue(ptr);
        }

        error("Opcode '" + code + "' is not allowed for this profile (" + profile_name + ").");
    }

    void ILogger::errorCustomFunctionsNotAllowed(llvm::Value* ptr)
    {
        errorWithLocation("Calls to custom defined functions not allowed.", ptr);
    }

    void ILogger::errorExternalCallsNotAllowed(
        String const& function_name,
        String const& profile_name,
        llvm::Value*  ptr)
    {
        errorWithLocation(
            "External call '" + function_name + "' is not allowed for this profile (" + profile_name + ").", ptr);
    }

    void ILogger::errorTypeNotAllowed(String const& type_name, String const& profile_name, llvm::Value* ptr)
    {
        errorWithLocation("Type '" + type_name + "' is not allowed for this profile (" + profile_name + ").", ptr);
    }
} // namespace quantum
} // namespace microsoft
