#ifndef PY_FILE_H
#define PY_FILE_H

#include <vector>
#include <string>

class PyFile {
public:
    PyFile(std::vector<std::string>);
    int getNbOpenBrackets(int);
    int getLineOpenFirstBracket(int);
private:
    std::vector<std::string> linesCode;
    std::vector<int> nbOpenBracketsLine;
};

#endif
