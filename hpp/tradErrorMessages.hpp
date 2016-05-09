#ifndef TRAD_ERROR_MESSAGES
#define TRAD_ERROR_MESSAGES

#include <map>
#include <ifstream>
#include <string>
#include <vector>
#include <utility>

class TradErrorMessages {
private:
    std::map<std::string, std::string>
public:
    TradErrorMessages(std::ifstream::ifstream);
    std::string getMessage(std::pair<std::string, std::vector<std::string>>);
    std::string getMessage(vector<std::pair<std::string, std::vector<std::string>>>);
};

#endif
