#ifndef parseErrorOutput
#define parseErrorOutput

#include <vector>
#include <string>
#include "pyCode.hpp"
#include "pyFile.hpp"

class PyError {
public:
    PyError(std::vector<std::string>);
    int getLine();
    int getChar();
    std::string getType();
    std::string getMessage();
    PyCode getCode();
private:
    std::string typeOfError, detailedMessage, pyCode;
    int lineOfError; // from 1 to...
    int charOfError; // from 0 to...

    void parseErrorLastLine(std::string);
    void parseBlockContentError(std::vector<std::string>);

public:
    static const int undefinedPosition = -1;
};

#endif
