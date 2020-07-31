#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

#include "pyError.hpp"
#include "tradErrorMessages.hpp"
#include "pyFile.hpp"
#include "errorParser.hpp"
#include "utility.hpp"

std::vector<std::string> getFile(std::string fileName) {
    std::ifstream errorFile(fileName);
    if (errorFile.bad()) {
        LogError::logFatal("Impossible d'ouvrir le fichier `" + fileName + "`.");
    }

    std::vector<std::string> linesOfFile;
    std::string line;
    while (std::getline(errorFile, line)) {
        linesOfFile.push_back(line);
    }
    return linesOfFile;
}

void showOutput(ParsedError& err, PyFile& codeFile) {
    std::cout << "-> Une erreur s'est produite à la ligne " << err.line + 1  << " du code" << std::endl;
    std::cout << "Vous avez écris le code : " << std::endl << codeFile.getLine(err.line).get() << std::endl << std::endl;

    std::cout << "-> Voici le problème : " << std::endl;
    std::cout << err.text << std::endl;
}

int main(int argc, char* argv[]) {
   if (argc < 4) {
        std::cerr << "Erreur : pas assez d'arguments." << std::endl;
        std::cerr << "Utilisation: pyfe solution.py pythonStderr outputDest.json [target(=python)] [langage(=fr)]" << std::endl;
        return 1;
    }

    std::string codePath = argv[1];
    std::string stderrPath = argv[2];
    std::string jsonOutput = argv[3];

    std::string target = argc > 4 ? argv[4] : "python";
    std::string langage = argc > 5 ? argv[5] : "fr";

    try {

        /*
            Construction des objets persistants (indépendants du cas)
        */
        std::ifstream tradFile("errorMessages.json");
        TradErrorMessages translator(tradFile);

        /*
            Récupération du code et de la sortie
        */
        PyFile codeFile(getFile(codePath));
        PyError pyErr(getFile(stderrPath));

        /*
            Inteprétation du message
        */
        ErrorParser parser(pyErr, codeFile);
        ParsedError err = parser.parse();
        translator.setMessage(&err, langage, target);

        /*
            Affichage de la sortie
        */
        std::ofstream outFile(jsonOutput);
        showOutput(err, codeFile);
        outFile << err.toJson();

    } catch (std::string e) { // In case there is no error
        std::cout << e << std::endl;
        return 1;
    } catch (const std::invalid_argument& ia) {
        LogError::logFatal("Invalid argument : " + std::string(ia.what()));
    }
    return 0;
}
