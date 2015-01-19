#ifndef __VOYTALKRESPONSE_H__
#define __VOYTALKRESPONSE_H__

#include "VoytalkBase.h"
#include "Cbor.h"

class VoytalkResponse : public VoytalkBase, CborListener {
public:
    VoytalkResponse();
    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize);
};


#endif // __VOYTALKRESPONSE_H__
