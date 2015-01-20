
#include "VoytalkRequest.h"

#include <string>

#include "Cbor.h"


typedef enum {
    EXPECT_TAG,
    EXPECT_ARRAY,
    EXPECT_METHOD,
    EXPECT_URL,
    EXPECT_ID,
    EXPECT_NONE
} state_t;



VoytalkRequest::VoytalkRequest(const uint8_t *cborBuffer, uint32_t size) : state(EXPECT_TAG), VoytalkBase(), CborListener()
{    
    CborInput input(cborBuffer, size);
    CborReader reader(&input, this);
    reader.run();
}


VoytalkRequest::VoytalkRequest(const uint8_t method, const std::string url, const uint32_t requestId) : state(EXPECT_TAG), VoytalkBase(), CborListener()
{
    this->tag = VOYTALK_REQUEST;
    this->method = method;
    this->url = url;
    this->requestId = requestId;
}

bool VoytalkRequest::isValid()
{
    return !error;
}

uint32_t VoytalkRequest::cborEncode(uint8_t *cborBuffer, uint32_t bufferSize)
{
    printf("encode\n");

    CborStaticOutput output(cborBuffer, bufferSize);
    CborWriter writer(&output);

    writer.writeTag(tag);
    writer.writeArray(3);
    writer.writeInt(method);
    writer.writeString(url);
    writer.writeInt(requestId);

    return output.getSize();
}


/* Listener handlers */
void VoytalkRequest::onInteger(int32_t value)
{
    if (state == EXPECT_METHOD)
    {
        method = value;
        state = EXPECT_URL;
    }
    else if (state == EXPECT_ID)
    {
        requestId = value;
        state = EXPECT_NONE;
    }
    else
    {
        error = 1;
    }
}

void VoytalkRequest::onString(std::string &string)
{
    if (state == EXPECT_URL)
    {
        url = string;
        state = EXPECT_ID;
    }
    else
    {
        error = 1;
    }
}

void VoytalkRequest::onArray(uint32_t size)
{
    if ((state == EXPECT_ARRAY) && (size == 3))
    {
        state = EXPECT_METHOD;
    }
    else
    {
        error = 1;
    }
}

void VoytalkRequest::onTag(uint32_t tag)
{
    if ((this->tag == -1) && (tag == VOYTALK_REQUEST) && (state == EXPECT_TAG))
    {
        this->tag = tag;
        state = EXPECT_ARRAY;
    }
    else
    {
        error = 1;
    }
}




