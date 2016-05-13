#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <vector>

class LogError {
public:
    static void log(const std::string);
    static void logFatal(const std::string);
};

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif
