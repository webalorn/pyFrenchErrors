#include "utility.hpp"
#include <stdlib.h>

void LogError::log(const std::string errMsg) {
    std::cerr << "-> An error occured: " << errMsg << std::endl;
}
void LogError::logFatal(const std::string errMsg) {
    std::cerr << "-> An fatal error occured: " << errMsg << std::endl;
    std::cerr << "The program needs to stop" << std::endl;
    exit(EXIT_FAILURE);
}
