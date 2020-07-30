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
    return isIn<std::string>(getFirstWord(), {"if", "for", "else", "elif", "while", "def", "class"});
}
bool PyLine::isConditianal() {
    return isIn<std::string>(getFirstWord(), {"if", "else", "elif", "while"});
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
