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

enum TokenType {
    k_exit,
    k_func,
    d_int,
    l_int,
    l_float,
    m_keybreak,
    m_unknown,
};

struct Token {
    std::string value;
    TokenType type;

    explicit operator bool() const {
        return value.length();
    }
};

struct TokenPusher {
    Token& token;
    std::vector<Token>& tokens;
    ~TokenPusher() {
        assert((token.type != TokenType::m_unknown) && "Pushing back unknown token.");
        tokens.push_back(token);
    }
};


class Tokenizer {
public:
    Tokenizer(std::string);
    std::vector<Token> tokenize();

private:
    static std::unordered_map<std::string, TokenType> m_keywords;
    static std::unordered_set<char>        m_keybreak;
    std::vector<Token> m_tokens;

    uint32_t m_pos = 0;
    std::string m_src;

    /* 
     * Technically peek always returns a single character,
     * but string comparisons are easier to do if we don't
     * worry about type conversions.
     */
    inline std::optional<char>        peek(uint32_t);
    inline std::optional<char>        consume();
           std::optional<std::string> read_token();
    

    inline bool is_keybreak(char);
           void append_token(std::string&);
    
    inline bool _is_keyword(Token&);
    inline bool _is_integer(Token&);
    inline bool _is_float  (Token&);


    inline bool __is_numeric(const std::string&);
};

