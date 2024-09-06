///////////////////////UNHANDLED CASES////////////////////////

/**
 * @brief Not Implemented cases
 * REGEX = .*
 * REGEX = .+
 * REGEX = .?
 * REGEX = []*
 * REGEX = []+
 * REGEX = []?
 * REGEX = ?!
 * REGEX = one of the above regex | one of the above regex , one of the above regex | handled regex, handled regex | one of the above regex
 * 
 */



#pragma once
#include"parseRegX.hpp"
#include"nodes.hpp"
//forward declaration
AstNode* LEFT_OF_STAR_PLUS_QUESTION(AstNode* node);
AstNode* OR_has_STAR_PLUS_QUESTION(AstNode* node);
AstNode* OR_has_DOT_CHAR(AstNode* node);
AstNode* STAR_PLUS__QUESTION_has_DOT_CHAR(AstNode* node);
AstNode* STAR_PLUS__QUESTION_has_OR(AstNode* node);

bool canSolve(std::vector<AstNode*> astNodes) {
    // std::cout << "AST NODES SIZE : " << astNodes.size() << std::endl;
    if (astNodes.size() == 0) {
        return false;
    }
    for (AstNode* node : astNodes) {
        // if it a or node , check if the subtree has star or plus or question
        // if node found ,from that node , check if it has dot or character class
        if (OrAstNode* orNode = dynamic_cast<OrAstNode*>(node)) {
            AstNode* starPlusQuestion = OR_has_STAR_PLUS_QUESTION(orNode);
            if (starPlusQuestion) {
                AstNode* dotChar = STAR_PLUS__QUESTION_has_DOT_CHAR(starPlusQuestion);
                if (dotChar) {
                    std::cout << "DEBUG VUL PAISI" << std::endl;
                    return false;
                }
            }
        }

        // if it a star or plus or question , ques node , check if the subtree has dot or character class
        if (StarAstNode* starNode = dynamic_cast<StarAstNode*>(node)) {
            AstNode* dotChar = LEFT_OF_STAR_PLUS_QUESTION(starNode);
            if (dotChar) {
                std::cout << "DEBUG VUL PAISI" << std::endl;
                return false;
            }
        }

        if (PlusAstNode* plusNode = dynamic_cast<PlusAstNode*>(node)) {
            AstNode* dotChar = LEFT_OF_STAR_PLUS_QUESTION(plusNode);
            if (dotChar) {
                return false;
            }
        }

        if (QuestionAstNode* questionNode = dynamic_cast<QuestionAstNode*>(node)) {
            AstNode* dotChar = LEFT_OF_STAR_PLUS_QUESTION(questionNode);
            if (dotChar) {
                return false;
            }
        }


        // if its star , plus or question , check if the subtree has OR node
        // if it has OR node,from that node check if it has dot or character class
        if (StarAstNode* starNode = dynamic_cast<StarAstNode*>(node)) {
            AstNode* orNode = STAR_PLUS__QUESTION_has_OR(starNode);
            if (orNode) {
                AstNode* dotChar = OR_has_DOT_CHAR(orNode);
                if (dotChar) {
                    return false;
                }
            }
        }

        if (PlusAstNode* plusNode = dynamic_cast<PlusAstNode*>(node)) {
            AstNode* orNode = STAR_PLUS__QUESTION_has_OR(plusNode);
            if (orNode) {
                AstNode* dotChar = OR_has_DOT_CHAR(orNode);
                if (dotChar) {
                    return false;
                }
            }
        }

        if (QuestionAstNode* questionNode = dynamic_cast<QuestionAstNode*>(node)) {
            AstNode* orNode = STAR_PLUS__QUESTION_has_OR(questionNode);
            if (orNode) {
                AstNode* dotChar = OR_has_DOT_CHAR(orNode);
                if (dotChar) {
                    return false;
                }
            }
        }

        // if node is non greedy star or plus then not handled yet
        if (StarNonGreedyAstNode* starNode = dynamic_cast<StarNonGreedyAstNode*>(node)) {
            return false;
        }

        if (PlusNonGreedyAstNode* plusNode = dynamic_cast<PlusNonGreedyAstNode*>(node)) {
            return false;
        }
    }
    return true;
}

AstNode* LEFT_OF_STAR_PLUS_QUESTION(AstNode* node) {
    if (DotAstNode* dotNode = dynamic_cast<DotAstNode*>(node)) {
        return dotNode;
    }
    if (CharacterClassAstNode* charNode = dynamic_cast<CharacterClassAstNode*>(node)) {
        return charNode;
    }
    return nullptr;
}




AstNode* OR_has_STAR_PLUS_QUESTION(AstNode* node) {
    // check if star or plus can be reached
    if (StarAstNode* starNode = dynamic_cast<StarAstNode*>(node)) {
        return starNode;
    }
    if (PlusAstNode* plusNode = dynamic_cast<PlusAstNode*>(node)) {
        return plusNode;
    }
    if (QuestionAstNode* questionNode = dynamic_cast<QuestionAstNode*>(node)) {
        return questionNode;
    }
    // check in subtree to find star or plus
    if (OrAstNode* orNode = dynamic_cast<OrAstNode*>(node)) {
        AstNode* left = OR_has_STAR_PLUS_QUESTION(orNode->left);
        AstNode* right = OR_has_STAR_PLUS_QUESTION(orNode->right);
        if (left) return left;
        if (right) return right;
    }
    if (SeqAstNode* seqNode = dynamic_cast<SeqAstNode*>(node)) {
        AstNode* left = OR_has_STAR_PLUS_QUESTION(seqNode->left);
        AstNode* right = OR_has_STAR_PLUS_QUESTION(seqNode->right);
        if (left) return left;
        if (right) return right;
    }
    return nullptr;
}

// IF OR HAS DOT OR CHARACTER CLASS
AstNode* OR_has_DOT_CHAR(AstNode* node) {
    // check if star or plus can be reached
    if (DotAstNode* dotNode = dynamic_cast<DotAstNode*>(node)) {
        return dotNode;
    }
    if (CharacterClassAstNode* charNode = dynamic_cast<CharacterClassAstNode*>(node)) {
        return charNode;
    }
    // check in subtree to find star or plus
    if (OrAstNode* orNode = dynamic_cast<OrAstNode*>(node)) {
        AstNode* left = OR_has_DOT_CHAR(orNode->left);
        AstNode* right = OR_has_DOT_CHAR(orNode->right);
        if (left) return left;
        if (right) return right;
    }
    if (SeqAstNode* seqNode = dynamic_cast<SeqAstNode*>(node)) {
        AstNode* left = OR_has_DOT_CHAR(seqNode->left);
        AstNode* right = OR_has_DOT_CHAR(seqNode->right);
        if (left) return left;
        if (right) return right;
    }
    if (StarAstNode* starNode = dynamic_cast<StarAstNode*>(node)) {
        return OR_has_DOT_CHAR(starNode->left);
    }

    if (PlusAstNode* plusNode = dynamic_cast<PlusAstNode*>(node)) {
        return OR_has_DOT_CHAR(plusNode->left);
    }
    return nullptr;
}

// IF STAR PLUS HAS DOT OR CHARACTER CLASS
AstNode* STAR_PLUS__QUESTION_has_DOT_CHAR(AstNode* node) {
    // check if star or plus can be reached
    if (DotAstNode* dotNode = dynamic_cast<DotAstNode*>(node)) {
        return dotNode;
    }
    if (CharacterClassAstNode* charNode = dynamic_cast<CharacterClassAstNode*>(node)) {
        return charNode;
    }
    // check in subtree to find star or plus
    if (OrAstNode* orNode = dynamic_cast<OrAstNode*>(node)) {
        AstNode* left = STAR_PLUS__QUESTION_has_DOT_CHAR(orNode->left);
        AstNode* right = STAR_PLUS__QUESTION_has_DOT_CHAR(orNode->right);
        if (left) return left;
        if (right) return right;
    }
    if (SeqAstNode* seqNode = dynamic_cast<SeqAstNode*>(node)) {
        AstNode* left = STAR_PLUS__QUESTION_has_DOT_CHAR(seqNode->left);
        AstNode* right = STAR_PLUS__QUESTION_has_DOT_CHAR(seqNode->right);
        if (left) return left;
        if (right) return right;
    }
    if (StarAstNode* starNode = dynamic_cast<StarAstNode*>(node)) {
        return STAR_PLUS__QUESTION_has_DOT_CHAR(starNode->left);
    }

    if (PlusAstNode* plusNode = dynamic_cast<PlusAstNode*>(node)) {
        return STAR_PLUS__QUESTION_has_DOT_CHAR(plusNode->left);
    }
    if (QuestionAstNode* questionNode = dynamic_cast<QuestionAstNode*>(node)) {
        return STAR_PLUS__QUESTION_has_DOT_CHAR(questionNode->left);
    }
    return nullptr;
}

AstNode* STAR_PLUS__QUESTION_has_OR(AstNode* node) {
    if (OrAstNode* orNode = dynamic_cast<OrAstNode*>(node)) {
        return orNode;
    }
    if (SeqAstNode* seqNode = dynamic_cast<SeqAstNode*>(node)) {
        AstNode* left = STAR_PLUS__QUESTION_has_OR(seqNode->left);
        AstNode* right = STAR_PLUS__QUESTION_has_OR(seqNode->right);
        if (left) return left;
        if (right) return right;
    }
    if (StarAstNode* starNode = dynamic_cast<StarAstNode*>(node)) {
        return STAR_PLUS__QUESTION_has_OR(starNode->left);
    }
    if (PlusAstNode* plusNode = dynamic_cast<PlusAstNode*>(node)) {
        return STAR_PLUS__QUESTION_has_OR(plusNode->left);
    }
    if (QuestionAstNode* questionNode = dynamic_cast<QuestionAstNode*>(node)) {
        return STAR_PLUS__QUESTION_has_OR(questionNode->left);
    }
    return nullptr;
}