#ifndef __VOYTALKRESPONSE_H__
#define __VOYTALKRESPONSE_H__

#include "CborMap.h"

enum {
    VOYTALK_RESPONSE = 0x4011
};

class VoytalkResponse : public CborMap
{
public:
    VoytalkResponse(uint32_t id, uint32_t status, CborBase* body) : CborMap(3)
    {
        CborMap::setTag(VOYTALK_RESPONSE);
        CborMap::insert("id", id);
        CborMap::insert("status", status);
        CborMap::insert("body", body);
    }

    int32_t getID()
    {
        int32_t retval = -1;

        CborInteger* response = (CborInteger*) CborMap::find("id");

        if (response)
        {
            retval = response->getInteger();
        }

        return retval;
    }

    int32_t getMethod()
    {
        int32_t retval = -1;

        CborInteger* response = (CborInteger*) CborMap::find("status");

        if (response)
        {
            retval = response->getInteger();
        }

        return retval;
    }

    CborBase* getBody()
    {
        return (CborBase*) CborMap::find("body");
    }

};

#endif // __VOYTALKRESPONSE_H__
