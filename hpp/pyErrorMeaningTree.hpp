#ifndef PY_ERROR_MEANING_TREE
#define PY_ERROR_MEANING_TREE

#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <regex>
#include "json.hpp"
#include "pyError.hpp"
#include "pyFile.hpp"
#include "usefulStructs.hpp"

class PyErrorMeaningTree {
private:
    nlohmann::json tree;
    errorDescription getMeaningDfs(PyError&, PyFile&, nlohmann::json&);
public:
    PyErrorMeaningTree(std::ifstream);
    //            vector de paires: message, paramètres                    +realLineNumber
    errorDescription getMeaningMessages(PyError&, PyFile&);
};

#endif
