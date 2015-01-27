#ifndef __VOYTALKINTENT_H__
#define __VOYTALKINTENT_H__

#include "CborMap.h"

enum {
    VOYTALK_INTENT = 0x400D
};

class VoytalkIntent : public CborMap
{
public:
    VoytalkIntent(const char* id, const char* endpoint) : CborMap(2)
    {
        CborMap::setTag(VOYTALK_INTENT);
        CborMap::insert("id", id);
        CborMap::insert("endpoint", endpoint);
    }

    VoytalkIntent(const char* id, const char* endpoint, const char* authority) : CborMap(3)
    {
        CborMap::setTag(VOYTALK_INTENT);
        CborMap::insert("id", id);
        CborMap::insert("endpoint", endpoint);
        CborMap::insert("authority", authority);
    }

    std::string getID()
    {
        std::string retval;

        CborString* intent = (CborString*) CborMap::find("id");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }

    std::string getEndpoint()
    {
        std::string retval;

        CborString* intent = (CborString*) CborMap::find("endpoint");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }

    std::string getAuthority()
    {
        std::string retval;

        CborString* intent = (CborString*) CborMap::find("authority");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }
};

#endif // __VOYTALKINTENT_H__
