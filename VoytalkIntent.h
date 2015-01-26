#ifndef __VOYTALKINTENT_H__
#define __VOYTALKINTENT_H__

#include "VoytalkBase.h"
#include "Cbor.h"

enum {
    VOYTALK_INTENT = 0x400D
};

class VoytalkIntent : public VoytalkBase, CborListener {
public:
    VoytalkIntent(const uint8_t *cborBuffer, uint32_t size);
    VoytalkIntent(const char* id, const char* endpoint, const char* authority);
    VoytalkIntent(const CborBase* object);

    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize);
    virtual bool isValid();

    virtual void onTag(uint32_t tag);    
    virtual void onArray(uint32_t size);
    virtual void onString(std::string &string);

    CborMap* cborObject();

    std::string id;
    std::string endpoint;
    std::string authority;

private:
    uint8_t state;
};


#endif // __VOYTALKINTENT_H__