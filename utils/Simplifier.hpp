#pragma once 
#include "nodes.hpp"
// will simplify a rfegex to a regex that can be solved by the engine

struct Simplifier {
    std::string simplifiedRegex;

    Simplifier(AstNode* node) {
        simplifiedRegex = "";
        simplify(node);
        simplifyRepeated(simplifiedRegex);
    }

    bool isSP(char c) {
        return c == '*' || c == '+' ;
    }

    /**
     * @brief 
     * steps :
     *  1. traverse the tree , add the nodes in a string format inorder traversal
     *  2. for Seq node , no need to add that node string into regexSimplied
     *  3. for OR node, start bracket when ever or node is found
     *  4. and close bracket when the or node is finished
     *  5. for star node , if star has seq node as left child then also add bracket from seq node
     * @param node 
     * @param str 
     * @param pos 
     * @return std::string 
     */
    void simplify(AstNode* node) {
        if (node == nullptr) {
            return ;
        }

        if (SeqAstNode* seqNode = dynamic_cast<SeqAstNode*>(node)) {
            simplify(seqNode->left);
            simplify(seqNode->right);
        }

        if (OrAstNode* orNode = dynamic_cast<OrAstNode*>(node)) {
            simplifiedRegex += "(";
            simplify(orNode->left);
            simplifiedRegex += "|";
            simplify(orNode->right);
            simplifiedRegex += ")";
        }

        if (StarAstNode* starNode = dynamic_cast<StarAstNode*>(node)) {
            if (SeqAstNode* seqNode = dynamic_cast<SeqAstNode*>(starNode->left)) {
                simplifiedRegex += "(";
                simplify(seqNode->left);
                simplify(seqNode->right);
                simplifiedRegex += ")*";
            } 
            else {
                simplify(starNode->left);
                simplifiedRegex += "*";
            }
        }
        if (PlusAstNode* plusNode = dynamic_cast<PlusAstNode*>(node)) {
            if (SeqAstNode* seqNode = dynamic_cast<SeqAstNode*>(plusNode->left)) {
                simplifiedRegex += "(";
                simplify(seqNode->left);
                simplify(seqNode->right);
                simplifiedRegex += ")+";
            } 
            else {
                simplify(plusNode->left);
                simplifiedRegex += "+";
            }
        }

        if (DotAstNode* dotNode = dynamic_cast<DotAstNode*>(node)) {
            simplifiedRegex += ".";
        }

        if (CharacterClassAstNode* charNode = dynamic_cast<CharacterClassAstNode*>(node)) {
            simplifiedRegex += '[';
            char prevChar = '\0';
            int prevIdx = 0;
            std::string temp = "";
            
            for (auto litNode : charNode->charClass) {
                char currentChar = litNode.ch;
                temp += currentChar;
                
                if (prevChar != '\0' && prevChar + 1 == currentChar) {
                    prevChar = currentChar;
                    prevIdx++;
                } 
                else {
                    if (prevIdx > 0) {
                        simplifiedRegex += temp[0];
                        simplifiedRegex += '-';
                        simplifiedRegex += temp[prevIdx];
                    } 
                    else if (!temp.empty()) {
                        simplifiedRegex += temp[0];
                    }
                    temp = currentChar;
                    prevChar = currentChar;
                    prevIdx = 0;
                }
            }
            if (prevIdx > 0) {
                simplifiedRegex += temp[0];
                simplifiedRegex += '-';
                simplifiedRegex += temp[prevIdx];
            } 
            else if (!temp.empty()) {
                simplifiedRegex += temp;
            }
            
            simplifiedRegex += ']';

        }

        if (LiteralCharacterAstNode* litNode = dynamic_cast<LiteralCharacterAstNode*>(node)) {
            simplifiedRegex += litNode->ch;
        }

        return ;

    }

    std::string simplifyRepeated(const std::string regex) {
        simplifiedRegex="";
        int n = regex.size() ;
        char lastChar = regex[0];  
        int repeatCount = 1;
        int found = 0;

        // dound = 0 // no star,plus
        // found = 1 // star
        // found = 2 // plus
        // will give precedence to plus

        for (int i = 0 ; i < n ; i ++) {
            if (i == 0) {
                if (i + 1 < n && isSP(regex[i + 1])) {
                    found = std::max(found, regex[i + 1] == '*' ? 1 : 2);
                    i++;
                    repeatCount--;
                }
            }
            else if (regex[i] == lastChar) {
                // std::cout << "Matched at position " << i << std::endl;
                lastChar = regex[i];
                repeatCount++;
                if (i + 1 < n && isSP(regex[i + 1])) {
                    repeatCount--;
                    found = std::max(found, regex[i + 1] == '*' ? 1 : 2);
                    i++;
                }
            }
            else {
                if (found) repeatCount++;
                simplifiedRegex += std::string(repeatCount, lastChar);
                simplifiedRegex += (found == 1 ? "*" : (found == 2 ? "+" : ""));

                if (i + 1 < n && isSP(regex[i + 1])) {
                    found = std::max(found, regex[i + 1] == '*' ? 1 : 2);
                    lastChar = regex[i];
                    repeatCount = 0;
                    i++;
                }
                else {
                    found = 0;
                    repeatCount = 1;
                    lastChar = regex[i];
                }
            }
            // std::cout << "Simplified Regex: " << simplifiedRegex << std::endl;
            // std::cout << "Repeat count: " << repeatCount << " index " << i << std::endl;

        }
        // extra case for last char
        if (found) repeatCount++;
        simplifiedRegex += std::string(repeatCount, lastChar);
        simplifiedRegex += (found == 1 ? "*" : (found == 2 ? "+" : ""));


        return simplifiedRegex;
    }
};
