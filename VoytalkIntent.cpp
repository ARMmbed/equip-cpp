#include "VoytalkIntent.h"

typedef enum {
    EXPECT_TAG,
    EXPECT_ARRAY,
    EXPECT_ID,
    EXPECT_ENDPOINT,
    EXPECT_AUTHORITY,
    EXPECT_NONE
} state_t;


VoytalkIntent::VoytalkIntent(const uint8_t *cborBuffer, uint32_t size) : 
    state(EXPECT_TAG), 
    VoytalkBase(), CborListener() 
{
    CborInput input(cborBuffer, size);
    CborReader reader(&input, this);
    reader.run();
}

VoytalkIntent::VoytalkIntent(const char* id, const char* endpoint, const char* authority) :
    state(EXPECT_NONE), 
    VoytalkBase(VOYTALK_INTENT), CborListener() 
{

    if (id)
    {
        this->id = std::string(id);
    }

    if (endpoint)
    {
        this->endpoint = std::string(endpoint);
    }

    if (authority)
    {
        this->authority = std::string(authority);
    }
}

VoytalkIntent::VoytalkIntent(const CborBase* object) :
    state(EXPECT_NONE), 
    VoytalkBase(VOYTALK_INTENT), CborListener() 
{
    if (object->getType() == CBOR_TYPE_MAP)
    {
        CborMap* map = (CborMap*) object;

        CborString* id = (CborString*) map->find("id");
        CborString* endpoint = (CborString*) map->find("endpoint");
        CborString* authority = (CborString*) map->find("authority");

        this->id = id->getString();
        this->endpoint = endpoint->getString();

        if (authority)
        {
            this->authority = authority->getString();
        }
    }
}


uint32_t VoytalkIntent::cborEncode(uint8_t *cborBuffer, uint32_t bufferSize)
{
    CborStaticOutput output(cborBuffer, bufferSize);
    CborWriter writer(&output);

    writer.writeTag(tag);
/*
    if (authority.size() > 0)
    {
        writer.writeArray(3);
        writer.writeString(id);
        writer.writeString(endpoint);
        writer.writeString(authority);        
    }
    else
    {
        writer.writeArray(2);
        writer.writeString(id);
        writer.writeString(endpoint);
    }
*/
    return output.getSize();
}

CborMap* VoytalkIntent::cborObject()
{
    CborMap* map;

    if (authority.size() == 0)
    {
        map = new CborMap(2);

        map->setTag(VOYTALK_INTENT);

        CborString* idValue = new CborString(id);
        CborString* endpointValue = new CborString(endpoint);

        map->insert("id", idValue);
        map->insert("endpoint", endpointValue);
    }
    else
    {
        map = new CborMap(3);

        map->setTag(VOYTALK_INTENT);

        CborString* idValue = new CborString(id);
        CborString* endpointValue = new CborString(endpoint);
        CborString* authorityValue = new CborString(authority);

        map->insert("id", idValue);
        map->insert("endpoint", endpointValue);
        map->insert("authority", authorityValue);
    }

    return map;
}


bool VoytalkIntent::isValid()
{
    return !error;
}

void VoytalkIntent::onTag(uint32_t tag)
{
    if ((state == EXPECT_TAG) && (tag == VOYTALK_INTENT))
    {
        this->tag = tag;
        state = EXPECT_ARRAY;
    }
    else
    {
        error = 1;
    }
}

void VoytalkIntent::onArray(uint32_t size)
{
    if ((state == EXPECT_ARRAY) && ((size == 2) || (size == 3)))
    {
        state = EXPECT_ID;
    }
    else
    {
        error = 1;
    }
}

void VoytalkIntent::onString(std::string &string)
{
    if (state == EXPECT_ID)
    {
        this->id = string;
        state = EXPECT_ENDPOINT;
    }
    else if (state == EXPECT_ENDPOINT)
    {
        this->endpoint = string;
        state = EXPECT_AUTHORITY;
    }
    else if (state == EXPECT_AUTHORITY)
    {
        this->authority = string;
        state = EXPECT_NONE;
    }
    else
    {
        error = 1;
    }
}


