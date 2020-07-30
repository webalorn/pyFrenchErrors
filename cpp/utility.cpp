#include "utility.hpp"
#include <stdlib.h>
#include <sstream>

void LogError::log(const std::string errMsg) {
    std::cerr << "-> An error occured : " << errMsg << std::endl;
}
void LogError::logFatal(const std::string errMsg) {
    std::cerr << "-> An fatal error occured : " << errMsg << std::endl;
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

template<class T> bool isIn(T val, std::set<T> values) {
    return values.find(val) != values.end();
}

template bool isIn<std::string>(std::string, std::set<std::string>);

std::string strJoin(std::vector<std::string>& vec, const char* delim) {
    std::stringstream res;
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(res, delim));
    return res.str();
}