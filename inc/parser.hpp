#pragma once

#include "tokenizer.hpp"
#include <memory>
#include <vector>
#include <string>
#include <optional>

/* === Abstract Syntax Tree (AST) Nodes === */

struct ASTNode {
    virtual ~ASTNode() = default;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

/* Program root */
struct ProgramNode : ASTNode {
    std::vector<ASTNodePtr> functions_and_statements;
};

/* Function definition */
struct FunctionDeclNode : ASTNode {
    std::string return_type;
    std::string name;
    std::optional<std::pair<std::string, std::string>> param; // (type, name)
    std::vector<ASTNodePtr> body;
};

/* Statements */
struct VarDeclNode : ASTNode {
    std::string name;
    std::string type;
    ASTNodePtr value;
};

struct AssignmentNode : ASTNode {
    std::string name;
    ASTNodePtr value;
};

struct ExitNode : ASTNode {
    ASTNodePtr value;
};

struct ExprStmtNode : ASTNode {
    ASTNodePtr expr;
};

/* Expressions */
struct BinaryExprNode : ASTNode {
    std::string op;
    ASTNodePtr left;
    ASTNodePtr right;
};

struct IdentNode : ASTNode {
    std::string name;
};

struct IntLiteralNode : ASTNode {
    int value;
};

/* === Parser === */

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    std::unique_ptr<ProgramNode> parse_program();

private:
    std::vector<Token> m_tokens;
    size_t m_pos = 0;

    /* Utility methods */
    const Token& peek(size_t offset = 0) const;
    const Token& advance();
    bool match(TokenType type);
    bool expect(TokenType type, const std::string& error_msg);

    /* Parsing functions (mirror grammar rules) */
    std::unique_ptr<FunctionDeclNode> parse_function_decl();
    std::unique_ptr<VarDeclNode>      parse_var_decl();
    std::unique_ptr<AssignmentNode>   parse_assignment();
    std::unique_ptr<ExitNode>         parse_exit_stmt();
    std::unique_ptr<ExprStmtNode>     parse_expr_stmt();
    ASTNodePtr                        parse_statement();

    /* Expressions */
    ASTNodePtr parse_expr();
    ASTNodePtr parse_equality();
    ASTNodePtr parse_addsub();
    ASTNodePtr parse_term();
    ASTNodePtr parse_factor();

    /* Helpers */
    bool at_end() const;
};
