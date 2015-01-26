#include "VoytalkTypes.h"




VoytalkBase::VoytalkBase() : tag(-1), CborMap()
{

}

VoytalkBase::VoytalkBase(int32_t tag) : tag(tag), CborMap()
{

}

int32_t VoytalkBase::getTag()
{
    return tag;
}



