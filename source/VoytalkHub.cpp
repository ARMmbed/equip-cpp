#include "voytalk/VoytalkHub.h"


#if 1
#include <stdio.h>
#define DEBUG(...) { printf(__VA_ARGS__); }
#else
#define DEBUG(...) /* nothing */
#endif // NDEBUG


VoytalkHub::VoytalkHub()
{

}

void VoytalkHub::processCBOR(const uint8_t* data, uint32_t length)
{
    DEBUG("blockServerWriteDone: %d %s\n\r", length, data);

}