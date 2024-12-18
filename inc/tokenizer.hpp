#pragma once

#include <fstream>
#include <vector>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

enum TokenType {
    k_exit,
    l_int,
};

struct Token {
    std::string value;
    TokenType type;
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
};

