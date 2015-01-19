#ifndef __VOYTALKINTENTINVOCATION_H__
#define __VOYTALKINTENTINVOCATION_H__

#include "VoytalkBase.h"
#include "Cbor.h"

class VoytalkIntentInvocation : public VoytalkBase, CborListener {
public:
    VoytalkIntentInvocation();
    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize);
};


#endif // __VOYTALKINTENTINVOCATION_H__
