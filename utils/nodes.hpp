#pragma once
#include "lex.hpp"
#include <iostream>
#include <set>
#include <string>

//Forward declaration of AstNode
class AstNode;
//other nodes
class OrAstNode;
class SeqAstNode;
class StarAstNode;
class StarNonGreedyAstNode;
class PlusAstNode;
class PlusNonGreedyAstNode;
class LiteralCharacterAstNode;
class DotAstNode;
class CaretAstNode;
class DollarAstNode;
class QuestionAstNode;
class CharacterClassAstNode;
class NegativeLookAheadAstNode;

//functions
bool tryMatch(AstNode* node, std::string& str, int& pos);



class AstNode {
public:
    virtual ~AstNode() = default;
    virtual std::string getLabel() const = 0;
    virtual void print(int indent = 0) const = 0;

    virtual bool match(std::string& str, int& pos) = 0;
};

class OrAstNode : public AstNode {
public:
    AstNode* left;
    AstNode* right;

    OrAstNode(AstNode* left, AstNode* right) {
        this->left = left;
        this->right = right;
    }
    
    ~OrAstNode() {
        delete left;
        delete right;
    }

    std::string getLabel() const override {
        return "OR";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
        if (right) right->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override {
        if (left->match(str, pos)) {
            return true;
        }
        
        return right->match(str, pos);
    }
    
};

class SeqAstNode : public AstNode {
public:
    AstNode* left;
    AstNode* right;

    SeqAstNode(AstNode* left, AstNode* right) {
        this->left = left;
        this->right = right;
    }

    ~SeqAstNode() {
        delete left;
        delete right;
    }

    std::string getLabel() const override {
        return "SEQ";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
        if (right) right->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override {
        if (left->match(str, pos) && right->match(str, pos)) {
            return true;
        }  
        return false;
    }
};

class StarAstNode : public AstNode {
public:
    AstNode* left;

    explicit StarAstNode(AstNode* left) {
        this->left = left;
    }

    ~StarAstNode() {
        delete left;
    }

    std::string getLabel() const override {
        return "STAR";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override {
        int originalPos = pos;
        while (true) {
            if (!left->match(str, pos)) {
                break;
            }
            originalPos = pos;
        }
        pos = originalPos;

        return true;
    }
};

class StarNonGreedyAstNode : public AstNode {
public:
    AstNode* left;

    explicit StarNonGreedyAstNode(AstNode* left) {
        this->left = left;
    }

    ~StarNonGreedyAstNode() {
        delete left;
    }

    std::string getLabel() const override {
        return "STAR_NON_GREEDY";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override {
        //NOT IMPLEMENTED YET
        return true;
    }



};




class PlusAstNode : public AstNode {
public:
    AstNode* left;

    explicit PlusAstNode(AstNode* left) {
        this->left = left;
    }

    ~PlusAstNode() {
        delete left;
    }

    std::string getLabel() const override {
        return "PLUS";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override {
        if (!left->match(str, pos)) {
            return false;
        }
        int originalPos = pos;
        while (true) {
            if (!left->match(str, pos)) {
                break;
            }
            originalPos = pos;
        }
        pos = originalPos;

        return true;
    }
};


class PlusNonGreedyAstNode : public AstNode {
public:
    AstNode* left;

    explicit PlusNonGreedyAstNode(AstNode* left) {
        this->left = left;
    }

    ~PlusNonGreedyAstNode() {
        delete left;
    }

    std::string getLabel() const override {
        return "PLUS_NON_GREEDY";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override { 
        //NOT IMPLEMENTED YET
        return true;
    }
};


class LiteralCharacterAstNode : public AstNode {
public:
    char ch;

    explicit LiteralCharacterAstNode(char ch) {
        this->ch = ch;
    }

    std::string getLabel() const override {
        return std::string(1, ch);
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "LITERAL , " << getLabel() << std::endl;
    }

    bool operator<(const LiteralCharacterAstNode& other) const {
        return std::string(1,ch) < std::string(1,other.ch);
    }

    bool match(std::string& str, int& pos) override {
        if (pos >= str.size()) return false;
        if (str[pos] == ch) {
            pos++;
            return true;
        }
        return false;
    }
};

class DotAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return "DOT";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
    }

    bool match(std::string& str, int& pos) override {
        // Match any character except newline
        if (pos >= str.size()) return false;
        if (str[pos] != '\n') {
            pos++;
            return true;
        }
        return false;
    }
};

class CaretAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return "CARET";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
    }

    bool match(std::string& str, int& pos) override {
        return pos == 0;
    }
};

class DollarAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return "DOLARA";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
    }

    bool match(std::string& str, int& pos) override {
        return pos == str.size() ;
    }
};

class QuestionAstNode : public AstNode {
public:
    AstNode* left;

    explicit QuestionAstNode(AstNode* left) {
        this->left = left;
    }

    ~QuestionAstNode() {
        delete left;
    }

    std::string getLabel() const override {
        return "QUESTION";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override {
        // only match atmost one time
        int originalPos = pos;
        if (left->match(str, pos)) {
            return true;
        }
        pos = originalPos;
        return true;
        
        
    }
};

class CharacterClassAstNode : public AstNode {
public:
    std::set<LiteralCharacterAstNode>charClass;

    CharacterClassAstNode(std::set<LiteralCharacterAstNode>& charClass) {
        this->charClass = charClass;
    }

    std::string getLabel() const override {
        return "SQBRACKET";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        for (auto node : charClass) {
            node.print(indent+2);
        }
    }

    bool match(std::string& str, int& pos) override {
        if (pos >= str.size()) return false;
        for (auto node : charClass) {
            if (node.match(str, pos)) {
                return true;
            }
        }
        return false;
    }
};
class NegativeLookAheadAstNode : public AstNode {
public:
    AstNode* left;

    explicit NegativeLookAheadAstNode(AstNode* left) {
        this->left = left;
    }

    ~NegativeLookAheadAstNode() {
        delete left;
    }

    std::string getLabel() const override {
        return "NEGATIVE_LOOKAHEAD";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
    }

    bool match(std::string& str, int& pos) override {
        throw std::runtime_error("Not implemented yet");
        if (left->match(str, pos)) {
            return false;
        }
        return true;
    }
};


// ^a*b+c?$
