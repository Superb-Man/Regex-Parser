#pragma once
#include<bits/stdc++.h>

#define OR 1
#define STAR 2
#define PLUS 3
#define OPEN_PAREN 4
#define CLOSED_PAREN 5
#define LITERAL 6
#define QUESTION 7
#define DOT 8
#define CARET 9
#define DOLLAR 10
#define OPEN_BRACKET 11
#define CLOSED_BRACKET 12
#define DASH 13
#define EXCLAMATION 14
#define SEQ 15


std::pair<int,std::string> getTokenType(char token) {
    if(token == '|') return {OR, "OR"} ;
    if(token == '*') return {STAR, "STAR"} ;
    if(token == '+') return {PLUS, "PLUS"} ;
    if(token == '(') return {OPEN_PAREN, "OPEN_PAREN"} ;
    if(token == ')') return {CLOSED_PAREN, "CLOSED_PAREN"} ;
    if (token == '?') return {QUESTION, "QUESTION"} ;
    if (token == '.') return {DOT, "DOT"} ;
    if (token == '^') return {CARET, "CARET"} ;
    if (token == '$') return {DOLLAR, "DOLLAR"} ;
    if (token == '[') return {OPEN_BRACKET, "OPEN_BRACKET"} ;
    if (token == ']') return {CLOSED_BRACKET, "CLOSED_BRACKET"} ;
    if (token == '-') return {DASH, "DASH"} ;
    if (token == '!') return {EXCLAMATION, "EXCLAMATION"} ;


    return {LITERAL, "LITERAL"} ;
}

char getTokenVal(int token) {
    if(token == OR) return '|' ;
    if(token == STAR) return '*' ;
    if(token == PLUS) return '+' ;
    if(token == OPEN_PAREN) return '(' ;
    if(token == CLOSED_PAREN) return ')' ;
    if (token == QUESTION) return '?' ;
    if (token == DOT) return '.' ;
    if (token == CARET) return '^' ;
    if (token == DOLLAR) return '$' ;
    if (token == OPEN_BRACKET) return '[' ;
    if (token == CLOSED_BRACKET) return ']' ;
    if (token == DASH) return '-' ;
    if (token == EXCLAMATION) return '!' ;
    
    return token ;
}

struct Token {
    std::string name;
    char val;
    int index;
    std::string content;

    Token(std::string name, char val,int index,std::string content = "") {
        this->name = name ;
        this->val = val ;
        this->index = index;
        this->content = content;
    }
    //overload the  cout operator
    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "Token(" << token.name << ", " << token.val << ")";
        return os;
    }
};

std::vector<Token> lexer(std::string regex) {
    std::vector<Token> tokenStream;
    for(int i = 0; i < regex.size(); i++) {
        auto it = getTokenType(regex[i]);
        Token token(it.second, regex[i],it.first);
        tokenStream.push_back(token);
    }
    return tokenStream;
}
