#include "pyError.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <iostream>

PyError::PyError(std::vector<std::string> errorOutput) {
    if (errorOutput.empty()) {
        throw std::string("Il n'y a pas d'erreur");
    }

    lineOfError = PyError::undefinedPosition, charOfError = PyError::undefinedPosition;
    auto lastLineIt = --errorOutput.end();
    auto blockContentError = errorOutput.begin();

    std::string fileName = "";
    for (auto lineIt = errorOutput.begin(); lineIt != lastLineIt; ++lineIt) {
        std::smatch extract;
        if (std::regex_search(*lineIt, extract, std::regex("  File \"(.*)\", line .*"))) {
            if (fileName == "" || fileName == extract[1]) {
                blockContentError = lineIt;
                fileName = extract[1];
            }
        }
    }
    //std::cerr << "Good line: " << *blockContentError << std::endl;

    parseErrorLastLine(*lastLineIt);

    std::vector<std::string> lastBlock;
    copy(blockContentError, lastLineIt, std::back_inserter(lastBlock));
    parseBlockContentError(lastBlock);
}

void PyError::parseErrorLastLine(std::string line) {
    std::regex rgx("^([^:]*): (.*)");
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
                lineOfError = stoi(match[1]) - 1; /* -1: from [1 : N] to [0 : N-1] */
                //std::cerr << "Line number: " << lineOfError << std::endl;
            } else {
                // TODO: exception
                //std::cerr << "Comportement inatendu: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else if (iLine == 1) {
            pyLine = blockLines[iLine];
            std::regex rgxGetCode("^    (.*)$");
            if (std::regex_search(blockLines[iLine], match, rgxGetCode)) {
                pyLine = match[1];
                //std::cerr << "Python code: '" << pyLine << "'" << std::endl;
            } else {
                // TODO: exception
                //std::cerr << "Comportement inatendu: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else if (iLine == 2) {
            std::regex rgxGetchar("    ( *)\\^");
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

int PyError::getLineNumber(){
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
PyLine PyError::getPyLine(){
    return PyLine(pyLine);
}
