#include "usefulStructs.hpp"


void errorDescription::push(errorDescription other) {
    messages.insert(messages.begin(), other.begin(), other.end());
}
bool errorDescription::isDefined() {
    return !messages.empty();
}
