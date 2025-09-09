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

    // Operators
    {"+", o_plus},
    {"-", o_sub},
    {"=", o_equal},
    {"==", o_equal_equal},
};

/* Batch tokenizer: consume all tokens until EOF. */
std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    reset(); // make sure we start fresh

    while (true) {
        auto tok = next_token();
        if (!tok) break;
        tokens.push_back(*tok);
    }
    return tokens;
}

/* Stream tokenizer: get next token and advance. */
std::optional<Token> Tokenizer::next_token() {
    if (m_peeked) {
        auto tok = m_peeked;
        m_peeked.reset();
        return tok;
    }

    auto text = read_token();
    if (!text) return std::nullopt;

    // Check keywords
    auto kw_it = m_keywords.find(*text);
    if (kw_it != m_keywords.end()) {
        return Token{*text, kw_it->second};
    }

    // Check symbols and operators
    auto sym_it = m_keybreak.find(*text);
    if (sym_it != m_keybreak.end()) {
        return Token{*text, sym_it->second};
    }

    // Check literals (integer for now)
    if (__is_numeric(*text)) {
        return Token{*text, l_int};
    }

    // Otherwise identifier
    if (std::isalpha((*text)[0]) || (*text)[0] == '_') {
        return Token{*text, m_ident};
    }

    // Fallback unknown
    return Token{*text, m_unknown};
}

/* Stream tokenizer: peek at next token without advancing. */
std::optional<Token> Tokenizer::peek_token() {
    if (!m_peeked) {
        m_peeked = next_token();
    }
    return m_peeked;
}

/* Reset tokenizer to start of input. */
void Tokenizer::reset() {
    m_pos = 0;
    m_peeked.reset();
}

/* Character helpers */
std::optional<char> Tokenizer::peek(uint32_t offset) {
    if (m_pos + offset < m_src.size()) {
        return m_src[m_pos + offset];
    }
    return std::nullopt;
}

std::optional<char> Tokenizer::consume() {
    if (m_pos < m_src.size()) {
        return m_src[m_pos++];
    }
    return std::nullopt;
}

/* Core: read the next raw token string. */
std::optional<std::string> Tokenizer::read_token() {
    // Skip whitespace
    while (true) {
        auto c = peek();
        if (!c || !std::isspace(static_cast<unsigned char>(*c))) break;
        consume();
    }

    if (m_pos >= m_src.size()) return std::nullopt;

    char c = *consume();

    // Multi-char operator check (==)
    if (c == '=') {
        if (auto nc = peek(); nc && *nc == '=') {
            consume(); // eat second '='
            return "==";
        }
        return "=";
    }

    // Single-char operators and symbols
    std::string s(1, c);
    if (m_keybreak.count(s)) {
        return s;
    }

    // Identifiers/keywords
    if (std::isalpha(c) || c == '_') {
        std::string ident(1, c);
        while (true) {
            auto nc = peek();
            if (nc && (std::isalnum(*nc) || *nc == '_')) {
                ident.push_back(*consume());
            } else break;
        }
        return ident;
    }

    // Numbers
    if (std::isdigit(c)) {
        std::string number(1, c);
        while (true) {
            auto nc = peek();
            if (nc && std::isdigit(*nc)) {
                number.push_back(*consume());
            } else break;
        }
        return number;
    }

    // Unknown token
    return std::string(1, c);
}

TokenType Tokenizer::_get_keyword(const std::string str) {
    if (str.empty()) return TokenType::m_unknown;
    auto res = Tokenizer::m_keywords.find(str);
    if (res == Tokenizer::m_keywords.end())
        return TokenType::m_unknown;
    return res->second;
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
