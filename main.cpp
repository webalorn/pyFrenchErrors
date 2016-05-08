#include <iostream>
#include "parseErrorOutput.hpp"
#include "interpreteErrors.hpp"
#include <fstream>
#include <cstdlib>
#include "pyCode.hpp"
#include <vector>
#include "json.hpp"

using Json = nlohmann::json;

std::vector<std::string> getFile(std::string fileName) {
    std::ifstream errorFile(fileName);
    std::vector<std::string> linesOfFile;
    std::string line;
    while (std::getline(errorFile, line)) {
        linesOfFile.push_back(line);
    }
    return linesOfFile;
}

int main() {
    try {
        system("python3 py/in_python_code.py 2> py/in_python_erros > pyOut");
        PyFile codeFile(getFile("py/in_python_code.py"));
        PyError pyErr(getFile("py/in_python_erros"));
        PyErrorMeaning meaning(pyErr, {"module.MyError"}, codeFile);


        std::cout << "-> Une erreure s'est produite à la ligne " << meaning.getRealErrorLine() + 1 /* +1: for humans */ << " de ton code" << std::endl;
        std::cout << "Tu as écrit le code: " << std::endl << codeFile.getLine(meaning.getRealErrorLine()) << std::endl << std::endl;

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
