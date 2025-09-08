#include "tokenizer.hpp"

std::unordered_map<std::string, TokenType> Tokenizer::m_keywords = {
    {"exit", TokenType::k_exit},
    {"fn"  , TokenType::k_func},
    {"int" , TokenType::d_int },
    {"let" , TokenType::k_let },
};

std::unordered_map<std::string, TokenType> Tokenizer::m_keybreak = {
    {"(", TokenType::b_lparen },
    {")", TokenType::b_rparen },
    {";", TokenType::b_semi   },
    {":", TokenType::b_colon  },
    {" ", TokenType::m_unknown},
};

Tokenizer::Tokenizer(std::string source) {
    this->m_pos = 0;
    this->m_src = source;
}

std::vector<Token> Tokenizer::tokenize() {
    while (true) {
        std::optional<std::string> token_buf = this->read_token();

        if (!token_buf.has_value())
            break;

        this->append_token(token_buf.value());
        this->m_pos += token_buf.value().length();

    }

    return this->m_tokens;
}


std::optional<std::string> Tokenizer::read_token() {
    uint32_t ahead = 0;
    std::string token_buf = "";
    std::optional<char> curr = this->peek(ahead);

    /* Consume whitespace. */
    while (curr.has_value() && curr.value() == ' ') {
        this->consume();
        curr = this->peek(ahead);
        continue;
    }

    if (!curr.has_value())
        return {};

    /* Parse next token char-by-char. */
    while (true) {
        /* Should not expect file to end during token parsing. */
        if (!curr.has_value())
            throw std::runtime_error("Parsed EOF when expecting token.");

        /*
         * TODO: Probably make this nicer?
         * 
         * If a keybreak is by itself, append it as a token.
         * 
         * If we have already parsed part of a token, and come
         * across a keybreak, leave the keybreak on the stream.
         */
        if (this->is_keybreak(curr.value())) {
            if (!token_buf.length())
                token_buf += curr.value();
            break;
        }

        token_buf += curr.value();
        ahead++;

        curr = this->peek(ahead);
    }

    return token_buf;
}

inline std::optional<char> Tokenizer::peek(uint32_t offset = 1) {
    if (this->m_pos + offset >= this->m_src.length())
        return {};

    return this->m_src.at(this->m_pos + offset);
}

void Tokenizer::append_token(std::string& value) {
    assert(!this->__is_whitespace(value) && "Attempted to append whitespace token.");

    struct Token token = {.value = value, .type = TokenType::m_unknown};

    /* RAII token pusher, for conciseness. */
    struct TokenPusher pusher = {.token = token, .tokens = this->m_tokens};


    if (this->_is_keybreak  (token)) return;
    if (this->_is_keyword   (token)) return;
    if (this->_is_integer   (token)) return;
    if (this->_is_float     (token)) return;
    if (this->_is_identifier(token)) return;
}

inline bool Tokenizer::is_keybreak(char val) {
    std::string str = std::string{val};
    return this->is_keybreak(str);
}

inline bool Tokenizer::is_keybreak(std::string& val) {
    return this->m_keybreak.find(val) != this->m_keybreak.end();
}

inline std::optional<char> Tokenizer::consume() {
    return this->m_src.at(this->m_pos++);
}

inline bool Tokenizer::_is_keybreak(Token& token) {
    assert(token);
    assert(token.type == TokenType::m_unknown);

    auto k_pos = this->m_keybreak.find(token.value);
    if (k_pos == this->m_keybreak.end())
        return false;
    
    token.type = k_pos->second;
    return true;
}

inline bool Tokenizer::_is_keyword(Token& token) {
    assert(token);
    assert(token.type == TokenType::m_unknown);

    auto k_pos = this->m_keywords.find(token.value);
    if (k_pos == this->m_keywords.end())
        return false;

    token.type = k_pos->second;
    return true;
}

inline bool Tokenizer::_is_integer(Token& token) {
    assert(token);
    assert(token.type == TokenType::m_unknown);

    if (!this->__is_numeric(token.value))
        return false;
    
    token.type = TokenType::l_int;
    return true;
}

inline bool Tokenizer::_is_float(Token& token) {
    assert(token);
    assert(token.type == TokenType::m_unknown);

    std::size_t pos = token.value.find('.');

    if (pos == std::string::npos)
        return false;
    
    std::string before = token.value.substr(0, pos );
    std::string  after = token.value.substr(pos + 1);

    if (!this->__is_numeric(before))
        return false;
    if (!this->__is_numeric(after))
        return false;
    
    token.type = TokenType::l_float;
    return true;
}

inline bool Tokenizer::_is_identifier(Token& token) {
    assert(token);
    assert(token.type == TokenType::m_unknown);

    if (this->__is_number(token.value.at(0)))
        return false;
    
    token.type = TokenType::m_ident;
    return true;
}

inline bool Tokenizer::__is_number(const char val) {
    return val >= '0' && val <= '9';
}

inline bool Tokenizer::__is_numeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

inline bool Tokenizer::__is_whitespace(const std::string& str) {
    return str.find_first_not_of("\t\n\b\f\r ") == std::string::npos;
}