#include "pyError.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

PyError::PyError(std::vector<std::string> errorOutput) {
    stderr = strJoin(errorOutput, "\n");
    if (errorOutput.empty()) {
        throw std::string("Il n'y a pas d'erreur");
    }

    lineOfError = PyError::undefinedPosition, charOfError = PyError::undefinedPosition;
    auto lastLineIt = --errorOutput.end();
    auto errorBeginPt = errorOutput.begin();

    std::string fileName = "";
    for (auto lineIt = errorOutput.begin(); lineIt != lastLineIt; ++lineIt) {
        std::smatch extract;
        if (std::regex_search(*lineIt, extract, std::regex("  File \"(.*)\", line .*"))) {
            if (fileName == "" || fileName == extract[1]) {
                errorBeginPt = lineIt;
                fileName = extract[1];
            }
        }
    }

    parseErrorLastLine(*lastLineIt);

    std::vector<std::string> errorBlock(errorBeginPt, lastLineIt);
    parseTraceback(errorBlock);
}

void PyError::parseErrorLastLine(std::string line) {
    std::regex rgx("^([^:]*): (.*)");
    std::smatch match;

    if (std::regex_search(line, match, rgx)) {
        typeOfError = match[1], detailedMessage = match[2];
    } else {
        typeOfError = "?";
        detailedMessage = line;
    }
}

void PyError::parseTraceback(std::vector<std::string> blockLines) {
    std::smatch match;
    for (size_t iLine = 0; iLine < blockLines.size(); iLine++) {
        if (iLine == 0) {
            std::regex rgxGetLine(", line ([0-9]*)(, in )?(.*)");
            if (std::regex_search(blockLines[iLine], match, rgxGetLine)) {
                lineOfError = stoi(match[1]) - 1; /* -1: from [1 : N] to [0 : N-1] */
            } else {
                std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else if (iLine == 1) {
            pyLine = blockLines[iLine];
            std::regex rgxGetCode("^    (.*)$");
            if (std::regex_search(blockLines[iLine], match, rgxGetCode)) {
                pyLine = match[1];
            } else {
                std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else if (iLine == 2) {
            std::regex rgxGetchar("^    ( *)\\^");
            if (std::regex_search(blockLines[iLine], match, rgxGetchar)) {
                charOfError = std::string(match[1]).size();
            } else {
                std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << std::endl;
            }
        } else {
            std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << std::endl;
        }
    }
}

int PyError::getLineNumber(){
    return lineOfError;
}
int PyError::getCharPos(){
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
std::string PyError::getDisplayableStderr() {
    // Remove the block ids from the stderr
    std::regex reg("#BlockIds=[^\n]*\n");
    std::string e = std::regex_replace(stderr, reg,"\n");
    return e;
}