#ifndef __VOYTALKINTENTINVOCATION_H__
#define __VOYTALKINTENTINVOCATION_H__

#include "CborMap.h"
#include "CborString.h"

enum {
    VOYTALK_INTENTINVOCATION = 0x400E
};

class VoytalkIntentInvocation : public CborMap
{
public:

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

    std::string getAction()
    {
        std::string retval;

        CborString* intent = (CborString*) CborMap::find("action");

        if (intent)
        {
            retval = intent->getString();
        }

        return retval;
    }

    CborBase* getParameters()
    {
        return (CborBase*) CborMap::find("parameters");
    }

};

#endif // __VOYTALKINTENTINVOCATION_H__
