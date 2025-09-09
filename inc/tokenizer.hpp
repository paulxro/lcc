#pragma once

#include <fstream>
#include <vector>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <cctype>
#include <algorithm>
#include <cassert>

#include "token_type.hpp"

struct Token {
    std::string value;
    TokenType type;

    explicit operator bool() const {
        return !value.empty();
    }
};

class Tokenizer {
public:
    explicit Tokenizer(std::string src) 
        : m_src(std::move(src)) {}

    /* Batch tokenizer: consumes all tokens. */
    std::vector<Token> tokenize();

    /* Streamed tokenizer: pull one token at a time. */
    std::optional<Token> next_token();
    std::optional<Token> peek_token();

    /* Reset to beginning of source. */
    void reset();

private:
    // keyword and symbol tables
    static std::unordered_map<std::string, TokenType> m_keywords;
    static std::unordered_map<std::string, TokenType> m_keybreak;

    std::string m_src;
    uint32_t m_pos = 0;               // char position in source
    std::optional<Token> m_peeked;    // cache for peeked token

    /* Character-level utilities */
    inline std::optional<char> peek(uint32_t offset = 0);
    inline std::optional<char> consume();
    std::optional<std::string> read_token();

    /* Tokenizer internals. */
    TokenType _get_keyword(const std::string);

    /* Helpers */
    inline bool __is_number    (char c);
    inline bool __is_numeric   (const std::string&);
    inline bool __is_whitespace(const std::string&);
};
