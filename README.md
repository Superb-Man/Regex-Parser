# Regex-Parser
# Purpose
To understand about parser/syntax tree. For learning. 

```
The AstNode is not necessarily needed for validating regex only.
Not all possible operations implemented

Drawback - so many sequential nodes!

Future hope - Will try to implement the RegexMatcher later
```
# Description
Extended work of Automaton- solver.Implemented a very basic regular expression parser. Although it doesn't support all the operations possible.Was motivated by regex matcher at linux system.Implementing full library by own is quite a complex task.Yet i gave a try and still i am learning. The backtracking is pretty hard. Will try with automata theory later.
# Limitations
```
- Limitaions with multiple OR operations!
- Doesn't support all. Only a mini subset.
```
# The subset of regex it worked!
        "a.?b",
        "a+b*(c+de)*f",
        "a*b+c?(d+)?",
        "a[a-zA-Z0-9.]",
        "a*b+c?d*?xyz+",
        "a(.6)*d",
        "a[0-9]62",
        "acd*b",
        cd "a?b",
        "a[a-z]b",
        "a(.bc|c*)+",
        "a([a-z].)*",
        "a(.s|a)*62",
        "[a-z]*b",
        "a(.sb)*bc*",
        "cd[a-z]|a(.bc)*b*",
        "ad|(b|.c)*",
        "a.?b",
        "a(a|bc)*(bd|b)",
        "aaab",
        "b|bd", /////////////////////////////DON'T WORK
        "a*(b|(cd+|e*f))bd",
        "([0-9]|a*(b|(cd+|e*f)))bd",
        "a.*b",
        "a((.)*)bcd+(((c)d)e)",
        "a.*b[0-9]+b+b*",
        "a*a+",
        "a.*b*cd+.*f",
        "a(.bc)*d",
        "a*b*.*g*.+",
        "abcd+.*f",
        "ab*b?c+.*f",
        "a?ab",
        "a*",
        "a*[dzf]*dfb*",
        "[a-c]+z",
        "a+",
        ".*a+[s-v0-5]*bcd+e(a|b)*",  ///////////WORKING
        ".*a+.*bcd+e(a|b)*", //////////WORKING
        "[a-c]+a[a-c]+",
        ".*a[a-c]+",
        "abc[a-d]?.*abc",
        "(ab*)*|b(c|d+)",
        "a*|c*",
        "(a|b)b",
        "a.*[a-c]+d",
        "a[a-c]+.*d",
        "[a-c]+a*a+a",
        ".+ba",
        "a(c|b)*abc"
