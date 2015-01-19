#include "VoytalkTypes.h"




VoytalkBase::VoytalkBase() : tag(-1), error(0), state(0)
{

}

uint32_t VoytalkBase::getTag()
{
    return tag;
}

uint8_t VoytalkBase::isValid()
{
    return (error == 0);
}

