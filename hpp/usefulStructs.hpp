#ifndef USEFUL_STRUCTS
#define USEFUL_STRUCTS

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

#endif
