#ifndef __VOYTALKBASE_H__
#define __VOYTALKBASE_H__

#include <cstdint>

#include "Cbor.h"

class VoytalkBase : public CborMap
{
public:
    VoytalkBase();
    VoytalkBase(int32_t tag);

    int32_t getTag();

    virtual bool isValid() = 0;
    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize) = 0;

protected:
    int32_t tag;
};



#endif // __VOYTALKBASE_H__

