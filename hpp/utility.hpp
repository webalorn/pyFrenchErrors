#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>

class LogError {
public:
    static void log(const std::string);
    static void logFatal(const std::string);
};

#endif
