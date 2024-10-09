#include "utils/splitter.hpp"

int main() {
    std::vector<std::string> regexPatterns = {
        // "a.?b",
        // "a+b*(c+de)*f",
        // "a*b+c?(d+)?",
        // "a[a-zA-Z0-9.]",
        // "a*b+c?d*?xyz+",
        // "a(.6)*d",
        // "a[0-9]62",
        // "acd*b",
        //cd "a?b",
        // "a[a-z]b",
        // "a(.bc|c*)+",
        // "a([a-z].)*",
        // "a(.s|a)*62",
        // "[a-z]*b",
        // "a(.sb)*bc*",
        // "cd[a-z]|a(.bc)*b*",
        // "ad|(b|.c)*",
        // "a.?b",
        // "a(a|bc)*(bd|b)",
        // "aaab",
        // "b|bd", /////////////////////////////DON'T WORK
        // "a*(b|(cd+|e*f))bd",
        // "([0-9]|a*(b|(cd+|e*f)))bd",
        // "a.*b",
        // "a((.)*)bcd+(((c)d)e)",
        // "a.*b[0-9]+b+b*",
        // "a*a+",
        // "a.*b*cd+.*f",
        // "a(.bc)*d",
        // "a*b*.*g*.+",
        // "abcd+.*f", //////////////////TESTED UPTO IT
        // "ab*b?c+.*f",
        // "a?ab",
        // "a*",
        // "a*[dzf]*dfb*",
        // "[a-c]+z",
        // "a+",
        // ".*a+[s-v0-5]*bcd+e(a|b)*",  ///////////WORKING
        // ".*a+.*bcd+e(a|b)*", //////////WORKING
        // "[a-c]+a[a-c]+",
        // ".*a[a-c]+",
        // "abc[a-d]?.*abc",
        // "(ab*)*|b(c|d+)",
        // "a*|c*",
        // "(a|b)b",
        // "a.*[a-c]+d",
        // "a[a-c]+.*d",
        // "[a-c]+a*a+a",
        // ".+ba",
        //"a(c|b)*abc"
    };

    for (std::string regex : regexPatterns) {
        Splitter splitter(regex);

        while (true) {
            std::cout << "Regex: " << regex << std::endl;
            std::string text;
            std::cout << "Enter a text: ";
            std::getline(std::cin, text);
            if (text == "exit") {
                break;
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
}