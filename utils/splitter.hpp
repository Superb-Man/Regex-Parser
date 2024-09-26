#include "Simplifier.hpp"
#include "parseRegX.hpp"
#include "NI_log.hpp"

AstNode* generateTree(std::string regex) {
    std::vector<Token> tokens = lexer(regex);
    ParseRegex parser(tokens);

    try {
        return parser.parse();
    } catch (const std::exception& e) {
        e.what();
        return nullptr;
    }
}

struct Splitter {
    std::string regex;
    std::vector<std::pair<AstNode*, std::string>> roots;

    // .* handle
    
    Splitter(std::string regex) {
        this->regex = Simplifier(generateTree(regex)).simplifiedRegex;
        std::cout << "Simplified regex: " << this->regex << std::endl;
    }

    // only handling the .* for now
    /**
     * @brief split the regex to multiple regexes
     */
    void split() {
        std::string temp = "";
        for (int i = 0 ; i < regex.size(); i++) {
            //for .*
            if (i +1 < regex.size() && regex[i] == '.' && regex[i+1] == '*') {
                if (temp.size())    roots.push_back({generateTree(temp), temp});
                roots.push_back({generateTree(".*"), ".*"});
                i++;
                temp = "";
            }
            //for .+
            else if (i +1 < regex.size() && regex[i] == '.' && regex[i+1] == '+') {
                if (temp.size()) roots.push_back({generateTree(temp), temp});
                roots.push_back({generateTree(".+"), ".+"});
                i++;
                temp = "";
            }
            else {
                temp += regex[i];
            }
        }
        if (temp.size()) {
            roots.push_back({generateTree(temp), temp});
        }

        std::cout << "Roots size: " << roots.size() << std::endl;
    }

    bool match(std::string text) {
        std::cout << "text size: " << text.size() << std::endl;

        int pos = 0;
        int visited = 0;

        for (int i = 0; i < roots.size() ; i++) {
            visited++;
            std::cout << "root index: " << i << " ";
            std::cout << "root: " << roots[i].second << std::endl;
            if (roots[i].second == ".*") continue;
            if (roots[i].second == ".+") {
                pos++;
                continue;
            }

            if (i == 0) {
                if (!roots[i].first->matchL(text, pos)) return false;
                pos = 0;
                continue;
            }

            if (i == roots.size() - 1 && roots.size() > 1) {
                pos = text.size() - 1; 
                if (!roots[i].first->matchR(text, pos)) return false;
                // std::cout << "pos: " << pos << std::endl;
                continue;
            }

            bool t = false;
            std::cout << pos << std::endl;
            for (int j = pos; j < text.size(); j++) {
                t = roots[i].first->matchL(text, j);
                if (t) {
                    // std::cout << "pos: " << j << std::endl;
                    pos = j;
                    break;
                }
            }

            if (!t) return false;
            std::cout << "total visited: " << visited << std::endl;
            std::cout << "pos: " << pos << std::endl;

        }
        // std::cout << "pos: " << pos << std::endl;
        if (visited != roots.size()) return false;
        if (roots.size() == 1) {
            if (pos == text.size()) return true;
            if (roots[0].second == ".*") return true;
            if (roots[0].second == ".+" && text.size() > 1) return true;
            return false ;
        }

        return true;


    }


};

