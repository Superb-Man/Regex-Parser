#pragma once
#include "utils/parseRegX.hpp"
#include"utils/NI_log.hpp"


int main() {
    std::string regex; 
    std::string text;
    // regex = "a+b*(c+de)*f";
    // regex = "a*b+c?(d+)?";
    // regex = "a[a-zA-Z0-9.]";
    // regex = "a*b+c?d*?xyz+";
    regex = "a(?!lara)62";
    regex = "a(?![a-z])62";
    regex = "a(.6)*d";
    // regex = "a[0-9]*62";
    // regex = "acd*b";
    // regex = "a.?b";
    // regex = "a[a-z]*b";
    // regex = "a(.*bc|c*)+";
    // regex = "a([a-z].)*";
    regex = "a(.bcd[a-z])*62";
    regex = "a.*b";
    regex = "(.a.*)*bc";
    regex = "a(.sb)*bc*";
    regex = "a*?b";
    std::cout << regex << std::endl;


    if (regex == "exit") {
        return 0;
    }
    AstNode* ast = nullptr;


    

    while (true) {
        std::cout << "Enter regex: ";
        std::getline(std::cin, regex);
        std::vector<Token> tokens = lexer(regex);
            
        ParseRegex parser(tokens);
        AstNode* ast = nullptr;

        try {
            ast = parser.parse();
            ast->print();
            std::cout << std::endl;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Invalid regular expression" << std::endl;
        }
    }
}
