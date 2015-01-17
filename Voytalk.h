#ifndef __VOYTALK_H__
#define __VOYTALK_H__

#include <cstddef>

#include "VoytalkTypes.h"
#include "Cbor.h"

class Voytalk : public CborListener {

public:

    Voytalk(void);

    VoytalkBase* decodeCBOR(const char *cbor, std::size_t size);
    std::size_t encodeCBOR(char *cbor, std::size_t size, const VoytalkBase* object);

    virtual void OnInteger(int value);
    virtual void OnBytes(unsigned char *data, int size);
    virtual void OnString(std::string &str);
    virtual void OnArray(int size);
    virtual void OnMap(int size);
    virtual void OnTag(unsigned int tag);
    virtual void OnSpecial(unsigned int code);
    virtual void OnError(const char *error);

    virtual void OnExtraInteger(unsigned long long value, int sign);
    virtual void OnExtraTag(unsigned long long tag);
    virtual void OnExtraSpecial(unsigned long long tag);

private:

    int some(void);

private:

    int someint;
};



#endif // __VOYTALK_H__

