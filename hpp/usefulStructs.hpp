#ifndef USEFUL_STRUCTS
#define USEFUL_STRUCTS

#include <string>
#include <vector>

class PyError;
class PyFile;


struct errorMsgParams {
    std::string mesageId;
    std::vector<std::string> params;
};

struct errorDescription {
    int errLine;
    std::vector<errorMsgParams> messages;

    void push(errorDescription);
    bool isDefined();
};

struct FctContext {
    std::vector<std::string> params;
    PyError& pyError;
    PyFile& pyCodeFile;
};

#endif
