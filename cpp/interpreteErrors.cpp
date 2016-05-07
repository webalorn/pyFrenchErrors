#include "interpreteErrors.hpp"
#include <string>
#include <map>
#include <regex>
#include <iostream>

PyErrorMeaning::PyErrorMeaning(PyError error, std::vector<std::string> addedExceptions, PyFile codeFile) {
    PyCode code = error.getCode();
    std::string errMessage = error.getMessage();
    std::smatch m;

    auto getSyntaxErrorId = [&]() {
        if (errMessage == "EOL while scanning string literal")
            return frErr::guillemetsError;
        if (std::regex_match(errMessage, std::regex("Non-ASCII character .* in file .* but no encoding declared; .*")))
            return frErr::nonAscii;
        if (errMessage == "'return' outside function")
            return frErr::returnOutsideFct;
        if (code.isStruct() && !code.endByTwoPoints()) {
            std::map<std::string, frErr> deuxPointsStructs = {
                {"for", frErr::deuxPointsFor},
                {"if", frErr::deuxPointsIf},
                {"elif", frErr::deuxPointsElif},
                {"else", frErr::deuxPointsElse},
                {"def", frErr::deuxPointsDef},
                {"while", frErr::deuxPointsWhile}
            };
            std::string firstWord = code.getFirstWord();
            if (deuxPointsStructs.find(firstWord) != deuxPointsStructs.end()) {
                return deuxPointsStructs[firstWord];
            }
        }
        if (std::regex_match(code.getLine(), std::regex(".*;[[:blank:]]*")))
            return frErr::pointVirguleFin;
        if (std::regex_match(code.getLine(), std::regex("\\s*(\\w)+\\s+(\\w)+\\s*=.*")))
            return frErr::typeVariableDeclaration;

        if (code.isConditianal() && std::regex_match(code.getLine(), std::regex(".*[^=]=[^=].*"))) {
            return frErr::conditionOneEqual;
        }
        if (codeFile.getNbOpenBrackets(error.getLine()) > 0) {
            //paramsFr[0] = std::to_string(codeFile.getLineOpenFirstBracket(error.getLine()) + 1);
            realLineOfError = codeFile.getLineOpenFirstBracket(error.getLine());
            return frErr::previousErrorBrackets;
        }
        if (code.countOpenCloseBrackets() != 0)
            return frErr::errorBrackets;
        if (code.isConditianal() && std::regex_match(code.getLine(), std::regex(".*=>.*")))
            return frErr::plusGrandEgalInversion;
        if (code.isConditianal() && std::regex_match(code.getLine(), std::regex(".*=<.*")))
            return frErr::plusPetitEgalInversion;
        return frErr::invalidSyntax;
    };

    auto getIndentationError = [&]() {
        if (errMessage == "expected an indented block")
            return frErr::indentationErrorEmptyBlock;
        if (errMessage == "unindent does not match any outer indentation level")
            return frErr::indentationErrorLevel;
        return frErr::indentationError;
    };

    auto getNameError = [&]() {
        if (std::regex_search(errMessage, m, std::regex("^name '(.*)' is not defined$"))) {
            paramsFr[0] = m[1];
            return frErr::undefinedVar;
        }
        return frErr::undefined;
    };

    auto getValueError = [&]() {
        std::cerr << "Test value error: '" << errMessage << "'" << std::endl;
        if (errMessage == "math domain error") {
            paramsFr[0] = "";
            if (code.contain("log"))
                paramsFr[0] += "La fonction log (logarithme) prend en paramètre un nombre qui doit être STRICTEMENT SUPÉRIEUR à 0. ";
            if (code.contain("sqrt"))
                paramsFr[0] += "La fonction sqrt (racine carrée) prend en paramètre un nombre qui doit être SUPÉRIEUR OU ÉGAL à 0. ";
            // TODO : principalles fonctions python pouvant donner une erreure de domaine mathématique
            return frErr::mathDomainError;
        }
        if (regex_search(errMessage, m, std::regex("invalid literal for int\\(\\) with base [[:digit:]]*: '(.*)'"))) {
            std::string chaine = m[1];
            if (regex_match(chaine, std::regex("([[:blank:]]*-?[[:d:]]+[[:blank:]]*)+")))
                return frErr::valueErrorIntMappage;
            if (regex_match(chaine, std::regex("[[:blank:]]*-?[[:d:]]+\\.[[:d:]]+[[:blank:]]*")))
                return frErr::valueErrorIntInsteadOfFloat;
            return frErr::valueErrorInt;
        }
        return frErr::undefined;
    };

    auto getErrorId = [&]()->frErr {
        if (error.getType() == "SyntaxError")
            return getSyntaxErrorId();
        if (error.getType() == "IndentationError")
            return getIndentationError();
        if (error.getType() == "ValueError")
            return getValueError();
        if (error.getType() == "NameError")
            return getNameError();
        if (error.getType() == "IndexError")
            return frErr::indexError;
        if (error.getType() == "KeyError")
            return frErr::KeyError;
        if (error.getType() == "ZeroDivisionError")
            return frErr::ZeroDivisionError;
        if (error.getType() == "ImportError")
            return frErr::ImportError;
        if (error.getType() == "EOFError")
            return frErr::EOFError;
        return frErr::undefined;
    };
    realLineOfError = error.getLine();
    errorMessageId = getErrorId();
    //std::cerr << "Error type: '" << error.getType() << "'" << std::endl;
    for (std::string customType : addedExceptions) {
        if (error.getType() == customType) {
            paramsFr[0] = error.getMessage();
            errorMessageId = frErr::custom;
        }
    }
}

std::string PyErrorMeaning::frMeaning(frErr errorMessageId) {
    std::string defaultMessage = "Désolé, mais nous n'avons pas pu déterminer la cause de ton erreure.";
    switch (errorMessageId) {
        // Invalid Syntax
        case frErr::deuxPointsIf: return "Une condition 'if' doit se terminer par les deux poiint ':' comme ceci -> 'if a < b:'";
        case frErr::deuxPointsElse: return "Une condition 'else' doit se terminer par les deux points ':' comme ceci -> 'else:'";
        case frErr::deuxPointsElif: return "Une condition 'elif' doit se terminer par les deux points ':' comme ceci -> 'else if b < c:'";
        case frErr::deuxPointsWhile: return "Une boucle 'while' doit se terminer par les deux points ':' comme ceci -> 'while maVariable < 5:'";
        case frErr::deuxPointsFor: return "Une boucle 'for' doit se terminer par les deux points ':' comme ceci -> 'for loop in range(5):'";
        case frErr::deuxPointsDef: return "Une définition de fonction 'def' doit se terminer par les deux points ':' comme ceci -> 'def maFonction():'";

        case frErr::conditionOneEqual: return "Pour écrire une égalitée dans une condition, on doit mettre un double égal, et donc écrire: a == b";
        case frErr::previousErrorBrackets:
        case frErr::errorBrackets: return "Tu dois avoir autant de parenthèses ouvrantes que fermetantes: tu peux écrire (a*(b+c)) mais PAS (a*(b+c) NI a*(b+c))";

        case frErr::pointVirguleFin: return "En python, il n'y a pas besoin de mettre un point-virgule à la fin d'une instruction";
        case frErr::typeVariableDeclaration: return "En python, on n'a pas besoin de donner le type d'une variable lors de sa déclaration";

        case frErr::plusGrandEgalInversion: return "Tu ne dois PAS écrire => mais : >=";
        case frErr::plusPetitEgalInversion: return "Tu ne dois PAS écrire =< mais : <=";
        case frErr::guillemetsError: return "Tu n'a pas bien fermé tes guillemets: tu dois avoir autant de guillements ouvrant que fermant autour d'une chaine de caractère. \nPar exemple, tu dois écrire 'Hello world !' ou encore \"Hello world !\"";

        case frErr::nonAscii: return "Tu as utilisé un accent dans ton code, et tu n'as pas le droit ! Enlève l'accent, ou alors rajoute au début de ton code, avant TOUTES les autres lignes: \"# coding=utf-8\"";
        case frErr::invalidSyntax: return "Ton code n'est pas écrit correctement ! Vérifie bien ce que tu as écrit.";

        // Exceptions
        case frErr::indentationError: return "Tu as une erreure dans l'indentation: tu dois mettre le même nombre d'espace devant les instructions d'un MEME bloc";
        case frErr::indentationErrorEmptyBlock: return "Tu as un bloc d'instruction (if, elif, else, while, for, def...) qui est VIDE, ce qui est interdit";
        case frErr::indentationErrorLevel: return "TOUTES les instructions d'un même bloc d'instruction(if, elif, else, while, for, def...) doivent avoir le même espace devant elle (sauf si elles sont dans des sous-blocs)";

        case frErr::indexError: return "Tu accède à une case de la liste qui n'existe pas. Si tu utilise une variable, vérifie qu'elle ne peut pas être trop grande. Souviens toi que les cases d'une liste sont numérotées de 0 à TAILLE-1: dans une liste de taille 5, les cases -3, 5 et 6 n'existent PAS";
        case frErr::KeyError: return "Tu accès à une CLÉE (une case) de ton dictionaire qui n'existe pas. Vérifie que ta clée existe bien !";
        case frErr::ZeroDivisionError: return "Cette instruction effectue une division pas zero, ce qui est IMPOSSIBLE. Si tu utilise une variable, vérifie que le diviseur le vaut jamais 0";
        case frErr::mathDomainError: return std::string("Tu utilise un fonction mathématique en lui donnant un nombre qui est interdit ! ") + paramsFr[0];
        case frErr::ImportError: return "Un fichier ou un module inexistant est importé. Vérifie bien son nom.";
        case frErr::EOFError: return "Ton code à tenté de lire une ligne d'entrée, mais il n'y en avais pas ou plus ! A chaque fois que tu utilise input(), ton code lit une nouvelle ligne. Une erreure se produit quand plus aucune ligne n'a pas étée lue.";

        case frErr::valueErrorInt: return "Tu as essayé de convertire en nombre entier (int) une chaine de caractère, mais celle-ci ne représente pas un nombre entier! (ou alors, pas seulement un unique nombre entier) Si elle contient plusieurs éléments, tu peut les séparer en faisant [chaine].split() (en remplaçant [chaine] par la chaine de caractères)";
        case frErr::valueErrorIntMappage: return "Tu utilise la fonction int() pour convertire en nombre une suite d'entiers. Mais tu convertit plusieurs nombres à la fois ! pour cela, remplace int(--ton code--) par: map(int, (--ton code--).split())";
        case frErr::valueErrorIntInsteadOfFloat: return "Tu essaye de convertire un nombre a virgule en nombre entier. Pour utiliser un nombre a virgule, remplace int() par float()";

        case frErr::undefinedVar: return std::string("La variable (ou la fonction) ") + paramsFr[0] + std::string(" n'existe pas, ou pas encore, à cet endroit. Vérifie que tu ne t'es pas trompé dans le nom ! Si c'est toi qui l'a créée, vérifie que tu le fait AVANT. Sinon, vérifie que le module et bien importé.");
        case frErr::returnOutsideFct: return "L'instruction 'return' doit se trouver dans une fonction. Elle termine la fonction et indique ce qu'elle retourne.";

        // other
        case frErr::custom: return paramsFr[0];
        case frErr::undefined:;
    }
    return defaultMessage;
}
std::string PyErrorMeaning::getFrenchErrorMessage() {
    return frMeaning(errorMessageId);
}
bool PyErrorMeaning::knowWhatErrorIs() {
    return (errorMessageId != frErr::undefined);
}
int PyErrorMeaning::getRealErrorLine() {
    return realLineOfError;
}
