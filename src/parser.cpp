#include "parser.hpp"
#include <stdexcept>
#include <iostream>

Parser::Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)), m_pos(0) {}

/* === Utility methods === */

const Token& Parser::peek(size_t offset) const {
    if (m_pos + offset >= m_tokens.size()) {
        static Token eof_token{"", m_unknown};
        return eof_token;
    }
    return m_tokens[m_pos + offset];
}

const Token& Parser::advance() {
    if (!at_end()) m_pos++;
    return peek(0);
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::expect(TokenType type, const std::string& error_msg) {
    if (peek().type == type) {
        advance();
        return true;
    }
    throw std::runtime_error("Parse error: " + error_msg);
}

bool Parser::at_end() const {
    return m_pos >= m_tokens.size();
}

/* === Top-level === */

std::unique_ptr<ProgramNode> Parser::parse_program() {
    auto program = std::make_unique<ProgramNode>();
    while (!at_end()) {
        if (peek().type == k_func) {
            program->functions_and_statements.push_back(parse_function_decl());
        } else {
            program->functions_and_statements.push_back(parse_statement());
        }
    }
    return program;
}

/* === Functions === */

std::unique_ptr<FunctionDeclNode> Parser::parse_function_decl() {
    expect(k_func, "Expected 'fn' keyword");

    // return type
    expect(d_int, "Only 'int' return type supported");
    std::string return_type = "int";

    // function name
    if (peek().type != m_ident)
        throw std::runtime_error("Expected function name");
    std::string name = peek().value;
    advance();

    // parameters
    expect(b_lparen, "Expected '('");
    std::optional<std::pair<std::string, std::string>> param;
    if (peek().type != b_rparen) {
        expect(d_int, "Only int parameter type supported");
        std::string param_type = "int";
        if (peek().type != m_ident)
            throw std::runtime_error("Expected parameter name");
        std::string param_name = peek().value;
        advance();
        param = std::make_pair(param_type, param_name);
    }
    expect(b_rparen, "Expected ')'");

    // body
    expect(b_left_curl, "Expected '{' for function body");
    auto body = std::vector<ASTNodePtr>();
    while (peek().value != "}") {
        body.push_back(parse_statement());
    }
    expect(b_right_curl, "Expected '}' at end of function");

    auto node = std::make_unique<FunctionDeclNode>();
    node->return_type = return_type;
    node->name = name;
    node->param = param;
    node->body = std::move(body);
    return node;
}

/* === Statements === */

ASTNodePtr Parser::parse_statement() {
    if (peek().type == k_let) return parse_var_decl();
    if (peek().type == m_ident && peek(1).type == o_equal) return parse_assignment();
    if (peek().type == k_exit) return parse_exit_stmt();
    return parse_expr_stmt();
}

std::unique_ptr<VarDeclNode> Parser::parse_var_decl() {
    expect(k_let, "Expected 'let' keyword");

    if (peek().type != m_ident)
        throw std::runtime_error("Expected variable name");
    std::string name = peek().value;
    advance();

    expect(b_colon, "Expected ':' after variable name");
    expect(d_int, "Only 'int' type supported");
    std::string type = "int";

    expect(o_equal, "Expected '=' after type");
    auto value = parse_expr();

    expect(b_semi, "Expected ';' after variable declaration");

    auto node = std::make_unique<VarDeclNode>();
    node->name = name;
    node->type = type;
    node->value = std::move(value);
    return node;
}

std::unique_ptr<AssignmentNode> Parser::parse_assignment() {
    if (peek().type != m_ident)
        throw std::runtime_error("Expected variable name in assignment");
    std::string name = peek().value;
    advance();

    expect(o_equal, "Expected '=' in assignment");
    auto value = parse_expr();

    expect(b_semi, "Expected ';' after assignment");

    auto node = std::make_unique<AssignmentNode>();
    node->name = name;
    node->value = std::move(value);
    return node;
}

std::unique_ptr<ExitNode> Parser::parse_exit_stmt() {
    expect(k_exit, "Expected 'exit' keyword");
    expect(b_lparen, "Expected '(' after exit");
    auto value = parse_expr();
    expect(b_rparen, "Expected ')' after exit argument");
    expect(b_semi, "Expected ';' after exit");
    auto node = std::make_unique<ExitNode>();
    node->value = std::move(value);
    return node;
}

std::unique_ptr<ExprStmtNode> Parser::parse_expr_stmt() {
    auto expr = parse_expr();
    expect(b_semi, "Expected ';' after expression");
    auto node = std::make_unique<ExprStmtNode>();
    node->expr = std::move(expr);
    return node;
}

/* === Expressions === */

ASTNodePtr Parser::parse_expr() {
    return parse_equality();
}

ASTNodePtr Parser::parse_equality() {
    auto left = parse_addsub();
    if (match(o_equal_equal)) {
        auto right = parse_addsub();
        auto node = std::make_unique<BinaryExprNode>();
        node->op = "==";
        node->left = std::move(left);
        node->right = std::move(right);
        return node;
    }
    return left;
}

ASTNodePtr Parser::parse_addsub() {
    auto left = parse_term();
    while (peek().type == o_plus || peek().type == o_sub) {
        std::string op = peek().value;
        advance();
        auto right = parse_term();
        auto node = std::make_unique<BinaryExprNode>();
        node->op = op;
        node->left = std::move(left);
        node->right = std::move(right);
        left = std::move(node);
    }
    return left;
}

ASTNodePtr Parser::parse_term() {
    return parse_factor(); // no * or / yet
}

ASTNodePtr Parser::parse_factor() {
    if (peek().type == l_int) {
        int value = std::stoi(peek().value);
        advance();
        auto node = std::make_unique<IntLiteralNode>();
        node->value = value;
        return node;
    }
    if (peek().type == m_ident) {
        std::string name = peek().value;
        advance();
        auto node = std::make_unique<IdentNode>();
        node->name = name;
        return node;
    }
    if (match(b_lparen)) {
        auto expr = parse_expr();
        expect(b_rparen, "Expected ')'");
        return expr;
    }
    throw std::runtime_error("Unexpected token in expression: " + peek().value);
}
