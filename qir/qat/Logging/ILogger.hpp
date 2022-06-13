#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "Logging/SourceLocation.hpp"
#include "QatTypes/QatTypes.hpp"

#include "Llvm/Llvm.hpp"

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

namespace microsoft
{
namespace quantum
{

    /// Logger interface to allow the collection of different types of messages during QIR
    /// transformation and/or validation.
    class ILogger
    {
      public:
        using Value            = llvm::Value;
        using LocationResolver = std::function<SourceLocation(Value const*)>;
        using ILoggerPtr       = std::shared_ptr<ILogger>;

        /// Class that holds the location of where the incident happened.
        struct Location : public SourceLocation
        {
            Location() = default;
            Location(SourceLocation const& source)
              : SourceLocation(source)
            {
            }

            Location(
                String  v_name,
                int64_t v_line,
                int64_t v_column,
                String  v_llvm_hint     = "",
                String  v_frontend_hint = "")
              : SourceLocation(v_name, v_line, v_column)
              , llvm_hint{v_llvm_hint}
              , frontend_hint{v_frontend_hint}
            {
            }

            Location(Location const& source)
              : SourceLocation(source)
              , llvm_hint{source.llvm_hint}
              , frontend_hint{source.frontend_hint}
            {
            }

            String llvm_hint{""};
            String frontend_hint{""};
        };

        /// Enum description what type of information we are conveying.
        enum class Type
        {
            Debug,
            Info,
            Warning,
            Error,
            InternalError,
        };

        /// Struct to hold a message together with its type and location
        struct Message
        {
            Type     type;
            Location location;
            String   message;
        };

        /// List of messages defined as alias.
        using Messages = std::vector<Message>;

        // Constructors, copy and move operators and destructors
        //

        ILogger()               = default;
        ILogger(ILogger const&) = default;
        ILogger(ILogger&&)      = default;
        ILogger& operator=(ILogger const&) = default;
        ILogger& operator=(ILogger&&) = default;

        virtual ~ILogger();

        // Abstract interface methods
        //

        /// Reports a debug message.
        virtual void debug(String const& message) = 0;

        /// Reports an info message.
        virtual void info(String const& message) = 0;

        /// Reports a warning message.
        virtual void warning(String const& message) = 0;

        /// Reports an error message.
        virtual void error(String const& message) = 0;

        /// Reports an internal error message.
        virtual void internalError(String const& message) = 0;

        /// Sets the current location. Importantly, the location can be set independently of the reported
        /// messages. This allows one to update the location upon updating the cursor position without
        /// having to worry about keeping a copy of the location to pass when reporting messages.
        /// The most obvious case of this is file path (name) with a line and character (line, col).
        virtual void setLocation(SourceLocation const& location) = 0;

        /// Sets the value of the LLVM instruction causing the issue.
        virtual void setLlvmHint(String const& value) = 0;

        /// Sets the value of the frontend instruction causing the issue.
        virtual void setFrontendHint(String const& value) = 0;

        virtual Messages const& messages() const;

        virtual void dump(std::ostream& out) const;

        // Location integration with LLVM
        //

        /// Sets the logger position based on a LLVM value.
        void setLocationFromValue(llvm::Value const* value);

        /// Sets a resolver which that translates a LLVM value into a position in the source
        void setLocationResolver(LocationResolver const& r);

        /// Returns a source location from the value pointer (if possible)
        SourceLocation resolveLocation(llvm::Value const* value);

        /// Whether or not errors were logged.
        bool hadErrors() const;

        /// Whether or not warnings were logged.
        bool hadWarnings() const;

        /// Standard messages
        virtual void errorCouldNotDeleteNode(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("Could not delete node.");
        }

        virtual void errorExpectedStraightLineCodeMultipleFunctions(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("Expected straight line code, but multiple functions present.");
        }

        virtual void errorExpectedStraightLineCodeMultipleBlocks(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("Expected straight line code, but multiple blocks present.");
        }

        virtual void errorReleaseFailNonStandardAlloc(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("Cannot release qubit from non-standard allocation.");
        }

        virtual void warningReleasePhiNodeQubit(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            warning("Cannot release qubit arising from phi node.");
        }

        virtual void errorFunctionInliningMaxRecursion(uint64_t n, llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("Function inlining exceeded maximum recursion depth of " + std::to_string(n));
        }

        virtual void errorNoQubitsPresent(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("No qubits present in function (required by profile).");
        }

        virtual void errorNoResultsPresent(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("No results present in function (required by profile).");
        }

        virtual void errorOpcodeNotAllowed(String const& code, String const& profile_name, llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }

            error("Opcode '" + code + "' is not allowed for this profile (" + profile_name + ").");
        }

        virtual void errorCustomFunctionsNotAllowed(llvm::Value* ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("Calls to custom defined functions not allowed.");
        }

        virtual void errorExternalCallsNotAllowed(
            String const& function_name,
            String const& profile_name,
            llvm::Value*  ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("External call '" + function_name + "' is not allowed for this profile (" + profile_name + ").");
        }

        virtual void errorTypeNotAllowed(
            String const& type_name,
            String const& profile_name,
            llvm::Value*  ptr = nullptr)
        {
            if (ptr)
            {
                setLocationFromValue(ptr);
            }
            error("Type '" + type_name + "' is not allowed for this profile (" + profile_name + ").");
        }

      protected:
        bool had_errors_{false};   ///< Variable to indicate whether or not errors were reported.
        bool had_warnings_{false}; ///< Variable to indicate whether or not warnings were reported.

      private:
        LocationResolver location_resolver_{nullptr};
    };

} // namespace quantum
} // namespace microsoft
