#ifndef parseErrorOutput
#define parseErrorOutput

#include <vector>
#include <string>
#include "pyLine.hpp"
#include "pyFile.hpp"
#include "utility.hpp"

class PyError {
public:
    std::string stderr;

    PyError(std::vector<std::string>);
    int getLineNumber();
    int getCharPos();
    std::string getType();
    std::string getMessage();
    PyLine getPyLine();
    std::string getDisplayableStderr();
private:
    std::string typeOfError, detailedMessage, pyLine;
    int lineOfError; // from 0 to...
    int charOfError; // from 0 to...

    void parseErrorLastLine(std::string);
    void parseTraceback(std::vector<std::string>);

public:
    static const int undefinedPosition = -1;
};

#endif
