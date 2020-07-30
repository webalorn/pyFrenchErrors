#include "pyFile.hpp"

PyFile::PyFile(std::vector<std::string> lines) {
    for (std::string l : lines) {
        linesCode.push_back(l);
    }
    nbOpenBracketsLine = std::vector<int>(lines.size(), 0);

    // Parsing
    int nbOpenBrackets = 0;
    char openQuotes = 0;
    for (size_t iLine = 0; iLine < lines.size(); iLine++) {
        for (char codeCar : lines[iLine]) {
            if (openQuotes > 0) {
                if (codeCar == openQuotes) {
                    openQuotes = 0;
                }
            } else {
                if (codeCar == '"' || codeCar == '\'')
                    openQuotes = codeCar;
                else if (codeCar == '(')
                    nbOpenBrackets++;
                else if (codeCar == ')')
                    nbOpenBrackets--;
            }
        }
        nbOpenBracketsLine[iLine] = nbOpenBrackets;
    }
}

int PyFile::getNbOpenBrackets(int iLine) {
    if (iLine < 0)
        return 0;
    return nbOpenBracketsLine[iLine];
}

int PyFile::getLineOpenFirstBracket(int iLine) {
    while (iLine > 0 && getNbOpenBrackets(iLine-1) > 0)
        iLine --;
    return iLine;
}

PyLine PyFile::getLine(int iLine) {
    if (iLine < 0 || iLine >= (int)linesCode.size()) {
        return PyLine("--- Erreur: ligne inexistante ---");
    }
    return linesCode[iLine];
}
