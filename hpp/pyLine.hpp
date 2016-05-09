#ifndef PY_CODE_H
#define PY_CODE_H

#include <string>

class PyLine {
private:
    std::string::iterator firstChar, lastChar;
    std::string line;

public:
    PyLine(std::string); // do not strip, but set first and last non-empty char
    operator std::string() { return get(); }

    std::string getFirstWord();
    std::string get();

    bool isStruct();
    bool isConditianal();
    bool endByTwoPoints();
    bool contain(std::string);
    int countOpenCloseBrackets();
};

#endif
