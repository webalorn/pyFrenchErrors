#include "usefulStructs.hpp"
#include <iostream>

void errorDescription::push(errorDescription other) {
    for (auto x : other.messages)
        messages.push_back(x);
}
bool errorDescription::isDefined() {
    return !messages.empty();
}
