#ifndef __VOYTALKINTENT_H__
#define __VOYTALKINTENT_H__

#include "cbor/Cbor.h"


enum {
    VOYTALK_INTENT = 0x400D
};

class VoytalkIntent : public CborMap
{
public:
    VoytalkIntent(const char* id, const char* endpoint, bool autoDelete = false);
    VoytalkIntent(const char* id, const char* endpoint, const char* authority, bool autoDelete = false);

    std::string getID();
    std::string getEndpoint();
    std::string getAuthority();

    static std::string getID(CborMap* map);
    static std::string getEndpoint(CborMap* map);
    static std::string getAuthority(CborMap* map);
};

#endif // __VOYTALKINTENT_H__
