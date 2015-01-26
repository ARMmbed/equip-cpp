#include "Voytalk.h"

#include <stdio.h>

#include "VoytalkTypes.h"

#include "Cbor.h"
#include "CborPrint.h"

VoytalkBase* Voytalk::decodeCBOR(const uint8_t *cborBuffer, uint32_t bufferSize)
{
    VoytalkBase* returnObject = NULL;

    CborInput input(cborBuffer, bufferSize);
    CborObjectListener listener;
    CborReader reader(&input, &listener);

    reader.run();
    CborBase* base = (CborBase*) listener.getRoot();

    if (base)
    {
        CborPrint::printBase(base, 0);  

        switch(base->getTag())
        {
            case VOYTALK_REQUEST:
                returnObject = new VoytalkRequest((CborMap*) base);
                break;
            case VOYTALK_RESPONSE:
                returnObject = new VoytalkResponse((CborMap*) base);
                break;
            case VOYTALK_INTENT:
                returnObject = new VoytalkIntent((CborMap*) base);
            default:
                break;
        }
    }

    delete base;

    // program flow
    // cbortag = CborReader.getFirstTag(const char* cbor, size_t size);
    //
    // switch(cborTag)
    //  case VoytalkRequestTag:
    //      VoytalkBase* returnObject = new VoytalkRequest(const char* cbor, size_t size);
    // if (returnObject != NULL)
    //      success
    // return returnObject

    // change cbor-calls to static function calls
    // CborReader::decode(const char* cborBuffer, std::size_t size, CborListener* this);
    // int tag = CborReader::getFirstTag(const char* cborBuffer, std::size_t size);

#if 0
    CborInput input(cbor, size);

    CborReader reader(input);
    reader.SetListener(*this);
    reader.Run();

    VoytalkRequest* object = new VoytalkRequest(1, "hello", 1);
#endif



    return returnObject;
}










