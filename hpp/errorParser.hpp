#ifndef ERROR_PARSER_HPP
#define ERROR_PARSER_HPP

#include "pyError.hpp"
#include "pyFile.hpp"
#include "json.hpp"

#include <string>
#include <utility>
#include <regex>
#include <map>

class ParsedError {
public:
    int line;
    std::vector<std::string> blockIds;
    std::string messageId;
    std::string text;
    std::string errorLineText;
    std::map<std::string, std::string> vars;
    PyError* fromError;

    bool isDefined();
    nlohmann::json toJson();
    ParsedError(PyError& e, PyFile& fromFile, std::string msg="", std::map<std::string, std::string> varsMap={}, int iLine=-1);
};

class ErrorParser {
private:
    PyError error;
    PyFile codeFile;

    int errorLine;
    std::map<std::string, std::string> messageVars;

    std::string parseGetMessageId();
    std::string parseGetErrorName();
    bool matchWith(std::string text, std::string regString, std::vector<std::pair<int, std::string>> extractVars={});

public:
    ErrorParser(PyError&, PyFile&);
    ParsedError parse();
};

#endif
