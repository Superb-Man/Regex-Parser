#include "Simplifier.hpp"
#include "parseRegX.hpp"
#include "NI_log.hpp"

AstNode* generateTree(std::string regex) {
    std::vector<Token> tokens = lexer(regex);
    ParseRegex parser(tokens);

    try {
        AstNode* ast = parser.parse();
        // ast->print();
        bool flag = canSolve(parser.getAstNodes());
        if (!flag) { 
            std::cout << "NOT YET IMPLEMENTED\nWon't match anything" << std::endl;
            return nullptr;
        }

        return ast;
    } catch (const std::exception& e) {
        e.what();
        return nullptr;
    }
}



struct Splitter {
    std::string regex;
    std::vector<std::pair<AstNode*, std::string>> roots;

    bool checkNode(AstNode* node, std::string text, int checkerStart, int checkerEnd) {
        std::string temp = text.substr(checkerStart, checkerEnd - checkerStart + 1);
        std::cout << "temp: " << temp << std::endl;
        checkerStart = 0; 
        if (!node->matchL(temp, checkerStart) || checkerStart != temp.size()) return false;

        return true;

    }

    // .* handle
    
    Splitter(std::string regex) {
        AstNode* ast = generateTree(regex);
        if (!ast) {
            this->regex = "";
        }
        else {
            this->regex = Simplifier(ast).simplifiedRegex;
            std::cout << "Simplified regex: " << this->regex << std::endl;
            split();
        }
    }

    bool insertPrevRegX(std::string temp,std::vector<std::pair<AstNode*, std::string>> &roots) {
        if (temp.size()) {
            AstNode* ast = generateTree(temp);
            if (ast) {
                roots.push_back({ast, temp});
            }
            else {
                roots.clear();
                return false;
            }
        }
        return true;
    }

    // only handling the .* for now
    /**
     * @brief split the regex to multiple regexes
     */
    void split() {
        std::string temp = "";
        int startingBracket = -1;
        for (int i = 0 ; i < regex.size(); i++) {
            //for [....]* or [....]+
            if (regex[i] == '[') {
                startingBracket = i;
                while (i < regex.size() && regex[i] != ']') {
                    i++;
                }
                if (i + 1 < regex.size() && (regex[i+1] == '*' || regex[i+1] == '+')) {
                    if (!insertPrevRegX(temp, roots)) return;
                    roots.push_back({generateTree(regex.substr(startingBracket, i +1 - startingBracket + 1)), regex.substr(startingBracket, i+1 - startingBracket + 1)});
                    i++;
                    temp = "";
                }
                else {
                    temp += regex.substr(startingBracket, i - startingBracket + 1);
                }
            }
            //for .*, .+
            else if (i +1 < regex.size() && regex[i] == '.' && (regex[i+1] == '+' || regex[i+1] == '*')) {
                if (!insertPrevRegX(temp, roots)) return;
                roots.push_back({generateTree(regex.substr(i, 2)), regex.substr(i, 2)});
                i++;
                temp = "";
            }
            else {
                temp += regex[i];
            }
        }
        insertPrevRegX(temp, roots);

        // std::cout << "Roots size: " << roots.size() << std::endl;
    }
    bool match(std::string text) {
        if (roots.size() == 0) {
            return false;
        }

        int pos = 0;
        int visited = 0;
        int checkerStart = -1;
        int checkerEnd = -1;

        auto checkSubstringMatchTwice = [&](int i, int endPos) {
            bool t = false;
            for (int j = std::max(checkerStart-1,0); j <= checkerEnd; j++) {
                if (roots[i-1].first->matchL(text, j) && checkNode(roots[i].first, text, j, checkerEnd)) {
                    checkerStart = -1;
                    checkerEnd = -1;
                    t = true;
                    break;
                }
            }
            return t;
        };

        auto checkSubstringMatch = [&](int i, int endPos) {
            bool t = false;
            for (int j = std::max(checkerStart-1,0); j <= checkerEnd; j++) {
                if (checkNode(roots[i].first, text, j, checkerEnd)) {
                    checkerStart = -1;
                    checkerEnd = -1;
                    t = true;
                    break;
                }
            }
            return t;
        };

        auto handleLastNode = [&](int i, bool t) {
            if (!t) return false;
            if (checkerStart != -1 && checkerEnd != -1) {
                return checkNode(roots[i-1].first, text, checkerStart, checkerEnd);
            }
            return true;
        };

        for (int i = 0; i < roots.size(); i++) {
            visited++;
            std::cout << "root index: " << i << " ";
            std::cout << "root: " << roots[i].second << std::endl;

            if (roots[i].second == ".*") continue;
            if (roots[i].second == ".+") {
                pos = std::min(pos+1,(int)text.size());
                continue;
            }
            
            if (roots[i].second.back() == '*' && roots[i].second[roots[i].second.size() - 2] == ']') {
                checkerStart = pos;
                continue;
            }
            if (roots[i].second.back() == '+' && roots[i].second[roots[i].second.size() - 2] == ']') {
                checkerStart = pos;
                if (i + 1 < roots.size() && (roots[i+1].second == ".*" || roots[i+1].second == ".+")) {
                    PlusAstNode* node = dynamic_cast<PlusAstNode*>(roots[i].first);
                    CharacterClassAstNode* newNode = dynamic_cast<CharacterClassAstNode*>(node->left);
                    int t = pos;
                    if (newNode->matchL(text, t)) pos++;
                    else return false;
                } 
                else {
                    pos = std::min(pos+1, (int)text.size());
                }
                continue;
            }

            if (i == 0) {
                if (!roots[i].first->matchL(text, pos)) return false;
                continue;
            }

            if (i == roots.size() - 1 && roots.size() > 1) {
                bool changed = false;
                std::cout << "pos: " << pos << std::endl;
                for (int j = pos; j < text.size(); j++) {
                    int t = j - 1;
                    if (roots[i].first->matchL(text, j) && j == text.size()) {
                        std::cout << "Matched at pos: " << j << std::endl;
                        pos = t;
                        changed = true;
                        break;
                    }
                }
                if (!changed) return false;

                // for []+ and []*;

                if (i-1 >= 0 && (roots[i-1].second.back() == '*' || roots[i-1].second.back() == '+') && roots[i-1].second[roots[i-1].second.size() - 2] == ']') {
                    checkerEnd = pos;
                    if (i-3 >= 0 && (roots[i-3].second == ".*" || roots[i-3].second == ".+")) {
                        if (!checkSubstringMatchTwice(i-1, checkerEnd)) return false;
                        continue;
                    }
                    if (i-2 >= 0 && (roots[i-2].second == ".*" || roots[i-2].second == ".+")) {
                        if (!checkSubstringMatch(i-1, checkerEnd)) return false;
                        continue;
                    }
                    if (!checkNode(roots[i-1].first, text, checkerStart, checkerEnd)) return false;
                    checkerStart = -1;
                    checkerEnd = -1;
                    continue;
                }
                continue;
            }

            bool t = false;
            for (int j = pos; j < text.size(); j++) {
                checkerEnd = j - 1;
                t = roots[i].first->matchL(text, j);
                if (t) {
                    std::cout << "Matched at pos: " << checkerEnd << std::endl;
                    pos = j;
                    break;
                }
            }
            if (!t) return false;
            
            // for []+ and []*;
            // make substring from checkerStart to pos if checkerStart != -1
            if (checkerStart != -1 && checkerEnd != -1) {
                if (i-3 >= 0 && (roots[i-3].second == ".*" || roots[i-3].second == ".+")) {
                    if (!checkSubstringMatchTwice(i-1, checkerEnd)) return false;
                    continue;
                }
                if (i-2 >= 0 && (roots[i-2].second == ".*" || roots[i-2].second == ".+")) {
                    if (!checkSubstringMatch(i-1, checkerEnd)) return false;
                    continue;
                }
                if (!checkNode(roots[i-1].first, text, checkerStart, checkerEnd)) return false;
                checkerStart = -1;
                checkerEnd = -1;
            }
        }

        if (visited != roots.size()) return false;

        if ((roots.back().second.back() == '*' || roots.back().second.back() == '+') && roots.back().second[roots.back().second.size() - 2] == ']') {
            if (text.empty() && roots.back().second.back() == '+') return false;
            checkerEnd = text.size() - 1;
            if (roots.size() == 1) return roots.back().first->matchL(text, pos);
            if (roots.size() - 2 >= 0 && (roots[roots.size()-2].second == ".*" || roots[roots.size()-2].second == ".+")) {
                return checkSubstringMatch(roots.size()-1, checkerEnd);
            }
            if (roots.size() - 3 >= 0 && (roots[roots.size()-3].second == ".*" || roots[roots.size()-3].second == ".+")) {
                return checkSubstringMatchTwice(roots.size()-1, checkerEnd);
            }
            return checkNode(roots.back().first, text, checkerStart, checkerEnd);
        }

        if (roots.size() == 1) {
            if (pos == text.size()) return true;
            if (roots[0].second == ".*") return true;
            if (roots[0].second == ".+" && text.size() > 1) return true;
            return false;
        }

        return true;
    }


    std::pair<std::string,bool> matchedString(std::string text) {
        // std::cout << "Roots size :" << roots.size() << std::endl;
        if (roots.size() == 0) {
            return {"", false};
        }
        if (match(text)) {
            return {text, true};
        }
        // check for all strings from 0 to i
        for (int i = text.size() - 2; i >= 0; i--) {
            if (match(text.substr(0, i+1))) {

                return {text.substr(0, i+1), true};
            }
        }

        return {"", false};
    }


};
