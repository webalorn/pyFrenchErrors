#include <iostream>
#include "parseErrorOutput.hpp"
#include "interpreteErrors.hpp"
#include <fstream>
#include <cstdlib>
#include "pyCode.hpp"
#include <vector>

std::vector<std::string> getFile(std::string fileName) {
    std::cerr << "Opening file " << fileName  << "..."<< std::endl;
    std::ifstream errorFile(fileName);
    std::vector<std::string> linesOfFile;
    std::string line;
    while (std::getline(errorFile, line)) {
        linesOfFile.push_back(line);
    }
    std::cerr << "DONE" << std::endl;
    return linesOfFile;
}

int main() {
    try {
        system("python in_python_code.py 2> in_python_erros > /dev/null");
        //std::cerr << "Create pyErr" << std::endl;
        PyError pyErr(getFile("py/in_python_erros"));
        //std::cerr << "Create codeFile" << std::endl;
        PyFile codeFile(getFile("py/in_python_code.py"));
        //std::cerr << "Create meaning" << std::endl;
        PyErrorMeaning meaning(pyErr, {}, codeFile);
        //std::cerr << "Tests" << std::endl;
        if (meaning.getIfPyErrorIsTrue()) {
            std::cout << "-> Une erreure s'est produite à la ligne " << pyErr.getLine() << " de ton code" << std::endl;
            std::cout << "Tu as écrit le code: " << pyErr.getCode().getLine() << std::endl << std::endl;
        }
        if (meaning.knowWhatErrorIs()) {
            std::cout << "-> Voici le problème: " << std::endl;
            std::cout << meaning.getFrenchErrorMessage() << std::endl;
        } else {
            std::cout << "Nous ne savons pas te dire ce qui est faux. Cependant, voici ce que PYTHON a affiché:" << std::endl;
            std::cout << pyErr.getType() << ": " << pyErr.getMessage() << std::endl;
        }
    } catch (std::string e) {
        std::cout << e << std::endl;
    }
    return 0;
}
