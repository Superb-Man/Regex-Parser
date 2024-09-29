#include "utils/splitter.hpp"

int main() {
    std::string regex; 
    std::string text;
    // regex = "a+b*(c+de)*f";
    // regex = "a*b+c?(d+)?";
    // regex = "a[a-zA-Z0-9.]";
    // regex = "a*b+c?d*?xyz+";
    // regex = "a(?!lara)62";
    // regex = "a(?![a-z])62";
    regex = "a(.6)*d";
    // regex = "a[0-9]62";
    // regex = "acd*b";
    // regex = "a.?b";
    // regex = "a[a-z]b";
    // regex = "a(.bc|c*)+";
    // regex = "a([a-z].)*";
    regex = "a(.s|a)*62";
    // regex = "[a-z]*b";
    regex = "a(.sb)*bc*";
    // regex = "cd[a-z]|a(.bc)*b*";
    // regex = "ad|(b|.c)*";
    // regex = "a.?b";
    regex = "a(a|bc)*(bc|b)";
    // regex = "aaab";
    regex = "a(ab|cd?e)*abc";
    // regex = "([0-9a-d.]|a*(b|(cd+|e*f)))bd";
    // regex = "a.*b";
    // regex = "a((.)*)bcd+(((c)d)e)";
    // regex = "a.*b[0-9]+b+b*";
    // regex = "a*a+";
    regex = "a.*b*cd+.*f";
    // regex = "a(.bc)*d";
    regex = "a*b*.*g*.+";
    regex = "abcd+.*f";
    regex = "ab*b?c+.*f";
    regex = "a?ab";
    regex = "a*";
    regex = "a*[dzf]*dfb*";
    regex = "[a-c]+";
    // regex = "a+";
    std::cout << regex << std::endl;

    Splitter splitter(regex);

    splitter.split();

    while (true) {
        std::cout << "Enter a text: ";
        std::getline(std::cin, text);
        if (text == "exit") {
            return 0;
        }
        std::pair<std::string, bool> result = splitter.matchedString(text);

        if (result.second) {
            std::cout << "Matched up to pos " << result.first.size() << std::endl;
            std::cout << "Matched string: " << result.first << std::endl;
        } else {
            std::cout << "Not matched" << std::endl;
        }
    }
}