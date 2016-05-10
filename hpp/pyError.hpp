#ifndef parseErrorOutput
#define parseErrorOutput

#include <vector>
#include <string>
#include "pyLine.hpp"
#include "pyFile.hpp"

class PyError {
public:
    PyError(std::vector<std::string>);
    int getLineNumber();
    int getChar();
    std::string getType();
    std::string getMessage();
    PyLine getPyLine();
private:
    std::string typeOfError, detailedMessage, pyLine;
    int lineOfError; // from 0 to...
    int charOfError; // from 0 to...

    void parseErrorLastLine(std::string);
    void parseBlockContentError(std::vector<std::string>);

public:
    static const int undefinedPosition = -1;
};

#endif