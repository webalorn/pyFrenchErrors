#include "pyLine.hpp"
#include "utility.hpp"
#include <cctype>
#include <iterator>
#include <set>
#include <regex>

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

std::vector<std::string> PyLine::extractBlockIds() {
    std::regex reg(".*#BlockIds=(.*)");
    std::smatch match;

    if (std::regex_match(line, reg) && std::regex_search(line, match, reg)) {
        std::vector<std::string> elems = {};
        split(match[1], '\'', elems);
        return elems;
    }
    return {};
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
