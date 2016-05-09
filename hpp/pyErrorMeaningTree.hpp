#ifndef PY_ERROR_MEANING_TREE
#define PY_ERROR_MEANING_TREE

#include <ifstream>
#include <vector>
#include <string>
#include <utility>
#include <regex>
#include "json.hpp"
#include "PyError.hpp"
#include "PyFile.hpp"

class PyErrorMeaningTree {
private:
    nlohmann::json tree;
    int realErrorLineNumber;
public:
    PyErrorMeaningTree(std::ifstream::ifstream);
    vector<std::pair<std::string, std::vector<std::string>>> getMeaningMessages(PyError, PyFile);
    int getRealErrorLineNumber();
};

#endif
