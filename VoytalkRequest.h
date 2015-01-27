#ifndef __VOYTALKREQUEST_H__
#define __VOYTALKREQUEST_H__

#include "CborMap.h"
#include "CborInteger.h"
#include "CborString.h"

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
        int32_t retval = -1;

        CborInteger* request = (CborInteger*) CborMap::find("id");

        if (request)
        {
            retval = request->getInteger();
        }

        return retval;
    }

    int32_t getMethod()
    {
        int32_t retval = -1;

        CborInteger* request = (CborInteger*) CborMap::find("method");

        if (request)
        {
            retval = request->getInteger();
        }

        return retval;
    }

    std::string getURL()
    {
        std::string retval;

        CborString* request = (CborString*) CborMap::find("url");

        if (request)
        {
            retval = request->getString();
        }

        return retval;
    }
};

#endif // __VOYTALKREQUEST_H__
