#include "parser_nodes.hpp"
#include "parser_errs.hpp"
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
    const Token& consume();

    /* Parsing for ASTNodes. */
    std::unique_ptr<ASTNode>          parse_statement    ();

    std::unique_ptr<FunctionDeclNode> parse_function_decl();
    std::unique_ptr<FunctionCallNode> parse_function_call();
    std::unique_ptr<VarDeclNode>      parse_var_decl     ();
    std::unique_ptr<AssignmentNode>   parse_assignment   ();
    std::unique_ptr<ExitNode>         parse_exit_stmt    ();
    std::unique_ptr<ExprStmtNode>     parse_expr_stmt    ();
    std::unique_ptr<BinaryExprNode>   parse_binary_expr  ();
    std::unique_ptr<IdentNode>        parse_ident_node   ();
    std::unique_ptr<IntLiteralNode>   parse_int_literal  ();
    

    bool _is_operator_token(Token&) const;
    bool _is_literal_token(Token&) const;
    bool _is_identifier_token(Token&) const;

    /*
     * Determines if a function call starts
     * with current token.
     * 
     * Current Logic:
     * 
     *  - Current token is an identifier
     *  - Next token is a l_paren token.
     */
    inline bool _is_func_call() const;

    /* 
     * Matches current token's type to parameter.
     *  - If matches:        returns true.
     *  - If does not match: returns false.
     */
    inline bool _match        (TokenType);
    inline bool _match_consume(TokenType);

    /* 
     * [[ STRONGER VERSION OF Parser::_match ]]
     * Matches current token's type to parameter.
     *  - If matches:        returns true.
     *  - If does not match: throws error.
     */
    inline bool _expect        (TokenType, ParseErrorType);
    inline bool _expect_consume(TokenType, ParseErrorType);
};