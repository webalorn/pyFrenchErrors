#include "meaningTreeFcts.hpp"
#include <iostream>

std::map<std::string, std::function<bool(FctContext)>> getBoolTreeFcts() {
    return {
        {"errorLine_openBracket > 0", [](FctContext context){
            return context.pyCodeFile.getNbOpenBrackets(context.pyError.getLineNumber()) > 0;
        }},
        {"code.countOpenCloseBrackets != 0", [](FctContext context) {
            return context.pyError.getPyLine().countOpenCloseBrackets() != 0;
        }},
        {"code.isConditianal", [](FctContext context) {
            return context.pyError.getPyLine().isConditianal();
        }},
        {"NOT code.endByTwoPoints", [](FctContext context) {
            return !context.pyError.getPyLine().endByTwoPoints();
        }},
        {"code.isStruct", [](FctContext context) {
            return context.pyError.getPyLine().isStruct();
        }}
    };
}

std::map<std::string, std::function<std::string(FctContext)>> getStringTreeFcts() {
    return {
        {"getLineFirstBracketOfError", [](FctContext context){
            return std::to_string(context.pyCodeFile.getLineOpenFirstBracket(context.pyError.getLineNumber()));
        }},
        {"code.getFirstWord", [](FctContext context) {
            return context.pyError.getPyLine().getFirstWord();
        }}
    };
}
