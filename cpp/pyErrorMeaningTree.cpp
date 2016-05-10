#include "pyErrorMeaningTree.hpp"
#include <iostream>

PyErrorMeaningTree::PyErrorMeaningTree (std::ifstream dataFile) {
    dataFile >> tree;
    /*for (nlohmann::json::iterator it = tree.begin(); it != tree.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << std::endl;
    }*/
    // TODO: création des fonctions
}

errorDescription PyErrorMeaningTree::getMeaningMessages(PyError& pyError, PyFile& pyCodeFile) {
    errorDescription err = getMeaningDfs(pyError, pyCodeFile, tree);
    // TODO: clean the structure
    return err;
}

errorDescription PyErrorMeaningTree::getMeaningDfs(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    errorDescription errMessages = {pyError.getLineNumber(), {}};

    if (node.is_object()) {

        // traitement selon le type
        if (node.find("condition") != node.end())
            return dfsConditionNode(pyError, pyCodeFile, node); // TODO new function

        if (node.find("type") != node.end()) {
            std::string typeOfNode = node["type"];

            if (typeOfNode == "errorMessage")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getMessage()); // TODO new function
            if (typeOfNode == "errorType")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getType()); // TODO (cf above)
            if (typeOfNode == "errorCodeLine")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getPyLine().get()); // TODO (cf above)
        }
        if (node.find("typeError")) {
            return dfsReturnTypeNode(pyError, pyCodeFile, node); // TODO new function
        }

    } else if (node.is_array()) {
        for (json::iterator it = node.begin(); it != node.end() && !errMessages.isDefined(); ++it) {
            errMessages = getMeaningDfs(pyError, pyCodeFile, *it);
        }
    } else if (node.is_string()) {
        errMessages.messages.push_back(std::string(node), {});
    }
    return errMessages;
}

bool useBoolFct(std::string name, FctContext context) {
    if (boolFcts.find(name) == boolFcts.end())
        return false;
    return boolFcts[name](context);
}
/*std::string getReturnFct(std::string name, FctContext context) {
    if (getFcts.find(name) == getFcts.end())
        return "";
    return getFcts[name](context);
}*/

errorDescription dfsConditionNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    FctContext context = {{}, pyError, pyCodeFile};
    if (node.find("params") != node.end())
        context.params = node["params"];
    if (node.find("block") != node.end() && useBoolFct(node["condition"], context)) {
        getMeaningDfs(pyError, pyCodeFile, node["block"])
    }
}
//TODO: errorDescription dfsRegexNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node, std::string regexApplyTo);
//TODO: errorDescription dfsReturnTypeNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node);
