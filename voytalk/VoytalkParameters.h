#ifndef __VOYTALKPARAMETERS_H__
#define __VOYTALKPARAMETERS_H__

#include "cbor/Cbor.h"


enum {
    VOYTALK_PARAMETERS = 0x406E
};

class VoytalkParameters : public CborMap
{

};

#endif // __VOYTALKPARAMETERS_H__
