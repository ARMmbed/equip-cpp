#include "VoytalkResponse.h"

#include "Cbor.h"


typedef enum {
    EXPECT_NONE,
    EXPECT_TAG
} state_t;


VoytalkResponse::VoytalkResponse(const uint8_t *cborBuffer, uint32_t size) : 
    state(EXPECT_TAG), 
    VoytalkBase(), CborListener()
{
    CborDebugListener listener;
    CborInput input(cborBuffer, size);
    CborReader reader(&input, &listener);
    reader.run();
}

VoytalkResponse::VoytalkResponse(uint32_t requestId, uint8_t status, const VoytalkBase *body) : 
    state(EXPECT_TAG),
    requestId(requestId),
    status(status),
    body(body),
    VoytalkBase(VOYTALK_RESPONSE), CborListener()
{
}

VoytalkResponse::VoytalkResponse(const CborMap* object) :
    state(EXPECT_TAG),
    VoytalkBase(VOYTALK_RESPONSE), CborListener()
{

}

VoytalkResponse::VoytalkResponse() :
    state(EXPECT_NONE),
    VoytalkBase(VOYTALK_RESPONSE), CborListener()
{
    CborMap::setTag(VOYTALK_RESPONSE);
}

bool VoytalkResponse::isValid()
{
    return !error;
}

uint32_t VoytalkResponse::cborEncode(uint8_t *cborBuffer, uint32_t bufferSize)
{
    int cborLength = 0;

    return cborLength;
}

