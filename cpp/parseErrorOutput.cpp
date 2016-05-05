#include "parseErrorOutput.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <iostream>

PyError::PyError(std::vector<std::string> errorOutput) {
    if (errorOutput.empty()) {
        throw std::string("Il n'y a pas d'erreure");
    }
    //std::cerr << "Construct" << std::endl;

    lineOfError = PyError::undefinedPosition, charOfError = PyError::undefinedPosition;
    //std::cerr << "step 1" << std::endl;
    auto lastLineIt = --errorOutput.end();
    auto blockContentError = errorOutput.begin();
    //std::cerr << "step 2" << std::endl;

    for (auto lineIt = errorOutput.begin(); lineIt != lastLineIt; ++lineIt) {
        if (std::regex_match(*lineIt, std::regex("  File .*"))) {
            blockContentError = lineIt;
        }
    }
    //std::cerr << "step 3" << std::endl;
    //std::cerr << "Lastline = '" << (*lastLineIt) << "'" << std::endl;
    //std::cerr << "last block" << std::endl;
    for (auto it = blockContentError; it != lastLineIt; it++) {
        //std::cerr << "-> " << "'"  << (*it) << "'"  << std::endl;
    }

    parseErrorLastLine(*lastLineIt);
    //std::cerr << "step 4" << std::endl;

    std::vector<std::string> lastBlock;
    copy(blockContentError, lastLineIt, std::back_inserter(lastBlock));
    parseBlockContentError(lastBlock);
    //std::cerr << "step 5" << std::endl;
}

void PyError::parseErrorLastLine(std::string line) {
    std::regex rgx("^([A-Za-z0-9]*): (.*)");
    std::smatch match;
    //std::cerr << "reagex_search " << line << std::endl;
    if (std::regex_search(line, match, rgx)) {
        typeOfError = match[1], detailedMessage = match[2];
    } else {
        // TODO: exception
    }
}

void PyError::parseBlockContentError(std::vector<std::string> blockLines) {
    std::smatch match;
    for (size_t iLine = 0; iLine < blockLines.size(); iLine++) {
        if (iLine == 0) {
            std::regex rgxGetLine(", line ([0-9]*)(, in )?(.*)");
            if (std::regex_search(blockLines[iLine], match, rgxGetLine)) {
                lineOfError = stoi(match[1]);
                //std::cerr << "Line number: " << lineOfError << std::endl;
            } else {
                // TODO: exception
                //std::cerr << "Comportement inatendu: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else if (iLine == 1) {
            pyCode = blockLines[iLine];
            std::regex rgxGetCode("^  (.*)$");
            if (std::regex_search(blockLines[iLine], match, rgxGetCode)) {
                pyCode = match[1];
                //std::cerr << "Python code: '" << pyCode << "'" << std::endl;
            } else {
                // TODO: exception
                //std::cerr << "Comportement inatendu: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else if (iLine == 2) {
            std::regex rgxGetchar("  ( *)\\^");
            if (std::regex_search(blockLines[iLine], match, rgxGetchar)) {
                charOfError = std::string(match[1]).size();
                //std::cerr << "Char number: " << charOfError << std::endl;
            } else {
                // TODO: exception
                //std::cerr << "Comportement inatendu: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else {
            //std::cerr << "Comportement inatendu: " << __FILE__ << " line " << __LINE__ << std::endl;
        }
    }
}

int PyError::getLine(){
    return lineOfError;
}
int PyError::getChar(){
    return charOfError;
}
std::string PyError::getType(){
    return typeOfError;
}
std::string PyError::getMessage(){
    return detailedMessage;
}
PyCode PyError::getCode(){
    return PyCode(pyCode);
}
