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

        std::cout << "Roots size: " << roots.size() << std::endl;
    }

    bool match(std::string text) {
        // std::cout << "text size: " << text.size() << std::endl;
        if (roots.size() == 0) {
            return false;
        }

        int pos = 0;
        int visited = 0;
        int checkerStart = -1;
        int checkerEnd = -1;

        for (int i = 0; i < roots.size() ; i++) {
            // roots[i].first->print();
            visited++;
            std::cout << "root index: " << i << " ";
            std::cout << "root: " << roots[i].second << std::endl;
            if (roots[i].second == ".*") continue;
            if (roots[i].second == ".+") {
                pos = std::min(pos+1,(int)text.size());
                continue;
            }
            // if it contains [....]*
            if (roots[i].second[roots[i].second.size() - 1] == '*' && roots[i].second[roots[i].second.size() - 2] == ']') {
                checkerStart = pos;
                // std::cout << "checkerStart: " << checkerStart << std::endl;
                continue;
            }
            if (roots[i].second[roots[i].second.size() - 1] == '+' && roots[i].second[roots[i].second.size() - 2] == ']') {
                checkerStart = pos;
                // if next two is .* or .+ then give a check for the next character
                if (i + 1 < roots.size() && (roots[i+1].second == ".*" || roots[i+1].second == ".+")) {
                    // match any character in the [....] node
                    // make astNode [] from []+
                    // get the left node of the star node with casting
                    PlusAstNode* node = dynamic_cast<PlusAstNode*>(roots[i].first);
                    CharacterClassAstNode* newNode = dynamic_cast<CharacterClassAstNode*>(node->left);

                    if (newNode->matchL(text, pos)) pos++;
                    else return false;
                }
                else {
                    pos = std::min(pos+1,(int)text.size());
                }
                std::cout << "checkerStart: " << checkerStart << std::endl;
                continue;
            }

            if (i == 0) {
                if (!roots[i].first->matchL(text, pos)) return false;
                // pos = 0;
                continue;
            }

            if (i == roots.size() - 1 && roots.size() > 1) {
                std::cout << "Last node" << std::endl;
                // pos = text.size() - 1; 
                // if (!roots[i].first->matchR(text, pos)) return false;
                int prev = pos;
                // std::cout << "prev is " << prev << " pos is " << pos << std::endl;
                for (int j = pos; j < text.size(); j++) {
                    int t = j-1;
                    if (roots[i].first->matchL(text, j) && j == text.size()) {
                        pos = t;
                        // std::cout << "Pos is " << pos << std::endl;
                        break;
                    }
                }
                std::cout << "prev is " << prev << " pos is " << pos << std::endl;

                if (prev == pos) return false;



                // for []+ and []*;
                if (i-1 >= 0 && (roots[i-1].second[roots[i-1].second.size() - 1] == '*' || roots[i-1].second[roots[i-1].second.size() - 1] == '+') && roots[i-1].second[roots[i-1].second.size() - 2] == ']') {
                    checkerEnd = pos ;
                    std::cout << "checkerStart: " << checkerStart << " checkerEnd: " << checkerEnd << std::endl;
                    if (checkerStart != -1 && checkerEnd != -1) {
                        if (!checkNode(roots[i-1].first, text, checkerStart, checkerEnd)) {
                            return false;
                        }
                        checkerStart = -1;
                        checkerEnd = -1;
                    }
                }

                continue;
            }

            bool t = false;
            // std::cout << pos << std::endl;
            for (int j = pos; j < text.size(); j++) {
                checkerEnd = j - 1 ;
                t = roots[i].first->matchL(text, j);
                if (t) {
                    // std::cout << "pos: " << j << std::endl;
                    pos = j;
                    break;
                }
            }

            if (!t) return false;

            // for []+ and []*;
            // make substring from checkerStart to pos if checkerStart != -1
            // std::cout << "checkerStart: " << checkerStart << " checkerEnd: " << checkerEnd << std::endl;
            if (checkerStart != -1 && checkerEnd != -1) {
                if (i - 1 >= 0 && checkNode(roots[i-1].first, text, checkerStart, checkerEnd)) {
                    return false;
                }
                checkerStart = -1;
                checkerEnd = -1;
            }

            // std::cout << "total visited: " << visited << std::endl;
            // std::cout << "pos: " << pos << std::endl;

        }
        // std::cout << "pos: " << pos << std::endl;
        if (visited != roots.size()) return false;
        if (roots.size() == 1) {
            if (pos == text.size()) return true;
            if (roots[0].second == ".*") return true;
            if (roots[0].second == ".+" && text.size() > 1) return true;
            if (roots[0].second[roots[0].second.size() - 1] == '*' && roots[0].second[roots[0].second.size() - 2] == ']') {
                return roots[0].first->matchL(text, pos);
            }
            if (roots[0].second[roots[0].second.size() - 1] == '+' && roots[0].second[roots[0].second.size() - 2] == ']' && text.size() > 1) {
                return roots[0].first->matchL(text, pos);
            }
            return false ;
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

