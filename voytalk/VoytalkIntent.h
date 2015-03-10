#ifndef __VOYTALKINTENT_H__
#define __VOYTALKINTENT_H__

#include "cbor/CborMap.h"
#include "cbor/CborString.h"

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

        CborString* url = new CborString(endpoint, true);
        url->setTag(32);

        CborMap::insert("endpoint", url);
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
        return VoytalkIntent::getID(this);
    }

    static std::string getID(CborMap* map)
    {
        std::string retval;

        CborString* intent = (CborString*) map->find("id");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }

    std::string getEndpoint()
    {
        return VoytalkIntent::getEndpoint(this);
    }

    static std::string getEndpoint(CborMap* map)
    {
        std::string retval;

        CborString* intent = (CborString*) map->find("endpoint");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }

    std::string getAuthority()
    {
        return VoytalkIntent::getAuthority(this);
    }

    static std::string getAuthority(CborMap* map)
    {
        std::string retval;

        CborString* intent = (CborString*) map->find("authority");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }
};

#endif // __VOYTALKINTENT_H__
