#include "pyErrorMeaningTree.hpp"
#include <iostream>
#include "meaningTreeFcts.hpp"

PyErrorMeaningTree::PyErrorMeaningTree (std::ifstream dataFile) {
    try {
        dataFile >> tree;
    } catch (const std::invalid_argument& ia) {
        LogError::logFatal("Construction PyErrorMeaningTree, fichier JSON invalide: " + std::string(ia.what()));
    }
    boolFcts = getBoolTreeFcts();
    getFcts = getStringTreeFcts();
}

errorDescription PyErrorMeaningTree::getMeaningMessages(PyError& pyError, PyFile& pyCodeFile) {
    errorDescription err = getMeaningDfs(pyError, pyCodeFile, tree);
    return err;
}

errorDescription PyErrorMeaningTree::getMeaningDfs(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    errorDescription errMessages = {pyError.getLineNumber(), {}};
    //std::cerr << "PyErrorMeaningTree::getMeaningDfs" << node << std::endl;
    if (node.is_object()) {

        // traitement selon le type
        if (node.find("condition") != node.end()) {
            return dfsConditionNode(pyError, pyCodeFile, node);
        } else if (node.find("type") != node.end()) {
            std::string typeOfNode = node["type"];

            if (typeOfNode == "errorMessage")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getMessage());
            if (typeOfNode == "errorType")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getType());
            if (typeOfNode == "errorCodeLine")
                return dfsRegexNode(pyError, pyCodeFile, node, pyError.getPyLine().get());
        } else if (node.find("typeError") != node.end()) {
            return dfsReturnTypeNode(pyError, pyCodeFile, node);
        } else if (node.find("regexOn") != node.end()) {
            std::string regexOnValue = node["regexOn"];
            return dfsRegexNode(pyError, pyCodeFile, node, evaluateParamVal(regexOnValue, {{}, pyError, pyCodeFile}));
        } else {
            LogError::log("PyErrorMeaningTree::getMeaningDfs: can't identifie block: " + node.dump());
        }

    } else if (node.is_array()) {
        for (nlohmann::json::iterator it = node.begin(); it != node.end() && !errMessages.isDefined(); ++it) {
            errMessages = getMeaningDfs(pyError, pyCodeFile, *it);
        }
    } else if (node.is_string()) {
        const std::string typeError = node;
        errMessages.messages.push_back({typeError, {}});
    } else {
        LogError::log("PyErrorMeaningTree::getMeaningDfs: can't identifie block: " + node.dump());
    }
    return errMessages;
}

bool PyErrorMeaningTree::useBoolFct(std::string name, FctContext context) {
    if (boolFcts.find(name) == boolFcts.end()) {
        LogError::log("Fonction booléenne dans l'arbre meaning inconnue: "+name);
        return false;
    }
    return boolFcts[name](context);
}
std::string PyErrorMeaningTree::getReturnFct(std::string name, FctContext context) {
    if (getFcts.find(name) == getFcts.end()) {
        LogError::log("PyErrorMeaningTree::getReturnFct:  fonction inexistante: "+name);
        return "";
    }
    return getFcts[name](context);
}

errorDescription PyErrorMeaningTree::dfsConditionNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    //std::cerr << "Conditional node: " << node["condition"] << std::endl;
    FctContext context = {{}, pyError, pyCodeFile};
    if (node.find("params") != node.end()) {
        //context.params = node["params"];
        for (std::string p : node["params"]) {
            context.params.push_back(p);
        }
    } if (useBoolFct(node["condition"], context)) {
        //std::cerr << "|-> TRUE" << std::endl;
        if (node.find("block") != node.end()) {
            //std::cerr << "block exist" << std::endl;
            errorDescription e = getMeaningDfs(pyError, pyCodeFile, node["block"]);
            //std::cerr << "isDefined-> " << std::boolalpha << e.isDefined() << std::endl;
            if (e.isDefined()) {
                //std::cerr << e.messages[0].messageId << std::endl;
            }
            return e;
        }
    } else {
        //std::cerr << "|-> FALSE" << std::endl;
        if (node.find("else") != node.end())
            return getMeaningDfs(pyError, pyCodeFile, node["else"]);
    }
    return {pyError.getLineNumber(), {}};
}
errorDescription PyErrorMeaningTree::dfsRegexNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node, std::string regexApplyTo) {
    std::smatch match;
    std::string varName = "";
    errorDescription errMessages = {pyError.getLineNumber(), {}};
    //std::cerr << "Regex on : " << regexApplyTo << std::endl;
    try {

        if (node.find("var") != node.end())
            varName = node["var"];
        for (nlohmann::json::iterator it = node.begin(); it != node.end() && !errMessages.isDefined(); ++it) {
            if (it.key() == "type" || it.key() == "var" || it.key() == "default" || it.key() == "regexOn")
                continue;
            std::regex regexOfNode = std::regex(it.key());
            //std::cerr << "Regex expr: " << it.key() << std::endl;
            if (std::regex_match(regexApplyTo, regexOfNode) && std::regex_search(regexApplyTo, match, regexOfNode)) {
                regexExtracts[varName] = match;
                errMessages = getMeaningDfs(pyError, pyCodeFile, *it);
            }
        }
        if (node.find("default") != node.end() && !errMessages.isDefined())
            errMessages = getMeaningDfs(pyError, pyCodeFile, node["default"]);
        if (regexExtracts.find(varName) != regexExtracts.end())
            regexExtracts.erase(regexExtracts.find(varName));
    } catch (const std::invalid_argument& ia) {
    }
    return errMessages;
}
errorDescription PyErrorMeaningTree::dfsReturnTypeNode(PyError& pyError, PyFile& pyCodeFile, nlohmann::json& node) {
    //std::cerr << "---> dfsReturnTypeNode" << std::endl;
    errorDescription errMessages = {pyError.getLineNumber(), {}};

    if (node.find("realErrorLine") != node.end()) {
        std::string realErrorLineStr = node["realErrorLine"];
        try {
            errMessages.errLine =  stoi(evaluateParamVal(realErrorLineStr, {{}, pyError, pyCodeFile}));
        } catch (std::invalid_argument e) {
            LogError::log("PyErrorMeaningTree::dfsReturnTypeNode "+realErrorLineStr+" can't be evaluate as a number");
        }
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
        //std::cerr << "Nodes concatenate" << std::endl;
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
        //std::cerr << "tab access: " << paramVal << std::endl;
        int id = stoi(match[2]);
        if (regexExtracts.find(match[1]) != regexExtracts.end())
            return regexExtracts[match[1]][id];
        else
        LogError::log("PyErrorMeaningTree::evaluateParamVal:  Paramètre inexistant: "+paramVal);
    }
    return "";
}
