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

class PyErrorMeaningTree {
private:
    nlohmann::json tree;
public:
    PyErrorMeaningTree(std::ifstream);
    //            vector de paires: message, paramètres                    +realLineNumber
    std::pair<std::vector<std::pair<std::string, std::vector<std::string>>>, int> getMeaningMessages(PyError, PyFile);
};

#endif
