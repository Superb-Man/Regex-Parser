#include "parseRegX.hpp"


int main() {
    std::string regex; 
    std::string text;
    

    while (true) {
        std::cout << "Enter regex: ";
        std::getline(std::cin, regex);
        if (regex == "exit") {
            break;
        }
        // std::cout << "Enter text: ";
        // std::getline(std::cin, text);
        std::vector<Token> tokens = lexer(regex);
        // for (const auto& token : tokens) {
        //     std::cout << token << std::endl; 
        // }
        
        ParseRegex parser(tokens);
        try {
            AstNode* ast = parser.parse();
            ast->print();
            std::cout << std::endl;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Invalid regular expression" << std::endl;
        }
    }
}
