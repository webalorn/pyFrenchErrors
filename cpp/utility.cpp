#include "utility.hpp"
#include <stdlib.h>
#include <sstream>

void LogError::log(const std::string errMsg) {
    std::cerr << "-> An error occured: " << errMsg << std::endl;
}
void LogError::logFatal(const std::string errMsg) {
    std::cerr << "-> An fatal error occured: " << errMsg << std::endl;
    std::cerr << "The program needs to stop" << std::endl;
    exit(EXIT_FAILURE);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
