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
    virtual MatchResult match(std::string text, int index) const = 0;
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
        return "|";
    }
    MatchResult match(const std::string text, int index) const override {
        MatchResult leftMatch = left->match(text, index);
        if (leftMatch.isMatch) {
            return leftMatch;
        }
        return right->match(text, index);
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
        return "x";
    }

    MatchResult match(const std::string text, int index) const override {
        MatchResult leftMatch = left->match(text, index);
        if (!leftMatch.isMatch) {
            return {false, 0};
        }
        MatchResult rightMatch = right->match(text, leftMatch.index);
        if (!rightMatch.isMatch) {
            return {false, 0};
        }
        return {true,leftMatch.index + rightMatch.index};
    }

};

class StarAstNode : public AstNode {

public:
    std::shared_ptr<AstNode> left;

    explicit StarAstNode(std::shared_ptr<AstNode> left) {
        this->left = left;
    }
    std::string getLabel() const override {
        return "*";
    }

    MatchResult match(const std::string text, int index) const override {
        int currIndex = 0;
        while (true) {
            MatchResult match = left->match(text, currIndex + index);
            if (!match.isMatch) {
                break;
            }
            currIndex += match.index;
        }
        return {true, currIndex};
    }


};

class PlusAstNode : public AstNode {
public:
    std::shared_ptr<AstNode> left ;

    explicit PlusAstNode(std::shared_ptr<AstNode> left) {
        this->left = left ;
    }
    std::string getLabel() const override {
        return "+";
    }

    MatchResult match(const std::string text, int index) const override {
        MatchResult fMatch = left->match(text, index);
        if (!fMatch.isMatch) {
            return {false, 0};
        }
        int currIndex = fMatch.index;
        while (true) {
            MatchResult match = left->match(text, currIndex + index);
            if (!match.isMatch) {
                break;
            }
            currIndex += match.index;
        }
        return {true, currIndex};
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

    MatchResult match(const std::string text, int index) const override {
        if (index < text.size() && text[index] == ch) {
            return {true, 1};
        }
        return {false, 0};
    }
};

class DotAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return ".";
    }

    MatchResult match(const std::string text, int index) const override {
        if (index < text.size()) {
            return {true, 1};
        }
        return {false, 0};
    }
};

class CaretAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return "^";
    }

    MatchResult match(const std::string text, int index) const override {
        if (index == 0) {
            return {true, 0};
        }
        return {false, 0};
    }
};

class DollarAstNode : public AstNode {
public:
    std::string getLabel() const override {
        return "$";
    }

    MatchResult match(const std::string text, int index) const override {
        if (index == text.size()) {
            return {true, 0};
        }
        return {false, 0};
    }
};

class QuestionAstNode : public AstNode {
public:
    std::shared_ptr<AstNode> left;

    explicit QuestionAstNode(std::shared_ptr<AstNode> left) {
        this->left = left;
    }
    std::string getLabel() const override {
        return "?";
    }

    MatchResult match(const std::string text, int index) const override {
        MatchResult match = left->match(text, index);
        if (match.isMatch) {
            return match;
        }
        return {true, 0};
    }
};