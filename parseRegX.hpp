#include "nodes.hpp"

// R -> U
// U -> C | U OR C
// C -> K | C K
// K -> S | K *
// S -> ( R ) | [ L DASH L ] | DOT | CARET | DOLLAR | L
// L -> LITERAL


class ParseRegex {
private:
    std::vector<Token> tokenStream;
    int currToken;

    AstNode* parse_R() {
        return parse_U();
    }

    AstNode* parse_U() {
        AstNode* ast = parse_C();
        while (isMatch(OR)) {
            AstNode* left = ast;
            AstNode* right = parse_C();
            ast = new OrAstNode(left, right);
        }
        return ast;
    }

    AstNode* parse_C() {
        AstNode* ast = parse_K();
        while (currToken < tokenStream.size()) {
            int tokenType = tokenStream[currToken].index;
            if (tokenType == LITERAL || tokenType == OPEN_PAREN || tokenType == DOT || 
                    tokenType == CARET || tokenType == DOLLAR) {
                AstNode* left = ast;
                AstNode* right = parse_K();
                ast = new SeqAstNode(left,right);
            } else {
                break;
            }
        }
        return ast;
    }

    AstNode* parse_K() {
        AstNode* ast = parse_S();
        while (currToken < tokenStream.size()) {
            int tokenType = tokenStream[currToken].index;
            if (tokenType == STAR) {
                ast = new StarAstNode(ast);
                currToken++;
            } else if (tokenType == PLUS) {
                ast = new PlusAstNode(ast);
                currToken++;
            } else if (tokenType == QUESTION) {
                ast = new QuestionAstNode(ast);
                currToken++;
            } else {
                break;
            }
        }
            return ast;
    }

    AstNode* parse_S() {
        if (isMatch(OPEN_PAREN)) {
            AstNode* ast = parse_R();
            check(CLOSED_PAREN);
            return ast;
        } else if (isMatch(OPEN_BRACKET)) {
            //  S -> [ LITERAL DASH LITERAL ]
            std::set<LiteralCharacterAstNode> charSet;
            char last = '\0';
            while (currToken < tokenStream.size()) {
                if (isMatch(CLOSED_BRACKET)) {
                    break;
                }
                if (isMatch(LITERAL)) {
                    char ch = tokenStream[currToken - 1].val;
                    charSet.insert(LiteralCharacterAstNode(ch));
                    last = ch;
                } else if (isMatch(DASH)) {
                    // if set is empty, then the dash is a literal
                    // if the next character is closed bracket it still a literall
                    // otherwise, it is a range 
                    if (charSet.empty() | currToken == tokenStream.size() | 
                        tokenStream[currToken].index == CLOSED_BRACKET) {
                            charSet.insert(LiteralCharacterAstNode('-'));
                    } else {
                        char start = last;
                        char endChar = tokenStream[currToken].val;
                        //insert the range into the set   

                        for (char itr = start; itr <= endChar; itr++) {
                            charSet.insert(LiteralCharacterAstNode(itr));
                        }
                        currToken++;
                        //now its a token!
                    }
                }
            }
            currToken++;

            return new CharacterClassAstNode(charSet);
        } else if (isMatch(LITERAL)) {
            return new LiteralCharacterAstNode(tokenStream[currToken - 1].val);
        } else if (isMatch(DOT)) {
            return new DotAstNode;
        } else if (isMatch(CARET)) {
            return new CaretAstNode;
        } else if (isMatch(DOLLAR)) {
            return new DollarAstNode;
        } else {
            throw std::runtime_error("Expected token");
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
            throw std::runtime_error("Expected token");
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
            throw std::runtime_error("Unchecked token");
        }
        return ast;
    }
};

