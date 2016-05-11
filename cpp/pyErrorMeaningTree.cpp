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
    //std::cerr << "---> getMeaningDfs" << std::endl;
    errorDescription errMessages = {pyError.getLineNumber(), {}};

    if (node.is_object()) {

        // traitement selon le type
        if (node.find("condition") != node.end())
            return dfsConditionNode(pyError, pyCodeFile, node);

        if (node.find("type") != node.end()) {
            std::string typeOfNode = node["type"];

            if (typeOfNode == "errorMessage")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getMessage());
            if (typeOfNode == "errorType")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getType());
            if (typeOfNode == "errorCodeLine")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getPyLine().get());
        }
        if (node.find("typeError") != node.end()) {
            return dfsReturnTypeNode(pyError, pyCodeFile, node);
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
std::string PyErrorMeaningTree::getReturnFct(std::string name, FctContext context) {
    if (getFcts.find(name) == getFcts.end())
        return "";
    return getFcts[name](context);
}

errorDescription PyErrorMeaningTree::dfsConditionNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    //std::cerr << "---> dfsConditionNode" << std::endl;
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
    //std::cerr << "---> dfsRegexNode sur: " << regexApplyTo << std::endl;
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
            //std::cerr << "Test match with " << it.key() << std::endl;
            if (std::regex_match(regexApplyTo, regexOfNode) && std::regex_search(regexApplyTo, match, regexOfNode)) {
                //std::cerr << "TRUE" << std::endl;
                regexExtracts[varName] = match;
                errMessages = getMeaningDfs(pyError, pyCodeFile, *it);
            }
        }
        if (regexExtracts.find(varName) != regexExtracts.end())
            regexExtracts.erase(regexExtracts.find(varName));
    } catch (const std::invalid_argument& ia) {
    }
    return errMessages;
}
errorDescription PyErrorMeaningTree::dfsReturnTypeNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    std::cerr << "---> dfsReturnTypeNode" << std::endl;
    errorDescription errMessages = {pyError.getLineNumber(), {}};

    //TODO
    if (node.find("realErrorLine") != node.end()) {
        std::string realErrorLineStr = node["realErrorLine"];
        errMessages.errLine =  stoi(realErrorLineStr);
    }
    errorMsgParams addMsg;
    addMsg.messageId = node["typeError"];
    if (node.find("params") != node.end()) {
        for (std::string paramVal : node["params"]) {
            addMsg.params.push_back(evaluateParamVal(paramVal, {{}, pyError, pyCodeFile}));
        }
    }
    errMessages.messages.push_back(addMsg);

    if (node.find("concatenate") != node.end()) {
        for (auto otherTree : node["concatenate"]) {
            errMessages.push(getMeaningDfs(pyError, pyCodeFile, otherTree));
        }
    }

    return errMessages;
}

std::string PyErrorMeaningTree::evaluateParamVal(std::string paramVal, FctContext context) {
    std::smatch match;
    //std::cerr << "évalutate param: " << paramVal << std::endl;
    if (std::regex_search(paramVal, match, std::regex("^(.*)\\((.*)\\)$"))) {
        //std::cerr << "is a function" << std::endl;
        context.params.push_back(match[2]);
        return getReturnFct(match[1], context);
    }
    if (std::regex_search(paramVal, match, std::regex("^(.*)\\[([[:digit:]]*)\\]$"))) {
        //std::cerr << "tab access" << std::endl;
        int id = stoi(match[2]);
        if (regexExtracts.find(match[1]) != regexExtracts.end())
            return regexExtracts[match[1]][id];
    }
    return "";
}
