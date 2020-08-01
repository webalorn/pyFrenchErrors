#ifndef PY_CODE_H
#define PY_CODE_H

#include <vector>
#include <string>
#include "utility.hpp"

class PyLine {
private:
    std::string::iterator firstChar, lastChar;
    std::string line;

public:
    PyLine(std::string); // do not strip, but set first and last non-empty char

    std::string getFirstWord();
    std::string get();
    std::vector<std::string> extractBlockIds();

    bool isStruct();
    bool isConditianal();
    bool endByTwoPoints();
    bool contain(std::string);
    int countOpenCloseBrackets();
};

#endif
