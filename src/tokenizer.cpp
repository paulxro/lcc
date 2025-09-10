#include "tokenizer.hpp"

/* Define keyword and symbol maps. */
std::unordered_map<std::string, TokenType> Tokenizer::m_keywords = {
    {"exit", k_exit},
    {"fn"  , k_func},
    {"let" , k_let},
    {"int" , d_int}
};

/* Keybreak includes symbols and operators */
std::unordered_map<std::string, TokenType> Tokenizer::m_keybreak = {
    {"(", b_lparen},
    {")", b_rparen},
    {":", b_colon},
    {";", b_semi},
    {"{", b_left_curl},
    {"}", b_right_curl},
    {",", b_comma},

    // Operators
    {"+", o_plus},
    {"-", o_sub},
    {"=", o_equal},
    {"==", o_equal_equal},
};

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> result {};

    for (auto curr = this->next_token(); curr; curr = this->next_token())
        result.push_back(*curr);
    
    return result;
}

std::optional<Token> Tokenizer::next_token() {
    if (this->m_peeked) {
        auto token = this->m_peeked;
        this->m_peeked.reset();
        return token;
    }

    auto next_str = this->read_token();
    if (!next_str)
        return std::nullopt;

    auto token_type = this->classify_token(*next_str);
    if (!token_type)
        return std::nullopt;
    
    return Token {.value = *next_str, .type = *token_type};
}

std::optional<Token> Tokenizer::peek_token() {
    if (!this->m_peeked)
        this->m_peeked = next_token();

    return this->m_peeked;
}

std::optional<char> Tokenizer::peek(uint32_t offset) {
    if (this->m_pos + offset >= this->m_src.length())
        return std::nullopt;
    return this->m_src[this->m_pos + offset];
}

std::optional<char> Tokenizer::consume() {
    if (this->m_pos >= this->m_src.length())
        return std::nullopt;
    return this->m_src[this->m_pos++];
}

std::optional<std::string> Tokenizer::read_token() {
    this->_consume_whitespace();

    auto curr = this->peek();
    if (!curr) return std::nullopt;

    if (this->__is_number(*curr))
        return this->_parse_number();
    
    if (this->__is_alpha(*curr))
        return this->_parse_alpha();

    // Parse symbols/operators
    return this->_parse_symbol();
}


std::optional<TokenType> Tokenizer::classify_token(const std::string& str) {
    if (str.empty()) return std::nullopt;

    if (this->__is_numeric(str))
        return TokenType::l_int;

    auto keyword_find = Tokenizer::m_keywords.find(str);
    if (keyword_find != Tokenizer::m_keywords.end())
        return keyword_find->second;

    auto keybreak_find = Tokenizer::m_keybreak.find(str);
    if (keybreak_find != Tokenizer::m_keybreak.end())
        return keybreak_find->second;

    return TokenType::m_ident;
}

void Tokenizer::_consume_whitespace() {
    while(true) {
        auto ch = peek();
        if (!ch || !std::isspace((int)(*ch)))
            return;
        this->consume();
    }
}

std::string Tokenizer::_parse_number() {
    assert(this->peek() && this->__is_number(*this->peek()));

    std::string num(1, *this->consume());

    while (true) {
        auto ch = this->peek();
        if (!ch || !this->__is_number(*ch))
            break;
        num.push_back(*this->consume());
    }

    return num;
}

std::string Tokenizer::_parse_alpha() {
    assert(this->peek() && this->__is_alpha(*this->peek()));

    std::string value(1, *this->consume());

    while (true) {
        auto ch = this->peek();
        if (!ch || !(this->__is_alpha(*ch) || this->__is_number(*ch)))
            break;
        value.push_back(*this->consume()); 
    }

    return value;
}

std::string Tokenizer::_parse_symbol() {
    // Try to match longest possible operator/symbol in m_keybreak
    std::string symbol;

    for (int len = 2; len >= 1; --len) {
        symbol.clear();
        for (int i = 0; i < len; ++i) {
            auto ch = this->peek(i);
            if (!ch) break;
            symbol.push_back(*ch);
        }
        if (Tokenizer::m_keybreak.find(symbol) != Tokenizer::m_keybreak.end()) {
            for (int i = 0; i < (int)symbol.size(); ++i) consume();
            return symbol;
        }
    }

    // If nothing matched, consume one char as unknown
    return std::string(1, *this->consume());
}

TokenType Tokenizer::_get_keyword(const std::string str) {
    if (str.empty()) return TokenType::m_unknown;
    auto res = Tokenizer::m_keywords.find(str);
    if (res == Tokenizer::m_keywords.end())
        return TokenType::m_unknown;
    return res->second;
}

bool Tokenizer::__is_alpha(char c) {
    return ('A' <= c && c <= 'Z') ||
           ('a' <= c && c <= 'z') ||
           (c == '_');
}

bool Tokenizer::__is_number(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Tokenizer::__is_numeric(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), 
                                     [](unsigned char c){ return std::isdigit(c); });
}

bool Tokenizer::__is_whitespace(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(),
                                     [](unsigned char c){ return std::isspace(c); });
}
