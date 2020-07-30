#ifndef TRAD_ERROR_MESSAGES
#define TRAD_ERROR_MESSAGES

#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <regex>
#include "json.hpp"

#include "errorParser.hpp"
#include "utility.hpp"

class TradErrorMessages { 
private:
    std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> messages;
public:
    TradErrorMessages(std::ifstream&);
    void setMessage(ParsedError*, std::string, std::string);
};

#endif
