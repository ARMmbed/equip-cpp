#ifndef __VOYTALKRESPONSE_H__
#define __VOYTALKRESPONSE_H__

#include "VoytalkBase.h"
#include "Cbor.h"

enum {
    VOYTALK_RESPONSE = 0x4011
};

class VoytalkResponse : public VoytalkBase, CborListener {
public:
    VoytalkResponse(const uint8_t *cborBuffer, uint32_t size);
    VoytalkResponse(uint32_t requestId, uint8_t status, const VoytalkBase *body);
    VoytalkResponse(const CborMap* object);
    VoytalkResponse();

    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize);
    virtual bool isValid();
    
    uint32_t requestId;
    uint8_t status;
    const VoytalkBase *body;

private:
    uint8_t state;
};


#endif // __VOYTALKRESPONSE_H__
