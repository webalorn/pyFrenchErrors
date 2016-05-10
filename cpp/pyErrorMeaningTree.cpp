#include "pyErrorMeaningTree.hpp"
#include <iostream>

PyErrorMeaningTree::PyErrorMeaningTree (std::ifstream dataFile) {
    dataFile >> tree;
    /*for (nlohmann::json::iterator it = tree.begin(); it != tree.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << std::endl;
    }*/
}

errorDescription PyErrorMeaningTree::getMeaningDfs(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    errorDescription errMessages = {pyError.getLineNumber(), {}};

    if (node.is_object()) {

        // traitement selon le type
        if (node.find("type") != node.end()) {
            std::string typeOfNode = node["type"];
            if (typeOfNode == "condition")
                return dfsConditionNode(pyError, pyCodeFile, node); // TODO new function
            if (typeOfNode == "errorMessage")
                return dfsConditionNode(pyError, pyCodeFile, node, pyError.getMessage()); // TODO new function
            if (typeOfNode == "errorType")
                return dfsConditionNode(pyError, pyCodeFile, node, pyError.getType()); // TODO (cf above)
            if (typeOfNode == "errorCodeLine")
                return dfsConditionNode(pyError, pyCodeFile, node, pyError.getPyLine().get()); // TODO (cf above)
        }
        if (node.find("typeError")) {
            return treatReturnTypeError(pyError, pyCodeFile, node); // TODO new function
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

errorDescription PyErrorMeaningTree::getMeaningMessages(PyError& pyError, PyFile& pyCodeFile) {
    errorDescription err = getMeaningDfs(pyError, pyCodeFile, tree);
    // TODO: clean the structure
    return err;
}
