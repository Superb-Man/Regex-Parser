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
        std::cout << "Enter text: ";
        std::getline(std::cin, text);
            std::vector<Token> tokens = lexer(regex);
        for (const auto& token : tokens) {
            std::cout << token << std::endl; 
        }
        
        ParseRegex parser(tokens);
        std::shared_ptr<AstNode> ast = parser.parse();
        ast->print();

    }
}
