#include <fstream>
#include <iostream>
#include <string>

static inline std::string ERR  = "Error";
static inline std::string WARN = "Warn";
static inline std::string INFO = "Info";

static inline void print_message(const std::string& type, const std::string& msg) {
    std::cout << "[LCC] " << type << ": " << msg << "\n";
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_message(ERR, "No input files");
        exit(1);
    }

    std::ifstream in_file(argv[1]);

    if (!in_file.is_open()) {
        print_message(ERR, std::string("Cannot open file ") + argv[1]);
        exit(1);
    }

    std::string token;
    // while (std::get) {

    // }
}

