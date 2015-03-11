#ifndef __VOYTALKCODA_H__
#define __VOYTALKCODA_H__

#include "cbor/Cbor.h"


enum {
    VOYTALK_CODA = 0x400F
};

class VoytalkCoda : public CborMap
{
public:
    VoytalkCoda(uint32_t id, uint32_t success) : CborMap(2)
    {
        CborMap::setTag(VOYTALK_CODA);
        CborMap::insert("invocation", id);
        CborMap::insert("success", success);
    }
};

#endif // __VOYTALKCODA_H__