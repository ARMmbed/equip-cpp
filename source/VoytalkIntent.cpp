#include "voytalk/VoytalkIntent.h"


VoytalkIntent::VoytalkIntent(const char* id, const char* endpoint, bool autoDelete)
    : CborMap(2, autoDelete)
{
    CborMap::setTag(VOYTALK_INTENT);
    CborMap::insert("id", id);

    CborString* url = new CborString(endpoint, true);
    url->setTag(32);

    CborMap::insert("endpoint", url);
}

VoytalkIntent::VoytalkIntent(const char* id, const char* endpoint, const char* authority, bool autoDelete)
    : CborMap(3, autoDelete)
{
    CborMap::setTag(VOYTALK_INTENT);
    CborMap::insert("id", id);
    CborMap::insert("endpoint", endpoint);
    CborMap::insert("authority", authority);
}

std::string VoytalkIntent::getID()
{
    return VoytalkIntent::getID(this);
}

std::string VoytalkIntent::getEndpoint()
{
    return VoytalkIntent::getEndpoint(this);
}

std::string VoytalkIntent::getAuthority()
{
    return VoytalkIntent::getAuthority(this);
}

std::string VoytalkIntent::getID(CborMap* map)
{
    std::string retval;

    CborString* intent = (CborString*) map->find("id");

    if (intent)
    {
        retval = intent->getString();
    }

    return retval;
}

std::string VoytalkIntent::getEndpoint(CborMap* map)
{
    std::string retval;

    CborString* intent = (CborString*) map->find("endpoint");

    if (intent)
    {
        retval = intent->getString();
    }

    return retval;
}

std::string VoytalkIntent::getAuthority(CborMap* map)
{
    std::string retval;

    CborString* intent = (CborString*) map->find("authority");

    if (intent)
    {
        retval = intent->getString();
    }

    return retval;
}

