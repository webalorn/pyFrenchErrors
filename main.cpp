#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "json.hpp"

#include "pyError.hpp"
#include "pyErrorMeaningTree.hpp"
#include "tradErrorMessages.hpp"
#include "pyFile.hpp"

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
        /*
            Construction des objets persistants (indépendants du cas)
        */
        PyErrorMeaningTree meaningTree(std::ifstream("data/pyErrorMeaningTree.json"));
        TradErrorMessages traductions(std::ifstream("data/errorMessageTranslate.json"));

        /*
            Éxecution du code python, récupération du code et de la sortie
        */
        system("python3 py/in_python_code.py 2> py/in_python_erros > pyOut");
        PyFile codeFile(getFile("py/in_python_code.py"));
        PyError pyErr(getFile("py/in_python_erros"));
        /*
            Inteprétation du messages
        */
        std::string langage = "fr";
        errorDescription meaning = meaningTree.getMeaningMessages(pyErr, codeFile);


        /*int realErrorLineNumber = meaning.second;
        std::string realErrorMessage = traductions.getMessage(meaning.first, langage);

        *//*
            Affichage
        *//*
        std::cout << "-> Une erreure s'est produite à la ligne " << realErrorLineNumber + 1  << " de ton code" << std::endl;
        std::cout << "Tu as écrit le code: " << std::endl << codeFile.getLine(realErrorLineNumber).get() << std::endl << std::endl;

        if (realErrorMessage != "") {
            std::cout << "-> Voici le problème: " << std::endl;
            std::cout << realErrorMessage << std::endl;
        } else {
            std::cout << "Nous ne savons pas te dire ce qui est faux. Cependant, voici ce que PYTHON a affiché:" << std::endl;
            std::cout << pyErr.getType() << ": " << pyErr.getMessage() << std::endl;
        }*/
    } catch (std::string e) {
        std::cout << e << std::endl;
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << '\n';
    }
    return 0;
}
