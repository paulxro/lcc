#include <fstream>
#include <iostream>
#include <string>

#include "tokenizer.hpp"
#include "parser.hpp"

static inline std::string CRIT = "Critical";
static inline std::string ERR  = "Error";
static inline std::string WARN = "Warn";
static inline std::string INFO = "Info";

static inline void print_message(const std::string& type, const std::string& msg) {
    std::cout << "[LCC] " << type << ": " << msg << "\n";
}

static inline void print_exit(const std::string& type, const std::string& msg) {
    print_message(type, msg);
    exit(1);
}

/* === Debug printer for AST === */
void print_ast(const ASTNode* node, int indent = 0) {
    auto pad = std::string(indent, ' ');

    if (auto prog = dynamic_cast<const ProgramNode*>(node)) {
        std::cout << pad << "Program\n";
        for (auto& child : prog->functions_and_statements) {
            print_ast(child.get(), indent + 2);
        }
    }
    else if (auto fn = dynamic_cast<const FunctionDeclNode*>(node)) {
        std::cout << pad << "FunctionDecl: " << fn->name << " -> " << fn->return_type << "\n";
        if (fn->param) {
            std::cout << pad << "  Param: " << fn->param->second
                      << " : " << fn->param->first << "\n";
        }
        for (auto& stmt : fn->body) {
            print_ast(stmt.get(), indent + 2);
        }
    }
    else if (auto var = dynamic_cast<const VarDeclNode*>(node)) {
        std::cout << pad << "VarDecl: " << var->name << " : " << var->type << "\n";
        print_ast(var->value.get(), indent + 2);
    }
    else if (auto asn = dynamic_cast<const AssignmentNode*>(node)) {
        std::cout << pad << "Assignment: " << asn->name << "\n";
        print_ast(asn->value.get(), indent + 2);
    }
    else if (auto ex = dynamic_cast<const ExitNode*>(node)) {
        std::cout << pad << "Exit\n";
        print_ast(ex->value.get(), indent + 2);
    }
    else if (auto es = dynamic_cast<const ExprStmtNode*>(node)) {
        std::cout << pad << "ExprStmt\n";
        print_ast(es->expr.get(), indent + 2);
    }
    else if (auto bin = dynamic_cast<const BinaryExprNode*>(node)) {
        std::cout << pad << "BinaryExpr: " << bin->op << "\n";
        print_ast(bin->left.get(), indent + 2);
        print_ast(bin->right.get(), indent + 2);
    }
    else if (auto id = dynamic_cast<const IdentNode*>(node)) {
        std::cout << pad << "Ident: " << id->name << "\n";
    }
    else if (auto lit = dynamic_cast<const IntLiteralNode*>(node)) {
        std::cout << pad << "IntLiteral: " << lit->value << "\n";
    }
    else {
        std::cout << pad << "(Unknown AST Node)\n";
    }
}

int main(int argc, char **argv) {
    if (argc < 2)
        print_exit(CRIT, "No input files");

    std::ifstream in_file(argv[1]);

    if (!in_file.is_open())
        print_exit(ERR, std::string("Cannot open file ") + argv[1]);
    
    std::string source(
        (std::istreambuf_iterator<char>(in_file)),
        std::istreambuf_iterator<char>()
    );

    Tokenizer tokenizer(source);
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(tokens);

    try {
        auto program = parser.parse_program();
        print_ast(program.get());
    } catch (const std::exception& e) {
        print_exit(ERR, e.what());
    }
}
