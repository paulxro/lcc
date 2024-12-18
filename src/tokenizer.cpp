#include "tokenizer.hpp"

std::unordered_map<std::string, TokenType> Tokenizer::m_keywords = {
    {"exit", TokenType::k_exit},
    {"fn"  , TokenType::k_func},
    {"int" , TokenType::d_int }
};

std::unordered_set<char> Tokenizer::m_keybreak = {
    '(', ')', ' ', ';', ':'
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
        
        std::string separator = std::string{
            this->peek(token_buf.value().length()).value()
        };

        // std::cout << "[" << this->m_pos << "] " << token_buf.value() << "\n";

        this->append_token(token_buf.value());
        this->append_token(separator);
        this->m_pos += token_buf.value().length() + 1;

    }

    return this->m_tokens;
}


std::optional<std::string> Tokenizer::read_token() {
    std::string token_buf = "";

    uint32_t ahead = 0;
    if (!this->peek(ahead).has_value())
        return {};

    while (true) {
        std::optional<char> curr = this->peek(ahead);

        /* Should not expect file to end during token parsing. */
        if (!curr.has_value())
            throw std::runtime_error("Parsed EOF when expecting token.");

        if (this->is_keybreak(curr.value()))
            break;

        token_buf += curr.value();
        ahead++;
    }

    return token_buf;
}

inline std::optional<char> Tokenizer::peek(uint32_t offset = 1) {
    if (this->m_pos + offset >= this->m_src.length())
        return {};

    return this->m_src.at(this->m_pos + offset);
}

void Tokenizer::append_token(std::string& value) {
    struct Token token = {.value = value, .type = TokenType::m_unknown};

    /* Want to append token breaks, if not spaces. */
    if (this->is_keybreak(value.at(0))) {
        if (value != " ") {
            /* Manually push token to tokens. */
            token.type = TokenType::m_keybreak;
            this->m_tokens.push_back(token);
        }
        return;
    }

    /* RAII token pusher, for conciseness. */
    struct TokenPusher pusher = {.token = token, .tokens = this->m_tokens};


    if (this->_is_keyword(token)) return;
    if (this->_is_integer(token)) return;
    if (this->_is_float  (token)) return;
}

inline bool Tokenizer::is_keybreak(char val) {
    return this->m_keybreak.contains(val);
}

inline std::optional<char> Tokenizer::consume() {
    return this->m_src.at(this->m_pos++);
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

inline bool Tokenizer::__is_numeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}