#ifndef __VOYTALKBASE_H__
#define __VOYTALKBASE_H__

#include <cstdint>

class VoytalkBase {
public:
    VoytalkBase();

    uint32_t getTag();
    uint8_t isValid();

    virtual uint32_t cborEncode(uint8_t *cborBuffer, uint32_t bufferSize) = 0;

protected:
    uint32_t tag;
    uint8_t error;
    uint8_t state;
};



#endif // __VOYTALKBASE_H__

