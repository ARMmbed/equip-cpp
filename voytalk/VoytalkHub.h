#ifndef __VOYTALKHUB_H__
#define __VOYTALKHUB_H__

#include <stdint.h>

#include "voytalk/Block.h"
#include "voytalk/Voytalk.h"


// intent invocation callback type
typedef void (* intent_invocation_handler_t)(VoytalkIntentInvocation* object);

// resource request callback type
typedef void (* resource_handler_t)(VoytalkResource* object);

// internal data structure for tracking intents
typedef struct {
    const char* intent;
    intent_invocation_handler_t callback;
    uint32_t endpoint;
    uint32_t state;
} intent_t;

// vector containing all registered intents
typedef std::vector<intent_t> IntentVectorType;

// map containing all registered resources
typedef std::map<std::string, resource_handler_t> ResourceMapType;

class VoytalkHub
{
public:
    /*  Name is automatically added to responses.
    */
    VoytalkHub(const char* name = "VoytalkHub");

    /*  Register intent in the hub.

        Intent names are strings, e.g., com.arm.intent,
        intentCallback is called when the intent is invoked,
        and the intentState is a bitmap for grouping intents together in different states.
    */
    void registerIntent(const char* intentName, intent_invocation_handler_t intentCallback, uint32_t intentState = 0xFFFFFFFF);

    /*  Register resource in the hub.

        Resources have an endpoint, e.g., /list, and a callback function
        responsible for generating the resource and inserting it in the
        resource map.
    */
    void registerResourceCallback(const char* endpoint, resource_handler_t resourceCallback);

    /*  Converts CBOR byte arrays into CBOR objects and processes the Voytalk request.

        The input block contains the byte array to be processd and the output block the
        response from the Voytalk request (if any).

        For memory optimization, the input block can be used as the output block if the
        application is done with the data in the input block.
    */
    void processCBOR(block_t* input, block_t* output);

    /*  Set and read the bit mask used to match intents.

        The state mask is AND'ed with each intent's bitmap
        and any intents with a non-zero mask is valid.
    */
    void setStateMask(uint32_t newState);
    uint32_t getStateMask();

private:
    uint32_t stringToUINT32(std::string& number);

    const char* name;
    uint32_t stateMask;
    IntentVectorType intentVector;
    ResourceMapType resourceMap;
};


#endif // __VOYTALKHUB_H__
