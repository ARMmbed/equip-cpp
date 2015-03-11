#ifndef __VOYTALKINTENTINVOCATION_H__
#define __VOYTALKINTENTINVOCATION_H__

#include "cbor/Cbor.h"


enum {
    VOYTALK_INTENTINVOCATION = 0x400E
};

class VoytalkIntentInvocation : public CborMap
{
public:

    std::string getEndpoint()
    {
        return VoytalkIntentInvocation::getEndpoint(this);
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

    int32_t getID()
    {
        return VoytalkIntentInvocation::getID(this);
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

    std::string getAction()
    {
        return VoytalkIntentInvocation::getAction(this);
    }

    static std::string getAction(CborMap* map)
    {
        std::string retval;

        CborString* intent = (CborString*) map->find("action");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }

    CborBase* getParameters()
    {
        return VoytalkIntentInvocation::getParameters(this);
    }

    static CborBase* getParameters(CborMap* map)
    {
        return (CborBase*) map->find("parameters");
    }

};

#endif // __VOYTALKINTENTINVOCATION_H__
