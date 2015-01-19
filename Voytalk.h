#ifndef __VOYTALK_H__
#define __VOYTALK_H__


#include "VoytalkTypes.h"
#include "Cbor.h"

class Voytalk {
public:
    static VoytalkBase* decodeCBOR(const uint8_t *cborBuffer, uint32_t bufferSize);
};



#endif // __VOYTALK_H__

