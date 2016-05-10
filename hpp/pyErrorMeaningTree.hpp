#ifndef PY_ERROR_MEANING_TREE
#define PY_ERROR_MEANING_TREE

#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <regex>
#include <functional>
#include <map>
#include "json.hpp"

#include "pyError.hpp"
#include "pyFile.hpp"
#include "usefulStructs.hpp"

class PyErrorMeaningTree {
private:
    nlohmann::json tree;
    std::map<std::string, std::function<bool(FctContext)>> boolFcts;
    //std::map<std::string, std::function<std::string(FctContext)>> getFcts;

    errorDescription getMeaningDfs(PyError&, PyFile&, nlohmann::json&);
    errorDescription dfsConditionNode(PyError&, PyFile&, nlohmann::json&);
    errorDescription dfsRegexNode(PyError&, PyFile&, nlohmann::json&, std::string regexApplyTo);
    errorDescription dfsReturnTypeNode(PyError&, PyFile&, nlohmann::json&);

    bool useBoolFct(std::string, FctContext);
    //std::string getReturnFct(std::string, FctContext);
public:
    PyErrorMeaningTree(std::ifstream);
    //            vector de paires: message, paramètres                    +realLineNumber
    errorDescription getMeaningMessages(PyError&, PyFile&);
};

#endif
