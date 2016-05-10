#include "tradErrorMessages.hpp"

TradErrorMessages::TradErrorMessages(std::ifstream dataFile) {
    nlohmann::json translationsJSON;
    dataFile >> translationsJSON;
    for (nlohmann::json::iterator itLang = translationsJSON.begin(); itLang != translationsJSON.end(); ++itLang) {
        for (nlohmann::json::iterator itTranslate = itLang.value().begin(); itTranslate != itLang.value().end(); ++itTranslate) {
            messages[itLang.key()][itTranslate.key()] = itTranslate.value();
        }
    }
}
