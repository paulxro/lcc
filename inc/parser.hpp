#include "parser_nodes.hpp"
#include "tokenizer.hpp"

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) 
        : m_tokens(std::move(tokens)) {}
    
    std::unique_ptr<ProgramNode> parse_program();

private:
    std::vector<Token> m_tokens;
    size_t m_pos = 0;

    const Token& peek   (size_t offset = 0) const;
    const Token& consume(size_t offset = 0);

    /* Parsing for ASTNodes. */
    std::unique_ptr<FunctionDeclNode> parse_function_decl();
    std::unique_ptr<FunctionCallNode> parse_function_call();
    std::unique_ptr<VarDeclNode>      parse_var_decl     ();
    std::unique_ptr<AssignmentNode>   parse_assignment   ();
    std::unique_ptr<ExitNode>         parse_exit_stmt    ();
    std::unique_ptr<ExprStmtNode>     parse_expr_stmt    ();
    std::unique_ptr<BinaryExprNode>   parse_binary_expr  ();
    std::unique_ptr<IdentNode>        parse_ident_node   ();
    std::unique_ptr<IntLiteralNode>   parse_int_literal  ();
    std::unique_ptr<ASTNode>          parse_statement    ();
};