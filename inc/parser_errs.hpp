#pragma once

#include <string>
#include <array>
#include <stdexcept>

enum class ParseErrorType {
    ExpectedLet,
    ExpectedColon,
    ExpectedSemiColon,
    ExpectedDataType,
    ExpectedIdentifier,
    ExpectedEqual,
    COUNT // handy to keep track of number of errors
};

inline const std::array<const char*, static_cast<size_t>(ParseErrorType::COUNT)>
ParseErrorStrings {{
    "Expected 'let'",
    "Expected ':'",
    "Expected ';'",
    "Expected [data type]",
    "Expected [identifier]",
    "Expected '='"
}};

inline std::string to_string(ParseErrorType type) {
    auto idx = static_cast<size_t>(type);
    if (idx >= ParseErrorStrings.size()) {
        throw std::out_of_range("Invalid ParseErrorType");
    }
    return ParseErrorStrings[idx];
}