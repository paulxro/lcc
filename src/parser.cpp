#include "parser.hpp"

const Token& Parser::peek(size_t offset) const {
    assert(this->m_pos + offset < this->m_tokens.size());

    return this->m_tokens.at(this->m_pos + offset);
}

const Token& Parser::consume() {
    assert(this->m_pos < this->m_tokens.size());

    return this->m_tokens.at(this->m_pos++);
}

std::unique_ptr<ProgramNode> Parser::parse_program() {
    auto program = std::make_unique<ProgramNode>();

    while (this->m_pos < this->m_tokens.size()) {
        program->functions_and_statements.push_back(
            this->parse_statement()
        );
        break;
    }

    return program;
}

bool Parser::_is_operator_token(Token& token) const {
    return token.type > TokenType::_O_TYPE_BEGIN &&
           token.type < TokenType::_O_TYPE_END;
}

bool Parser::_is_literal_token(Token& token) const {
    return token.type == TokenType::l_int;
}

bool Parser::_is_identifier_token(Token& token) const {
    return token.type == TokenType::m_ident;
}

bool Parser::_match(TokenType type) {
    return this->peek().type == type;
}

bool Parser::_match_consume(TokenType type) {
    if (this->_match(type)) {
        this->consume();
        return true;
    }

    return false;
}

bool Parser::_expect(TokenType type, ParseErrorType err) {
    if (this->_match(type))
        return true;

    throw std::runtime_error("Parse error: " + to_string(err));
}

bool Parser::_expect_consume(TokenType type, ParseErrorType err) {
    this->_expect(type, err);
    this->consume();
    return true;
}

bool Parser::_is_func_call() const {
    return this->peek(0).type == TokenType::m_ident &&
           this->peek(1).type == TokenType::b_lparen;
}

/* PARSING FUNCTIONS */
std::unique_ptr<ASTNode> Parser::parse_statement() {
    if (this->_match(TokenType::k_func))
        return this->parse_function_decl();
    if (this->_match(TokenType::k_let))
        return this->parse_var_decl();
    
    return nullptr;
}


std::unique_ptr<FunctionDeclNode> Parser::parse_function_decl() {
    auto function_decl = std::make_unique<FunctionDeclNode>();

    return function_decl;
}

std::unique_ptr<FunctionCallNode> Parser::parse_function_call() {
    auto function_call = std::make_unique<FunctionCallNode>();

    return function_call;
}

std::unique_ptr<VarDeclNode> Parser::parse_var_decl() {
    auto var_decl = std::make_unique<VarDeclNode>();

    this->_expect_consume(TokenType::k_let, ParseErrorType::ExpectedLet);

    this->_expect(TokenType::m_ident, ParseErrorType::ExpectedIdentifier);
    var_decl->name = this->consume().value;

    this->_expect_consume(TokenType::b_colon, ParseErrorType::ExpectedColon);

    this->_expect(TokenType::d_int, ParseErrorType::ExpectedDataType);
    var_decl->type = this->consume().value;

    this->_expect_consume(TokenType::o_equal, ParseErrorType::ExpectedEqual);

    var_decl->value = this->parse_expr_stmt();

    this->_expect_consume(TokenType::b_semi, ParseErrorType::ExpectedSemiColon);

    return var_decl;
}


std::unique_ptr<AssignmentNode> Parser::parse_assignment() {
    auto assign = std::make_unique<AssignmentNode>();

    return assign;
}

std::unique_ptr<ExitNode> Parser::parse_exit_stmt() {
    auto exit_node = std::make_unique<ExitNode>();

    return exit_node;
}

std::unique_ptr<ExprStmtNode> Parser::parse_expr_stmt() {
    auto expr_stmt = std::make_unique<ExprStmtNode>();
    this->consume();

    return expr_stmt;
}

std::unique_ptr<BinaryExprNode> Parser::parse_binary_expr() {
    auto binary_expr = std::make_unique<BinaryExprNode>();

    return binary_expr;
}

std::unique_ptr<IdentNode> Parser::parse_ident_node() {
    auto ident = std::make_unique<IdentNode>();

    return ident;
}

std::unique_ptr<IntLiteralNode> Parser::parse_int_literal() {
    auto int_lit = std::make_unique<IntLiteralNode>();

    return int_lit;
}