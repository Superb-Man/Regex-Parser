#pragma once
#include "utils/parseRegX.hpp"
#include"utils/NI_log.hpp"
#include "utils/Simplifier.hpp"



int main() {
    std::string regex; 
    std::string text;
    // regex = "a+b*(c+de)*f";
    // regex = "a*b+c?(d+)?";
    // regex = "a[a-zA-Z0-9.]";
    // regex = "a*b+c?d*?xyz+";
    // regex = "a(?!lara)62";
    // regex = "a(?![a-z])62";
    regex = "a(.6)*d";
    // regex = "a[0-9]62";
    // regex = "acd*b";
    // regex = "a.?b";
    // regex = "a[a-z]b";
    // regex = "a(.bc|c*)+";
    // regex = "a([a-z].)*";
    regex = "a(.s|a)*62";
    // regex = "[a-z]*b";
    regex = "a(.sb)*bc*";
    // regex = "cd[a-z]|a(.bc)*b*";
    // regex = "ad|(b|.c)*";
    // regex = "a.?b";
    regex = "a(a|bc)*(bc|b)";
    // regex = "aaab";
    regex = "a(ab|cd?e)*abc";
    // regex = "([0-9a-d.]|a*(b|(cd+|e*f)))bd";
    // regex = "a.*b";
    // regex = "a((.)*)bcd+(((c)d)e)";
    // regex = "a.*b[0-9]+b+b*";
    // regex = "a*a+";
    regex = "[7-9ac-e]";
    std::cout << regex << std::endl;


    if (regex == "exit") {
        return 0;
    }
    // for (const auto& token : tokens) {
    //     std::cout << token << std::endl; 
    // }
        
    AstNode* ast = nullptr;

    while (true) {
        std::cout << "Enter a regex: ";
        std::getline(std::cin, regex);
        std::vector<Token> tokens = lexer(regex);
        ParseRegex parser(tokens);
        try {
            ast = parser.parse();
            ast->print();
            // simply regex
            std::cout << "Simplified regex: ";
            regex = Simplifier(ast).simplifiedRegex;
            std::cout << regex << std::endl;
            std::cout << std::endl;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Invalid regular expression" << std::endl;
            return 0;
        }
    }
}