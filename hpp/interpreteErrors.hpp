#ifndef interpreteErrors_H
#define interpreteErrors_H
#include "parseErrorOutput.hpp"
#include "pyFile.hpp"
#include <string>
#include <vector>
#include <array>

enum class frErr {
    // invalid syntaxes
    deuxPointsIf, deuxPointsFor, deuxPointsWhile, deuxPointsElse, deuxPointsElif, deuxPointsDef,
    conditionOneEqual, errorBrackets, previousErrorBrackets,
    plusGrandEgalInversion, plusPetitEgalInversion,
    guillemetsError, invalidSyntax,

    // exeptions
    indentationErrorEmptyBlock, indentationErrorLevel, indentationError,
    indexError,
    KeyError,
    ZeroDivisionError,
    ImportError,

    undefinedVar,
    returnOutsideFct,
    // other...
    custom,
    undefined
};

class PyErrorMeaning {
public:
    PyErrorMeaning(PyError, std::vector<std::string>, PyFile);
    std::string getFrenchErrorMessage();
    bool knowWhatErrorIs();
    bool getIfPyErrorIsTrue();

private:
    frErr errorMessageId;
    static const int MAX_PARAMETERS = 3;
    std::array<std::string, PyErrorMeaning::MAX_PARAMETERS> paramsFr;
    bool errorIsTrue;
public:
    std::string frMeaning(frErr);
};

#endif
