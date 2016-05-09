#ifndef TRAD_ERROR_MESSAGES
#define TRAD_ERROR_MESSAGES

#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

class TradErrorMessages {
private:
    std::map<std::string, std::map<std::string, std::string>> messages;
public:
    TradErrorMessages(std::ifstream);
    std::string getMessage(std::pair<std::string, std::vector<std::string>> messageWithParams, std::string langage);
    std::string getMessage(std::vector<std::pair<std::string, std::vector<std::string>>> messageWithParamsTab, std::string langage);
};

#endif
