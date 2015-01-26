
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



VoytalkRequest::VoytalkRequest(const uint8_t *cborBuffer, uint32_t size) : 
    state(EXPECT_TAG), 
    VoytalkBase(-1), CborListener()
{    
    CborInput input(cborBuffer, size);
    CborReader reader(&input, this);
    reader.run();
}


VoytalkRequest::VoytalkRequest(uint8_t method, const std::string url, uint32_t requestId) : 
    state(EXPECT_NONE), 
    method(method),
    url(url),
    requestId(requestId),
    VoytalkBase(VOYTALK_REQUEST), CborListener()
{
}

VoytalkRequest::VoytalkRequest(const CborMap* base) :
    state(EXPECT_NONE), 
    VoytalkBase(VOYTALK_REQUEST), CborListener()
{
    // 
    CborBase* baseMethod = base->find("method");

    if (baseMethod)
    {
        if (baseMethod->getType() == CBOR_TYPE_INTEGER)
        {
            CborInteger* method = (CborInteger*) baseMethod;
            this->method = method->getInteger();
        }
    }

    // 
    CborBase* baseUrl = base->find("url");

    if (baseUrl)
    {
        if (baseUrl->getType() == CBOR_TYPE_STRING)
        {
            CborString* url = (CborString*) baseUrl;
            this->url = url->getString();
        }
    }

    // 
    CborBase* baseId = base->find("id");

    if (baseId)
    {
        if (baseId->getType() == CBOR_TYPE_INTEGER)
        {
            CborInteger* id = (CborInteger*) baseId;
            this->requestId = id->getInteger();
        }
    }
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




