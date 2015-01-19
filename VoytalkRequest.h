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

    void onInteger(int32_t value);
    void onBytes(uint8_t *data, uint32_t size);
    void onString(std::string &string);
    void onArray(uint32_t size);
    void onMap(uint32_t size);
    void onTag(uint32_t tag);
    void onSpecial(uint32_t code);
    void onError(const char *error);

    void onExtraInteger(uint64_t value, int8_t sign);
    void onExtraTag(uint64_t tag);
    void onExtraSpecial(uint64_t tag);

    int method;
    std::string url;
    int requestId;
private:

};


#endif // __VOYTALKREQUEST_H__
