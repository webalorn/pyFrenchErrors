#ifndef PY_FILE_H
#define PY_FILE_H

#include <vector>
#include <string>
#include "pyLine.hpp"

class PyFile {
public:
    PyFile(std::vector<std::string>);
    int getNbOpenBrackets(int);
    int getLineOpenFirstBracket(int);
    PyLine getLine(int);
private:
    std::vector<PyLine> linesCode;
    std::vector<int> nbOpenBracketsLine;
};

#endif
