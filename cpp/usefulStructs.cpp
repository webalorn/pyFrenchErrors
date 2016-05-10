#include "usefulStructs.hpp"


void errorDescription::push(errorDescription other) {
    messages.insert(messages.begin(), other.messages.begin(), other.messages.end());
}
bool errorDescription::isDefined() {
    return !messages.empty();
}
