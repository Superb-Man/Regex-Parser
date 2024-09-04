#include"parseRegX.hpp"

class ParserHandler {
private:
    std::vector<std::pair<AstNode*,bool>> trees;
    std::vector<Token> tokens;
    std::string regex;

    void splitTree() {
        // if .* , .+ , .? , []*, []+, []?, (?!.......)
        int start = 0;
        int parenStart = 0;
        for (int i = 0; i < tokens.size(); i++) {
            if (tokens[i].index == OPEN_PAREN) {
                parenStart = i;
            }
            if ((i < tokens.size()-1 && tokens[i+1].index != OR) 
            &&  ((tokens[i].index == DOT && tokens[i+1].index == STAR)
            || (tokens[i].index == DOT && tokens[i+1].index == PLUS)
            || (tokens[i].index == DOT && tokens[i+1].index == QUESTION)
            || (tokens[i].index == CLOSED_BRACKET && tokens[i+1].index == STAR)
            || (tokens[i].index == CLOSED_BRACKET && tokens[i+1].index == PLUS)
            || (tokens[i].index == CLOSED_BRACKET && tokens[i+1].index == QUESTION))) {
                // parse upto i+1 tokens
                // create a new tree
                // add the tree to the trees vector
                // remove the tokens from the tokens vector
                // repeat the process
                ParseRegex parser(std::vector<Token>(tokens.begin()+start, tokens.begin()+i+2));
                try{
                    AstNode* tree = parser.parse();
                    addTree(tree);
                    i++;
                    start = i+1;
                    tree->print();
                } catch (const std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::cout << "Invalid regular expression" << std::endl;
                    exit(1);
                }
                
            }
            else if ((tokens[i].index == CLOSED_PAREN 
            && tokens[parenStart+1].index == QUESTION 
            && tokens[parenStart+2].index == EXCLAMATION)) {
                std::cout << "Negative lookahead" << std::endl;

                ParseRegex parser(std::vector<Token>(tokens.begin()+start, tokens.begin()+i+1));
                try{
                    AstNode* tree = parser.parse();
                    addTree(tree);
                    start = i+1;
                    tree->print();
                } catch (const std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::cout << "Invalid regular expression" << std::endl;
                    exit(1);
                }    

            }

        }
        if (start < tokens.size()) {
            try {
                ParseRegex parser(std::vector<Token>(tokens.begin()+start, tokens.end()));
                AstNode* tree = parser.parse();
                addTree(tree);
                tree->print();
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                std::cout << "Invalid regular expression" << std::endl;
                exit(1);
            }
        }
    }

    bool match (std::string str) {
        bool curMatch = true;
        int pos = 0;
        for (auto tree : trees) {
            bool isMatch = false;
            // check for all starting points
            if (tree.second) {
                for (int i = pos ; i < str.size(); i++) {
                    if (tree.first->match(str, pos) && pos == str.size()) {
                        isMatch = true;
                        break;
                    }
                }
            }
            else {
                isMatch = tree.first->match(str, pos) && pos == str.size();
                std::cout << pos << std::endl;
            }
            curMatch = curMatch && isMatch;

            if (!curMatch) {
                return false;
            }
        }
        return true;
    }

public:

    ParserHandler(std::string regex) {
        this->regex = regex;
        this->tokens = lexer(regex);
        splitTree();
    }
    void addTree(AstNode* tree) {
        bool siz = trees.size() > 0 ? true : false; 
        trees.push_back({tree,siz});
    }

    bool matchString(std::string str) {
        return match(str);
    }

    


    

    //maitianing state based design 
    // for ab*c.*
};