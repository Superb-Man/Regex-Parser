// #pragma once
// #include "utils/parseRegX.hpp"
// #include"utils/NI_log.hpp"
// #include "utils/Simplifier.hpp"



// int main() {
//     std::string regex; 
//     std::string text;
//     // regex = "a+b*(c+de)*f";
//     // regex = "a*b+c?(d+)?";
//     // regex = "a[a-zA-Z0-9.]";
//     // regex = "a*b+c?d*?xyz+";
//     // regex = "a(?!lara)62";
//     // regex = "a(?![a-z])62";
//     regex = "a(.6)*d";
//     // regex = "a[0-9]62";
//     // regex = "acd*b";
//     // regex = "a.?b";
//     // regex = "a[a-z]b";
//     // regex = "a(.bc|c*)+";
//     // regex = "a([a-z].)*";
//     regex = "a(.s|a)*62";
//     // regex = "[a-z]*b";
//     regex = "a(.sb)*bc*";
//     // regex = "cd[a-z]|a(.bc)*b*";
//     // regex = "ad|(b|.c)*";
//     // regex = "a.?b";
//     regex = "a(a|bc)*(bc|b)";
//     // regex = "aaab";
//     regex = "a(ab|cd?e)*abc";
//     // regex = "([0-9a-d.]|a*(b|(cd+|e*f)))bd";
//     // regex = "a.*b";
//     // regex = "a((.)*)bcd+(((c)d)e)";
//     regex = "a.*b[0-9]+b+b*";
//     regex = "a(..)*d";
//     regex = "aa?b";
//     // regex = "[7-9ac-e]";
//     std::cout << regex << std::endl;


//     if (regex == "exit") {
//         return 0;
//     }
//     std::vector<Token> tokens = lexer(regex);
//     // for (const auto& token : tokens) {
//     //     std::cout << token << std::endl; 
//     // }
        
//     ParseRegex parser(tokens);
//     AstNode* ast = nullptr;

//     try {
//         ast = parser.parse();
//         // ast->print();
//         // simply regex
//         // std::cout << "Simplified regex: ";
//         // regex = Simplifier(ast).simplifiedRegex;
//         // std::cout << regex << std::endl;

//         // // again parse the simplified regex
//         // tokens = lexer(regex);
//         // parser = ParseRegex(tokens);
//         // ast = parser.parse();
//         // std::cout << std::endl;
//     } catch (const std::exception& e) {
//         std::cout << e.what() << std::endl;
//         std::cout << "Invalid regular expression" << std::endl;
//         return 0;
//     }

//     bool flag = canSolve(parser.getAstNodes());
//     if (!flag) { 
//         std::cout << "NOT YET IMPLEMENTED" << std::endl;
//         return 0;
//     }
    

//     while (true) {
//         std::cout << "Enter text: ";
//         std::getline(std::cin, text);
//         int pos = 0;
//         int bpos = text.size() - 1;
//         bool l = ast->matchL(text,pos);
//         bool r = ast->matchR(text,bpos);
//         if ((pos == text.size() && l) || (bpos == -1 && r)) {
//             std::cout << "Matched completely" << std::endl;
//             if (pos == text.size()) {
//                 std::cout << "prefix matched :" << text.substr(0,pos) << std::endl;
//             } 
//             else {
//                 std::cout << "suffix matched :" << text.substr(bpos+1) << std::endl;
//             }

//         } 
//         else {
//             std::cout << "Not matched" << std::endl;
//         }
//         // if ((pos == text.size() && bpos == -1)) {
//         //     std::cout << "Matched completely" << std::endl;

//         // } else {
//         //     std::cout << "Not matched" << std::endl;
//         // }
//     }
// }

// // ajsfbvudjfvbjbbbbcddddddf