#include"lex.hpp"

struct MatchResult {
    bool isMatch;
    int index;

    MatchResult(bool isMatch, int index) {
        this->isMatch = isMatch;
        this->index = index;
    }
};


class AstNode {
public:
    virtual ~AstNode() = default;
    virtual std::string getLabel() const = 0;

    virtual void print(int indent = 0) const = 0;
};

class OrAstNode : public AstNode {

public:
    std::shared_ptr<AstNode> left;
    std::shared_ptr<AstNode> right;

    OrAstNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right) {
        this->left = left;
        this->right = right;
    }
    std::string getLabel() const override {
        return "OR";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
        if (right) right->print(indent + 2);
    }

};

class SeqAstNode : public AstNode {

public:
    std::shared_ptr<AstNode> left;
    std::shared_ptr<AstNode> right;

    SeqAstNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right){
        this->left = left;
        this->right = right;
    }
    std::string getLabel() const override {
        return "AND";
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
        if (right) right->print(indent + 2);
    }

};

class StarAstNode : public AstNode {

public:
    std::shared_ptr<AstNode> left;

    explicit StarAstNode(std::shared_ptr<AstNode> left) {
        this->left = left;
    }
    std::string getLabel() const override {
        return "STAR";
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
    }
};

class PlusAstNode : public AstNode {
public:
    std::shared_ptr<AstNode> left ;

    explicit PlusAstNode(std::shared_ptr<AstNode> left) {
        this->left = left ;
    }
    std::string getLabel() const override {
        return "PLUS";
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
        if (left) left->print(indent + 2);
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
        std::cout << std::string(indent, ' ') << "LITERAL ; " << getLabel() << std::endl;
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
};

class CaretAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return "CARET";
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
    }
};

class DollarAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return "END";
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
    }

};

class QuestionAstNode : public AstNode {
public:
    std::shared_ptr<AstNode> left;

    explicit QuestionAstNode(std::shared_ptr<AstNode> left) {
        this->left = left;
    }
    std::string getLabel() const override {
        return "OPTIONAL";
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << getLabel() << std::endl;
    }
};
