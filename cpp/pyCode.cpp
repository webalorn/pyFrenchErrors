#include "pyCode.hpp"
#include <cctype>
#include <iterator>
#include <set>

PyCode::PyCode(std::string lineOfCode) {
    line = lineOfCode;
    firstChar = line.begin(), lastChar = line.end();
    while (firstChar != line.end() && isspace(*firstChar))
        firstChar++;
    while (std::prev(lastChar) != line.begin() && isspace(*prev(lastChar)))
        lastChar--;
}

std::string PyCode::getFirstWord() {
    std::string::iterator endWord = firstChar;
    while (endWord != line.end() && endWord != lastChar && isalnum(*endWord)) {
        endWord++;
    }
    return std::string(firstChar, endWord);
}
std::string PyCode::getLine() {
    return line;
}

bool PyCode::isStruct() {
    std::set<std::string> pyStructs = {"if", "for", "else", "elif", "while", "def", "class"};
    return pyStructs.find(getFirstWord()) != pyStructs.end();
}
bool PyCode::isConditianal() {
    std::set<std::string> pyStructs = {"if", "else", "elif", "while"};
    return pyStructs.find(getFirstWord()) != pyStructs.end();
}
bool PyCode::endByTwoPoints() {
    return (*prev(lastChar)) == ':';
}

int PyCode::countOpenCloseBrackets() {
    int difference = 0;
    for (char car : line) {
        if (car == '(')
            difference++;
        if (car == ')')
            difference--;
    }
    return difference;
}
