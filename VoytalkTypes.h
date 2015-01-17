#ifndef __VOYTALKTYPES_H__
#define __VOYTALKTYPES_H__


#include <string>


class VoytalkBase {
public:
    VoytalkBase();

    int getTag();

private:
    int tag;
};


class VoytalkIntent : public VoytalkBase {
public:
    VoytalkIntent();    

};

class VoytalkIntentInvocation : public VoytalkBase {
public:
    VoytalkIntentInvocation();
};

class VoytalkRequest : public VoytalkBase {
public:
    VoytalkRequest(const int method, const std::string url, const int requestId);

private:
    int method;
    std::string url;
    int requestId;
};

class VoytalkResponse : public VoytalkBase {
public:
    VoytalkResponse();
};


#endif // __VOYTALKTYPES_H__
