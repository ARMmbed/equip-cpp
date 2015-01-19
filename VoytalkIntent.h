#ifndef __VOYTALKINTENT_H__
#define __VOYTALKINTENT_H__

#include "VoytalkBase.h"
#include "Cbor.h"

class VoytalkIntent : public VoytalkBase, CborListener {
public:
    VoytalkIntent();    
    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize);
};


#endif // __VOYTALKINTENT_H__