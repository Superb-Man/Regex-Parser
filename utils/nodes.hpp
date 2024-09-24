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




class AstNode {
public:
    virtual ~AstNode() = default;
    virtual std::string getLabel() const = 0;
    virtual void print(int indent = 0) const = 0;

    virtual bool matchL(std::string& str, int& pos) = 0;
    virtual bool matchR(std::string& str, int& pos) = 0;
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

    bool matchL(std::string& str, int& pos) override {
        if (left->matchL(str, pos)) {
            return true;
        }
        
        return right->matchL(str, pos);
    }
    
    bool matchR(std::string& str, int& pos) override {
        if (right->matchR(str, pos)) {
            return true;
        }
        
        return left->matchR(str, pos);
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

    bool matchL(std::string& str, int& pos) override {
        if (left->matchL(str, pos) && right->matchL(str, pos)) {
            return true;
        }  
        return false;
    }

    bool matchR(std::string& str, int& pos) override {
        if (right->matchR(str, pos) && left->matchR(str, pos)) {
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

    bool matchL(std::string& str, int& pos) override {
        int originalPos = pos;
        while (true) {
            if (!left->matchL(str, pos)) {
                break;
            }
            originalPos = pos;
        }
        pos = originalPos;

        return true;
    }

    bool matchR(std::string& str, int& pos) override {
        int originalPos = pos;
        while (true) {
            if (!left->matchR(str, pos)) {
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

    bool matchL(std::string& str, int& pos) override {
        //NOT IMPLEMENTED YET
        return true;
    }

    bool matchR(std::string& str, int& pos) override {
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

    bool matchL(std::string& str, int& pos) override {
        if (!left->matchL(str, pos)) {
            return false;
        }
        int originalPos = pos;
        while (true) {
            if (!left->matchL(str, pos)) {
                break;
            }
            originalPos = pos;
        }
        pos = originalPos;

        return true;
    }

    bool matchR(std::string& str, int& pos) override {
        if (!left->matchR(str, pos)) {
            return false;
        }
        int originalPos = pos;
        while (true) {
            if (!left->matchR(str, pos)) {
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

    bool matchL(std::string& str, int& pos) override { 
        //NOT IMPLEMENTED YET
        return true;
    }

    bool matchR(std::string& str, int& pos) override {
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

    bool matchL(std::string& str, int& pos) override {
        if (pos >= str.size()) return false;
        if (str[pos] == ch) {
            pos++;
            return true;
        }
        return false;
    }

    bool matchR(std::string& str, int& pos) override {
        if (pos <= -1) return false;
        if (str[pos] == ch) {
            pos--;
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

    bool matchL(std::string& str, int& pos) override {
        // Match any character except newline
        if (pos >= str.size()) return false;
        if (str[pos] != '\n') {
            pos++;
            return true;
        }
        return false;
    }

    bool matchR(std::string& str, int& pos) override {
        // Match any character except newline
        if (pos <= -1) return false;
        if (str[pos] != '\n') {
            pos--;
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

    bool matchL(std::string& str, int& pos) override {
        return pos == 0;
    }

    bool matchR(std::string& str, int& pos) override {
        return matchL(str, pos);
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

    bool matchL(std::string& str, int& pos) override {
        return pos == str.size() ;
    }

    bool matchR(std::string& str, int& pos) override {
        return matchL(str, pos);
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

    bool matchL(std::string& str, int& pos) override {
        // only matchL atmost one time
        int originalPos = pos;
        if (left->matchL(str, pos)) {
            return true;
        }
        pos = originalPos;
        return true;
    }

    bool matchR(std::string& str, int& pos) override {
        // only matchR atmost one time
        int originalPos = pos;
        if (left->matchR(str, pos)) {
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

    bool matchL(std::string& str, int& pos) override {
        if (pos >= str.size()) return false;
        for (auto node : charClass) {
            if (node.matchL(str, pos)) {
                return true;
            }
        }
        return false;
    }

    bool matchR(std::string& str, int& pos) override {
        if (pos <= -1) return false;
        for (auto node : charClass) {
            if (node.matchR(str, pos)) {
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

    bool matchL(std::string& str, int& pos) override {
        throw std::runtime_error("Not implemented yet");
        if (left->matchL(str, pos)) {
            return false;
        }
        return true;
    }

    bool matchR(std::string& str, int& pos) override {
        throw std::runtime_error("Not implemented yet");
        if (left->matchR(str, pos)) {
            return false;
        }
        return true;
    }
};


// ^a*b+c?$
