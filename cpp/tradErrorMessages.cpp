#include "tradErrorMessages.hpp"
#include "utility.hpp"

TradErrorMessages::TradErrorMessages(std::ifstream& dataFile) {
    nlohmann::json translationsJSON;
    try {
        dataFile >> translationsJSON;
    } catch (const std::invalid_argument& ia) {
        LogError::logFatal("Construction TradErrorMessages, fichier JSON invalide: " + std::string(ia.what()));
    }
    for (nlohmann::json::iterator itLang = translationsJSON.begin(); itLang != translationsJSON.end(); ++itLang) {
        for (nlohmann::json::iterator itTarget = itLang.value().begin(); itTarget != itLang.value().end(); ++itTarget) {
            for (nlohmann::json::iterator itTranslate = itTarget.value().begin(); itTranslate != itTarget.value().end(); ++itTranslate) {
                for (std::string key : split(itTranslate.key(), '|')) {
                    messages[itLang.key()][itTarget.key()][key] = itTranslate.value();
                }
            }
        }
    }
}

void TradErrorMessages::setMessage(ParsedError* parsedErr, std::string langage, std::string target) {
    std::string messageId = parsedErr->messageId;
    if (messageId == "") {
        messageId = "default";
    }
    if (messages.find(langage) == messages.end()) {
        LogError::log("TradErrorMessages::getMessage: langage " + langage + " inconnu");
        return;
    }
    if (messages[langage].find(target) == messages[langage].end()) {
        LogError::log("TradErrorMessages::getMessage: target " + target + " inconnue dans le langage : " + langage);
        return;
    }
    if (messages[langage][target].find(messageId) == messages[langage][target].end()) {
        LogError::log("TradErrorMessages::getMessage: messageId " + messageId + " inconnu dans le langage : " + langage);
        return;
    }
    std::string theMessage = messages[langage][target][messageId];
    for (auto& var : parsedErr->vars) {
        std::string keyTemplate = "\\{\\{" + var.first + "\\}\\}";
        theMessage = std::regex_replace(theMessage, std::regex(keyTemplate), var.second);
    }
    parsedErr->text = theMessage;
}