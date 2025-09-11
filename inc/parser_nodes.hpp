#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <sstream>
#include <ostream>

inline std::string indent_str(int indent) {
    return std::string(indent * 2, ' '); // 2 spaces per level
}

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual std::string to_string(int indent = 0) const = 0;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

/* Program root */
struct ProgramNode : ASTNode {
    std::vector<ASTNodePtr> functions_and_statements;

    std::string to_string(int indent = 0) const override {
        std::string s;
        for (auto& stmt : functions_and_statements) {
            s += stmt->to_string(indent) + "\n";
        }
        return s;
    }
};

/* Function definition */
struct FunctionDeclNode : ASTNode {
    std::string return_type;
    std::string name;

    struct Param {
        std::string name;
        std::string type;
    };

    std::vector<Param> params;
    std::vector<ASTNodePtr> body;

    std::string to_string(int indent = 0) const override {
        std::ostringstream oss;
        oss << indent_str(indent) << "fn " << return_type << " " << name << "(";
        for (size_t i = 0; i < params.size(); i++) {
            if (i > 0) oss << ", ";
            oss << params[i].type << " " << params[i].name;
        }
        oss << ") {\n";
        for (auto& stmt : body) {
            oss << stmt->to_string(indent + 1) << "\n";
        }
        oss << indent_str(indent) << "}";
        return oss.str();
    }
};

struct FunctionCallNode : ASTNode {
    std::string name;
    std::vector<ASTNodePtr> args;

    std::string to_string(int indent = 0) const override {
        std::ostringstream oss;
        oss << indent_str(indent) << name << "(";
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0) oss << ", ";
            oss << args[i]->to_string();
        }
        oss << ")";
        return oss.str();
    }
};

/* Statements */
struct VarDeclNode : ASTNode {
    std::string name;
    std::string type;
    ASTNodePtr value;

    std::string to_string(int indent = 0) const override {
        return indent_str(indent) + "let " + name + " : " + type + " = " +
               (value ? value->to_string() : "<null>") + ";";
    }
};

struct AssignmentNode : ASTNode {
    std::string name;
    ASTNodePtr value;

    std::string to_string(int indent = 0) const override {
        return indent_str(indent) + name + " = " +
               (value ? value->to_string() : "<null>") + ";";
    }
};

struct ExitNode : ASTNode {
    ASTNodePtr value;

    std::string to_string(int indent = 0) const override {
        return indent_str(indent) + "exit " +
               (value ? value->to_string() : "<null>") + ";";
    }
};

struct ExprStmtNode : ASTNode {
    ASTNodePtr expr;

    std::string to_string(int indent = 0) const override {
        return indent_str(indent) +
               (expr ? expr->to_string() : "<null>");
    }
};

/* Expressions */
struct BinaryExprNode : ASTNode {
    std::string op;
    ASTNodePtr left;
    ASTNodePtr right;

    std::string to_string(int indent = 0) const override {
        return indent_str(indent) + "(" +
               (left ? left->to_string() : "<null>") +
               " " + op + " " +
               (right ? right->to_string() : "<null>") + ")";
    }
};

struct IdentNode : ASTNode {
    std::string name;

    std::string to_string(int indent = 0) const override {
        return indent_str(indent) + name;
    }
};

struct IntLiteralNode : ASTNode {
    int value;

    std::string to_string(int indent = 0) const override {
        return indent_str(indent) + std::to_string(value);
    }
};

/* Operator<< for any ASTNode */
inline std::ostream& operator<<(std::ostream& os, const ASTNode& node) {
    os << node.to_string();
    return os;
}
