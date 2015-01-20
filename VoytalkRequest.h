#ifndef __VOYTALKREQUEST_H__
#define __VOYTALKREQUEST_H__

#include "VoytalkBase.h"
#include "Cbor.h"



enum {
    VOYTALK_REQUEST = 0x4010
};


class VoytalkRequest : public VoytalkBase, CborListener {
public:
    VoytalkRequest(const uint8_t *cborBuffer, uint32_t size);
    VoytalkRequest(const uint8_t method, const std::string url, const uint32_t requestId);

    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize);
    virtual bool isValid();

    virtual void onInteger(int32_t value);
    virtual void onString(std::string &string);
    virtual void onArray(uint32_t size);
    virtual void onTag(uint32_t tag);

    uint8_t method;
    std::string url;
    uint32_t requestId;
private:
    uint8_t state;
};


#endif // __VOYTALKREQUEST_H__
