
#include "VoytalkRequest.h"

#include <string>

#include "Cbor.h"


typedef enum {
    FIND_KEY,
    STORE_METHOD,
    STORE_URL,
    STORE_ID
} state_t;



VoytalkRequest::VoytalkRequest(const uint8_t *cborBuffer, uint32_t size) : VoytalkBase(), CborListener()
{
    CborInput input(cborBuffer, size);
    CborReader reader(input);
    reader.SetListener(*this);
    reader.Run();
}


VoytalkRequest::VoytalkRequest(const uint8_t method, const std::string url, const uint32_t requestId) : VoytalkBase(), CborListener()
{
    this->tag = VOYTALK_REQUEST;
    this->method = method;
    this->url = url;
    this->requestId = requestId;
}

uint32_t VoytalkRequest::cborEncode(uint8_t *cborBuffer, uint32_t bufferSize)
{
    printf("encode\n");

    CborStaticOutput output(cborBuffer, bufferSize);
    CborWriter writer(output);

    writer.writeTag(tag);
    writer.writeMap(3);
    writer.writeString("method", 6);
    writer.writeInt(method);
    writer.writeString("url", 3);
    writer.writeTag(0x20);
    writer.writeString(url);
    writer.writeString("id", 2);
    writer.writeInt(requestId);

    return output.getSize();
}


/* Listener handlers */
void VoytalkRequest::onInteger(int32_t value)
{
    printf("val %d\n", value);

    if (state == STORE_METHOD)
    {
        method = value;
        state = FIND_KEY;
    }
    else if (state == STORE_ID)
    {
        requestId = value;
        state = FIND_KEY;
    }
    else
    {
        error = 1;
    }
}

void VoytalkRequest::onBytes(uint8_t *data, uint32_t size)
{
    printf("bytes %d\n", size);
}

void VoytalkRequest::onString(std::string &string)
{
    printf("string %s\n", string.c_str());

    if (state == FIND_KEY)
    {
        if (string.compare("method") == 0)
        {
            state = STORE_METHOD;
        }
        else if (string.compare("url") == 0)
        {
            state = STORE_URL;
        }
        else if (string.compare("id") == 0)
        {
            state = STORE_ID;
        }
    }
    else if (state == STORE_URL)
    {
        url = string;
        state = FIND_KEY;
    }
    else
    {
        error = 1;
    }
}

void VoytalkRequest::onArray(uint32_t size)
{
    printf("array %d\n", size);
}

void VoytalkRequest::onMap(uint32_t size)
{
    printf("map %d\n", size);
}

void VoytalkRequest::onTag(uint32_t tag)
{
    printf("tag: %d\n", tag);

    if (this->tag == -1)
    {
        this->tag = tag;
    }
}

void VoytalkRequest::onSpecial(uint32_t code)
{
    printf("error\n");
}

void VoytalkRequest::onError(const char *error)
{
    printf("error\n");
}

void VoytalkRequest::onExtraInteger(uint64_t value, int8_t sign)
{
    printf("error\n");
}

void VoytalkRequest::onExtraTag(uint64_t tag)
{
    printf("error\n");
}

void VoytalkRequest::onExtraSpecial(uint64_t tag)
{
    printf("error\n");
}


