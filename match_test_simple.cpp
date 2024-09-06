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
    // regex = "a(ab)*?abc";
    std::cout << regex << std::endl;


    if (regex == "exit") {
        return 0;
    }
    std::vector<Token> tokens = lexer(regex);
    // for (const auto& token : tokens) {
    //     std::cout << token << std::endl; 
    // }
        
    ParseRegex parser(tokens);
    AstNode* ast = nullptr;

    try {
        ast = parser.parse();
        ast->print();
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Invalid regular expression" << std::endl;
        return 0;
    }

    bool flag = canSolve(parser.getAstNodes());
    if (!flag) { 
        std::cout << "NOT YET IMPLEMENTED" << std::endl;
        return 0;
    }
    

    while (true) {
        std::cout << "Enter text: ";
        std::getline(std::cin, text);
        int pos = 0;
        ast->match(text,pos);
        std::cout << "Matched up to position = " << pos << std::endl;
        pos = (pos >= text.size()) ? pos-1 : pos;
        std::cout << "Matched prefix: " << text.substr(0,pos) << std::endl;
    }
}