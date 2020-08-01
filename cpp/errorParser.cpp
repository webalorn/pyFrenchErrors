#include "errorParser.hpp"
#include "pyError.hpp"

// ParsedError

bool ParsedError::isDefined() {
    return messageId.size();
}

ParsedError::ParsedError(PyError& e, PyFile& fromFile, std::string msg, std::map<std::string, std::string> varsMap, int iLine) {
    messageId = msg;
    vars = varsMap;
    fromError = &e;

    if (iLine != -1) {
        line = iLine;
    } else {
        line = e.getLineNumber();
    }
    blockIds = fromFile.getLine(line).extractBlockIds();
}

nlohmann::json ParsedError::toJson() {
    // Define the Json object
    nlohmann::json encoded = {
        {"line", line+1},
        {"blockIds", blockIds},
        {"messageId", messageId},
        {"text", text},
        {"stderr", fromError->getDisplayableStderr()},
        {"causeFound", isDefined()},
        {"vars", vars},
    };
    // Set null values
    if (line == -1) {
        encoded["line"] = nullptr;
    }
    if (messageId == "") {
        encoded["messageId"] = nullptr;
    }
    return encoded;
}


// ErrorParser


ErrorParser::ErrorParser(PyError& e, PyFile& f) : error(e), codeFile(f) {
    errorLine = error.getLineNumber();
}

bool ErrorParser::matchWith(std::string text, std::string regString, std::vector<std::pair<int, std::string>> extractVars) {
    std::regex reg = std::regex(regString);
    std::smatch match;

    if (std::regex_match(text, reg) && std::regex_search(text, match, reg)) {
        for (auto& exVar : extractVars) {
            messageVars[exVar.second] = match[exVar.first];
        }
        return true;
    }
    return false;
}

ParsedError ErrorParser::parse() {
    std::string messageId = parseGetMessageId();
    ParsedError err(error, codeFile, messageId, messageVars, errorLine);

    return err;
}

std::string ErrorParser::parseGetMessageId() {
    std::string msg = error.getMessage();
    std::string errorCodeLine = error.getPyLine().get();
    std::string firstWord = error.getPyLine().getFirstWord();
    
    if (error.getType() == "SyntaxError") {

        if (matchWith(msg, "EOL while scanning string literal")) {
            return "guillemetsError";
        }
        if (matchWith(msg, "Non-ASCII character .* in file .* but no encoding declared; .*")) {
            return "nonAscii";
        }
        if (matchWith(msg, "'return' outside function")) {
            return "returnOutsideFct";
        }
        if (matchWith(msg, "^Missing parentheses in call to '(.*)'$", {{1, "missFunction"}})) {
            return "missingParentheses";
        }
        // ELSE
        if (codeFile.getNbOpenBrackets(error.getLineNumber()) > 0) {
            errorLine = codeFile.getLineOpenFirstBracket(error.getLineNumber());
            return "previousErrorBrackets";
        }
        if (error.getPyLine().countOpenCloseBrackets() != 0) {
            return "errorBrackets";
        }
        if (error.getPyLine().isConditianal()) {
            if (matchWith(errorCodeLine, ".*[^=<>]=[^=<>].*")) {
                return "conditionOneEqual";
            }
            if (matchWith(errorCodeLine, ".*=>.*")) {
                return "plusGrandEgalInversion";
            }
            if (matchWith(errorCodeLine, ".*=<.*")) {
                return "plusPetitEgalInversion";
            }
        }
        if (matchWith(errorCodeLine, ".*;[[:blank:]]*")) {
            return "pointVirguleFin";
        } else if (matchWith(errorCodeLine, "\\s*(\\w)+\\s+(\\w)+\\s*=.*")) {
            return "typeVariableDeclaration";
        }

        if (!error.getPyLine().endByTwoPoints() && error.getPyLine().isStruct()) {
            if (firstWord == "for") { return "deuxPointsFor"; }
            if (firstWord == "if") { return "deuxPointsIf"; }
            if (firstWord == "elif") { return "deuxPointsElif"; }
            if (firstWord == "else") { return "deuxPointsElse"; }
            if (firstWord == "def") { return "deuxPointsDef"; }
            if (firstWord == "while") { return "deuxPointsWhile"; }
        }
        return "invalidSyntax";

    } else if (error.getType() == "IndentationError") {

        if (matchWith(msg, "expected an indented block")) {
            return "indentationErrorEmptyBlock";
        }
        if (matchWith(msg, "unindent does not match any outer indentation level")) {
            return "indentationErrorLevel";
        }
        return "indentationError";

    } else if (error.getType() == "ValueError") { // TODO : float ? etc...

        if (matchWith(msg, "math domain error")) {
            messageVars["detail_log"] = matchWith(errorCodeLine, ".*log.*") ? "yes" : "";
            messageVars["detail_sqrt"] = matchWith(errorCodeLine, ".*sqrt.*") ? "yes" : "";
            return "mathDomainError";
        }
        if (matchWith(msg, "invalid literal for int\\(\\) with base [[:digit:]]*: '(.*)'", {{1, "literal"}})) {
            std::string literal = messageVars["literal"];
            if (matchWith(literal, "([[:blank:]]*-?[[:d:]]+[[:blank:]]*)+")) {
                return "valueErrorIntMappage";
            }
            if (matchWith(literal, "[[:blank:]]*-?[[:d:]]+\\.[[:d:]]+[[:blank:]]*")) {
                return "valueErrorIntInsteadOfFloat";
            }
            return "valueErrorInt";
        }
        if (matchWith(msg, "could not convert (.*) to (.*): (.*)", {{1, "type1"}, {2, "type2"}, {3, "value"}})) {
            return "CantConvert";
        }

    } else if (error.getType() == "NameError") {

        if (matchWith(msg, "name '(.*)' is not defined", {{1, "undefVar"}})) {
            return "undefinedVar";
        }

    } else if (error.getType() == "IndexError") {
        return "IndexError";
    } else if (error.getType() == "KeyError") {
        return "KeyError";
    } else if (error.getType() == "ZeroDivisionError") {
        return "ZeroDivisionError";
    } else if (error.getType() == "ImportError") {
        return "ImportError";
    } else if (error.getType() == "EOFError") {
        return "EOFError";
    } else if (error.getType() == "TypeError") {

        if (matchWith(msg, "'int' object is not subscriptable")) {
            return "IntNotSubscriptable";
        }
        if (matchWith(msg, "'int' object does not support item assignment")) {
            return "IntNotSubscriptable";
        }
        if (matchWith(msg, "unsupported operand type... for ([^:]*): '(.*)' and '(.*)'", 
            {{1, "operand"}, {2, "left"}, {3, "right"}})) {

            return "UnsupportedOperand";
        }
        if (matchWith(msg, "Can't convert (.+) .*to (.+) implicitly", {{1, "type1"}, {2, "type2"}})) {
            return "ConvertImplicit";
        }

    } else if (error.getType() == "RuntimeError") {

        if (matchWith(msg, "maximum recursion depth exceeded")) {
            return "MaximumRecursion";
        }
        
    } else if (error.getType() == "RecursionError") {

        if (matchWith(msg, "maximum recursion depth exceeded")) {
            return "MaximumRecursion";
        }

    }
    messageVars["stderr"] = error.stderr;
    return "";
}