#include "pyLine.hpp"
#include <cctype>
#include <iterator>
#include <set>

PyLine::PyLine(std::string lineOfCode) {
    line = lineOfCode;
    firstChar = line.begin(), lastChar = line.end();
    while (firstChar != line.end() && isspace(*firstChar))
        firstChar++;
    while (std::prev(lastChar) != line.begin() && isspace(*prev(lastChar)))
        lastChar--;
}

std::string PyLine::getFirstWord() {
    std::string::iterator endWord = firstChar;
    while (endWord != line.end() && endWord != lastChar && isalnum(*endWord)) {
        endWord++;
    }
    return std::string(firstChar, endWord);
}
std::string PyLine::get() {
    return line;
}

bool PyLine::isStruct() {
    std::set<std::string> pyStructs = {"if", "for", "else", "elif", "while", "def", "class"};
    return pyStructs.find(getFirstWord()) != pyStructs.end();
}
bool PyLine::isConditianal() {
    std::set<std::string> pyStructs = {"if", "else", "elif", "while"};
    return pyStructs.find(getFirstWord()) != pyStructs.end();
}
bool PyLine::endByTwoPoints() {
    return (*prev(lastChar)) == ':';
}
bool PyLine::contain(std::string occ) {
    return line.find(occ) != std::string::npos;
}

int PyLine::countOpenCloseBrackets() {
    int difference = 0;
    for (char car : line) {
        if (car == '(')
            difference++;
        if (car == ')')
            difference--;
    }
    return difference;
}
