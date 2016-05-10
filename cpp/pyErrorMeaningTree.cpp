#include "pyErrorMeaningTree.hpp"
#include <iostream>

PyErrorMeaningTree::PyErrorMeaningTree (std::ifstream dataFile) {
    try {
        dataFile >> tree;
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Construction PyErrorMeaningTree: " << ia.what() << '\n';
    }
    /*for (nlohmann::json::iterator it = tree.begin(); it != tree.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << std::endl;
    }*/
    // TODO: création des fonctions
}

errorDescription PyErrorMeaningTree::getMeaningMessages(PyError& pyError, PyFile& pyCodeFile) {
    errorDescription err = getMeaningDfs(pyError, pyCodeFile, tree);
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
        if (node.find("typeError") != node.end()) {
            return dfsReturnTypeNode(pyError, pyCodeFile, node); // TODO new function
        }

    } else if (node.is_array()) {
        for (nlohmann::json::iterator it = node.begin(); it != node.end() && !errMessages.isDefined(); ++it) {
            errMessages = getMeaningDfs(pyError, pyCodeFile, *it);
        }
    } else if (node.is_string()) {
        const std::string typeError = node;
        errMessages.messages.push_back({typeError, {}});
    }
    return errMessages;
}

bool PyErrorMeaningTree::useBoolFct(std::string name, FctContext context) {
    if (boolFcts.find(name) == boolFcts.end())
        return false;
    return boolFcts[name](context);
}
/*std::string getReturnFct(std::string name, FctContext context) {
    if (getFcts.find(name) == getFcts.end())
        return "";
    return getFcts[name](context);
}*/

errorDescription PyErrorMeaningTree::dfsConditionNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    FctContext context = {{}, pyError, pyCodeFile};
    if (node.find("params") != node.end()) {
        //context.params = node["params"];
        for (std::string p : node["params"]) {
            context.params.push_back(p);
        }
    } if (node.find("block") != node.end() && useBoolFct(node["condition"], context)) {
        return getMeaningDfs(pyError, pyCodeFile, node["block"]);
    }
    return {pyError.getLineNumber(), {}};
}
errorDescription PyErrorMeaningTree::dfsRegexNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node, std::string regexApplyTo) {
    std::smatch match;
    std::string varName = "";
    errorDescription errMessages = {pyError.getLineNumber(), {}};
    try {

        if (node.find("var") != node.end())
            varName = node["var"];
        for (nlohmann::json::iterator it = node.begin(); it != node.end() && !errMessages.isDefined(); ++it) {
            if (it.key() == "type" || it.key() == "var")
                continue;
            std::regex regexOfNode = std::regex(it.key());
            if (std::regex_match(regexApplyTo, regexOfNode) && std::regex_search(regexApplyTo, match, regexOfNode)) {
                regexExtracts[varName] = match;
                errMessages = getMeaningDfs(pyError, pyCodeFile, *it);
            }
        }
        regexExtracts.erase(regexExtracts.find(varName));
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << '\n';
    }
    return errMessages;
}
errorDescription PyErrorMeaningTree::dfsReturnTypeNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    errorDescription errMessages = {pyError.getLineNumber(), {}};

    //TODO

    return errMessages;
}
