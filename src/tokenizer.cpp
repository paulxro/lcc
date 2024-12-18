#include "tokenizer.hpp"

std::unordered_map<std::string, TokenType> Tokenizer::m_keywords = {
    {"exit", TokenType::k_exit},
};

std::unordered_set<char> Tokenizer::m_keybreak = {
    '(', ')', ' ', ';'
};

Tokenizer::Tokenizer(std::string source) {
    this->m_pos = 0;
    this->m_src = source;
}

std::vector<Token> Tokenizer::tokenize() {
    while (true) {
        std::optional<std::string> token_buf = this->read_token();

        if (!token_buf.has_value()) {
            break;
        }

        std::cout << "[" << this->m_pos << "] " << token_buf.value() << "\n";

        // break;

        this->append_token(token_buf.value());
        this->m_pos += token_buf.value().length() + 1;

    }

    return this->m_tokens;
}


std::optional<std::string> Tokenizer::read_token() {
    std::string token_buf = "";

    uint32_t ahead = 0;
    

    while (true) {
        std::optional<char> curr = this->peek(ahead);

        if (!curr.has_value())
            return {};

        if (this->is_keybreak(curr.value()))
            break;

        token_buf += curr.value();
        ahead++;
    }

    return token_buf;
}

inline std::optional<char> Tokenizer::peek(uint32_t offset = 1) {
    if (this->m_pos + offset >= this->m_src.length()) {
        return {};
    }

    return this->m_src.at(this->m_pos + offset);
}

void Tokenizer::append_token(std::string& value) {
    if (this->m_keywords.contains(value)) {
        this->m_tokens.push_back(
            {.value = value, .type = TokenType::k_exit}
        );

        return;
    }
    
    // this->m_tokens.push_back({ .value = value, .type = TokenType::k_exit});
}

inline bool Tokenizer::is_keybreak(char val) {
    return this->m_keybreak.contains(val);
}

inline std::optional<char> Tokenizer::consume() {
    return this->m_src.at(this->m_pos++);
}