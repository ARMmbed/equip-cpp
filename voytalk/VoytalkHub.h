#ifndef __VOYTALKHUB_H__
#define __VOYTALKHUB_H__

#include <stdint.h>

class VoytalkHub
{
public:
    VoytalkHub();

    void processCBOR(const uint8_t* data, uint32_t length);
};


#endif // __VOYTALKHUB_H__
