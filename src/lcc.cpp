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
    auto prog = parser.parse_program();

    std::cout << prog->to_string() << std::endl;

    // for (const Token& t : tokens) {
    //     std::cout << t.value << " " << to_string(t.type) << std::endl;
    // }
}
