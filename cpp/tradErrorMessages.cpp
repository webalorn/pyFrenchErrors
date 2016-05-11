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

std::string TradErrorMessages::getMessage(errorMsgParams messageWithParams, std::string langage) {
    std::string messageId = messageWithParams.messageId;
    if (messages.find(langage) == messages.end())
        return "";
    if (messages[langage].find(messageId) == messages[langage].end())
        return "";
    std::string theMessage = messages[langage][messageId];
    for (int iParam = 0; iParam < (int)messageWithParams.params.size(); iParam++) {
        theMessage = std::regex_replace(theMessage, std::regex("\\{\\{" + std::to_string(iParam) + "\\}\\}"), messageWithParams.params[iParam]);
    }
    return theMessage;
}
std::string TradErrorMessages::getMessage(std::vector<errorMsgParams> messageWithParamsTab, std::string langage) {
    std::string finalMessage;
    for (errorMsgParams msg : messageWithParamsTab) {
        std::string addMsg = getMessage(msg, langage);
        if (finalMessage != "" && addMsg != "")
            finalMessage += " ";
        finalMessage += addMsg;
    }
    return finalMessage;
}
