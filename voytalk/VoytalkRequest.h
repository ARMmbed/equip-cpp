#ifndef __VOYTALKREQUEST_H__
#define __VOYTALKREQUEST_H__

#include "cbor/Cbor.h"


enum {
    VOYTALK_REQUEST = 0x4010
};

class VoytalkRequest : public CborMap
{
public:
    VoytalkRequest(uint32_t id, uint32_t method, const char* url) : CborMap(3)
    {
        CborMap::setTag(VOYTALK_REQUEST);
        CborMap::insert("id", id);
        CborMap::insert("method", method);
        CborMap::insert("url", url);
    }

    int32_t getID()
    {
        return VoytalkRequest::getID(this);
    }

    static int32_t getID(CborMap* map)
    {
        int32_t retval = -1;

        CborInteger* request = (CborInteger*) map->find("id");

        if (request)
        {
            retval = request->getInteger();
        }

        return retval;
    }

    int32_t getMethod()
    {
        return VoytalkRequest::getMethod(this);
    }

    static int32_t getMethod(CborMap* map)
    {
        int32_t retval = -1;

        CborInteger* request = (CborInteger*) map->find("method");

        if (request)
        {
            retval = request->getInteger();
        }

        return retval;
    }

    std::string getURL()
    {
        return VoytalkRequest::getURL(this);
    }

    static std::string getURL(CborMap* map)
    {
        std::string retval;

        CborString* request = (CborString*) map->find("url");

        if (request)
        {
            retval = request->getString();
        }

        return retval;
    }
};

#endif // __VOYTALKREQUEST_H__
