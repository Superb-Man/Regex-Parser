#include "nodes.hpp"

// all grammar
// R -> U
// U -> U OR C | C
// C -> C K | K
// K -> K * | K + | K ? | S
// S -> ( R ) | ( ? ! R ) | [ LITERAL DASH LITERAL ] | LITERAL | . | ^ | $


void printRule(std::string rule) {
    std::cout << rule << std::endl;
}

class ParseRegex {
private:
    std::vector<Token> tokenStream;
    int currToken;

    AstNode* parse_R() {
        printRule("R -> U");
        return parse_U();
    }

    AstNode* parse_U() {
        bool isU = false;

        AstNode* ast = parse_C();
        while (isMatch(OR)) {
            AstNode* left = ast;
            AstNode* right = parse_C();
            ast = new OrAstNode(left, right);
            isU = true;
        }

        (isU) ? printRule("U -> U OR C") : printRule("U -> C");
        return ast;
    }

    AstNode* parse_C() {
        bool isC = false;

        AstNode* ast = parse_K();
        while (currToken < tokenStream.size()) {
            int tokenType = tokenStream[currToken].index;
            if (tokenType == LITERAL || tokenType == OPEN_PAREN || tokenType == DOT || 
                    tokenType == CARET || tokenType == DOLLAR || tokenType == OPEN_BRACKET) {
                AstNode* left = ast;
                AstNode* right = parse_K();
                ast = new SeqAstNode(left,right);
                
                isC = true;
            
            } else {
                break;
            }
        }

        (isC) ? printRule("C -> C K") : printRule("C -> K");

        return ast;
    }

    AstNode* parse_K() {
        AstNode* ast = parse_S();
        while (currToken < tokenStream.size()) {
            int tokenType = tokenStream[currToken].index;
            if (tokenType == STAR) {
                currToken++;

                if (currToken < tokenStream.size() && tokenStream[currToken].index == QUESTION) {
                    printRule("K -> K * ?");
                    ast = new StarNonGreedyAstNode(ast);
                    currToken++;
                } 
                else {
                    printRule("K -> K *");
                    ast = new StarAstNode(ast);
                }
            } else if (tokenType == PLUS) {
                currToken++;

                if (currToken < tokenStream.size() && tokenStream[currToken].index == QUESTION) {
                    printRule("K -> K + ?");
                    // std::cout << "Found a plus and question" <<std::endl;
                    ast = new PlusNonGreedyAstNode(ast);
                    currToken++;
                } 
                else {
                    printRule("K -> K +");
                    ast = new PlusAstNode(ast);
                }
            } else if (tokenType == QUESTION) {
                printRule("K -> K ?");
                ast = new QuestionAstNode(ast);
                currToken++;
            } 
            else {
                printRule("K -> S");
                break;
            }
        }

        return ast;
    }

    AstNode* parse_S() {
        if (isMatch(OPEN_PAREN)) {
            if (isMatch(QUESTION) && isMatch(EXCLAMATION)) {
                printRule("S -> ( ? ! R )");
                AstNode* ast = parse_R();
                check(CLOSED_PAREN);
                return new NegativeLookAheadAstNode(ast);
            } else {
                printRule("S -> ( R )");
                AstNode* ast = parse_R();
                check(CLOSED_PAREN);
                return ast;
            }
        } else if (isMatch(OPEN_BRACKET)) {
            //  S -> [ LITERAL DASH LITERAL ]
            printRule("S -> [ L - L ]");

            std::set<LiteralCharacterAstNode> charSet;
            // std::vector<char>list;
            char last = '\0';

            while (currToken < tokenStream.size()) {
                //debug
                // std::cout << "Cur stream char : " << tokenStream[currToken].val <<std::endl;
                
                if (isMatch(CLOSED_BRACKET)) {
                    currToken--;
                    break;
                } else if (isMatch(LITERAL)) {
                    char ch = tokenStream[currToken - 1].val;
                    charSet.insert(LiteralCharacterAstNode(ch));
                    last = ch;
                    // list.push_back(ch);
                    currToken--;
                    // if (tokenStream[currToken-1].val == '*') {
                        // std::cout << "Found a star" <<std::endl;
                        // break;
                    // }
                } else if (isMatch(DASH)) { // dash matched
                    // if set is empty, then the dash is a literal
                    // if the next character is closed bracket it still a literall
                    // otherwise, it is a range 
                    if (charSet.empty() | currToken == tokenStream.size() | 
                        tokenStream[currToken].index == CLOSED_BRACKET) {
                            charSet.insert(LiteralCharacterAstNode('-'));
                            currToken--;
                    } else {
                        // char last = list.back();                        
                        // list.pop_back();

                        int start = int(last);
                        int endChar = int(tokenStream[currToken].val);


                        //debug 
                        // std::cout << "start " << start << " and End " << endChar <<std::endl;
                        
                        //insert the range into the set   
                        if (start > endChar) throw std::runtime_error("start index is higher than end index");
                        for (int it = start; it <= endChar; it++) {
                            charSet.insert(LiteralCharacterAstNode(char(it)));
                        }
                        //now its a token!

                    }
                } 
                // else {
                //     throw std::runtime_error("Not handled for this token");
                // }
                currToken++;
            }
            check(CLOSED_BRACKET);
            return new CharacterClassAstNode(charSet);
        } else if (isMatch(LITERAL)) {
            printRule("S -> L");
            return new LiteralCharacterAstNode(tokenStream[currToken - 1].val);
        } else if (isMatch(DOT)) {
            printRule("S -> .");
            return new DotAstNode;
        } else if (isMatch(CARET)) {
            printRule("S -> ^");
            return new CaretAstNode;
        } else if (isMatch(DOLLAR)) {
            printRule("S -> $");
            return new DollarAstNode;
        } else {
            throw std::runtime_error("In parse_S: Not expected token");
        }
    }

    bool isMatch(int tokenType) {
        if (currToken >= tokenStream.size()) {
            return false;
        }
        if (tokenStream[currToken].index == tokenType) {
            currToken++;
            return true;
        }
        return false;
    }

    void check(int tokenType) {
        if (!isMatch(tokenType)) {
            throw std::runtime_error("In check: Not expected token");
        }
    }

public:
    ParseRegex(const std::vector<Token>& tokenStream) {
        this->tokenStream = tokenStream;
        currToken = 0;
    }

    AstNode* parse() {
        AstNode* ast = parse_R();
        if (currToken < tokenStream.size()) {
            std::cout << "Token stream size: " << tokenStream.size() << std::endl;
            std::cout << "Current token: " << currToken << std::endl;
            throw std::runtime_error("IN PARSE: Not expected token");
        }
        return ast;
    }
};
