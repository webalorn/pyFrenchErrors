#include "pyFile.hpp"
#include <iostream>

PyFile::PyFile(std::vector<std::string> lines) {
    linesCode = lines;
    //std::cerr << "---------- CONSTRUCT PYFILE"<< std::endl;
    nbOpenBracketsLine = std::vector<int>(lines.size(), 0);
    // Parsing
    int nbOpenBrackets = 0;
    char openQuotes = 0;
    for (size_t iLine = 0; iLine < lines.size(); iLine++) {
        //std::cerr << "=====> iLine = " << iLine << std::endl;
        for (char codeCar : lines[iLine]) {
            //std::cerr << "--- c=" << codeCar <<std::endl;
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
        //std::cerr << "Opening brackets = " << nbOpenBrackets << std::endl;
        nbOpenBracketsLine[iLine] = nbOpenBrackets;
    }
    //std::cerr << "---------- END CONSTRUCT PYFILE"<< std::endl;
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
