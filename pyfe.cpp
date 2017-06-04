#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "json.hpp"

#include "pyError.hpp"
#include "pyErrorMeaningTree.hpp"
#include "tradErrorMessages.hpp"
#include "pyFile.hpp"

using namespace std;

using Json = nlohmann::json;

vector<string> getFile(string fileName) {
    ifstream errorFile(fileName);
    if (errorFile.bad()) {
        cerr << "Erreur: impossible d'ouvrir le fichier `" << fileName << "`." << endl;
        exit(EXIT_FAILURE);
    }
    vector<string> linesOfFile;
    string line;
    while (getline(errorFile, line)) {
        linesOfFile.push_back(line);
    }
    return linesOfFile;
}

int main(int argc, char** argv) {
    // Check number of arguments
    if(argc != 3) {
        cerr << "Erreur: pas assez d'arguments." << endl;
        cerr << "Utilisation: pyfe solution.py sortieErreur" << endl;

        return 1;
    }

    bool errorTranslated = false;

    try {
        /*
            Construction des objets persistants (indépendants du cas)
        */
        ifstream emFile("data/pyErrorMeaningTree.json");
        PyErrorMeaningTree meaningTree = PyErrorMeaningTree(&emFile);
        ifstream tradFile("data/errorMessageTranslate.json");
        TradErrorMessages traductions(&tradFile);

        // Code source de la solution
        PyFile codeFile(getFile(argv[1]));
        // Sortie d'erreur
        PyError pyErr(getFile(argv[2]));

        /*
            Interprétation du message
        */
        string langage = "fr";
        errorDescription meaning = meaningTree.getMeaningMessages(pyErr, codeFile);

        int realErrorLineNumber = meaning.errLine-1;
        string realErrorMessage = traductions.getMessage(meaning.messages, langage);

        /*
            Affichage de la sortie
        */
        cout << "Une erreur s'est produite à la ligne " << realErrorLineNumber + 1 << " de votre code." << endl;
//        cout << "Tu as écrit le code : " << endl << codeFile.getLine(realErrorLineNumber).get() << endl << endl;

        if (realErrorMessage != "") {
            cout << realErrorMessage << endl;
            errorTranslated = true;
/*        } else {
            cout << "Message d'erreur Python :" << endl;
            cout << pyErr.getType() << " : " << pyErr.getMessage() << endl;*/
        }
    } catch (string e) {
        cout << e << endl;
    } catch (const invalid_argument& ia) {
        cerr << "Invalid argument: " << ia.what() << '\n';
    }
    return errorTranslated ? 0 : 2;
}
