#ifndef PY_CODE_H
#define PY_CODE_H

#include <string>

class PyCode {
private:
    std::string::iterator firstChar, lastChar;
    std::string line;

public:
    PyCode(std::string); // do not strip, but set first and last non-empty char

    std::string getFirstWord();
    std::string getLine();

    bool isStruct();
    bool isConditianal();
    bool endByTwoPoints();
    int countOpenCloseBrackets();
};

#endif
