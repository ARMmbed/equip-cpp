#include "Voytalk.h"

#include <stdio.h>
#include "Cbor.h"

Voytalk::Voytalk(void) : someint(0)
{

}

VoytalkBase* Voytalk::decodeCBOR(const char *cbor, std::size_t size)
{
    CborInput input(cbor, size);

    CborReader reader(input);
    reader.SetListener(*this);
    reader.Run();

    VoytalkRequest* object = new VoytalkRequest(1, "hello", 1);



    return object;
}

std::size_t encodeCBOR(char *cbor, std::size_t size, const VoytalkBase* object)
{
    return 0;
}


int Voytalk::some(void)
{
    return someint;
}



void Voytalk::OnInteger(int value) {
    printf("integer: %d\n", value);
}

void Voytalk::OnBytes(unsigned char *data, int size) {
    printf("bytes with size: %d", size);
}

void Voytalk::OnString(std::string &str) {
    printf("string: '%.*s'\n", (int)str.size(), str.c_str());
}

void Voytalk::OnArray(int size) {
    printf("array: %d\n", size);
}

void Voytalk::OnMap(int size) {
    printf("map: %d\n", size);
}

void Voytalk::OnTag(unsigned int tag) {
    printf("tag: %d\n", tag);
}

void Voytalk::OnSpecial(unsigned int code) {
    printf("special: %d\n", code);
}

void Voytalk::OnError(const char *error) {
    printf("error: %s\n", error);
}

void Voytalk::OnExtraInteger(unsigned long long value, int sign) {
    if(sign >= 0) {
        printf("extra integer: %llu\n", value);
    } else {
        printf("extra integer: -%llu\n", value);
    }
}

void Voytalk::OnExtraTag(unsigned long long tag) {
    printf("extra tag: %llu\n", tag);
}

void Voytalk::OnExtraSpecial(unsigned long long tag) {
    printf("extra special: %llu\n", tag);
}











