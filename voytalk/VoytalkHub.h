/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __VOYTALKHUB_H__
#define __VOYTALKHUB_H__

#include <stdint.h>
#include <vector>
#include <map>

#include "voytalk/Voytalk.h"



class VoytalkHub
{
public:
    // intent construction callback type
    typedef void (*intent_construction_handler_t)(VoytalkHub& hub);

    // intent invocation callback type
    typedef void (*intent_invocation_handler_t)(VoytalkHub& hub, VoytalkIntentInvocation& object);

    // resource request callback type
    typedef void (*resource_handler_t)(VoytalkHub& hub);

    // internal data structure for tracking intents
    typedef struct {
        intent_construction_handler_t constructionCallback;
        intent_invocation_handler_t invocationCallback;
        uint32_t endpoint;
        uint32_t state;
    } intent_t;

    // vector containing all registered intents
    typedef std::vector<intent_t> IntentVectorType;

    // map containing all registered resources
    typedef std::map<std::string, resource_handler_t> ResourceMapType;

    /*  Name is automatically added to responses.
    */
    VoytalkHub(const char* name = "VoytalkHub");

    /*  Register intent in the hub.

        Intent names are strings, e.g., com.arm.intent,
        intentCallback is called when the intent is invoked,
        and the intentState is a bitmap for grouping intents together in different states.
    */
    void registerIntent(intent_construction_handler_t constructionCallback,
                        intent_invocation_handler_t invocationCallback,
                        uint32_t intentState = 0xFFFFFFFF);

    /*  Register resource in the hub.

        Resources have an endpoint, e.g., /list, and a callback function
        responsible for generating the resource and inserting it in the
        resource map.
    */
    void registerResourceCallback(const char* endpoint, resource_handler_t resourceCallback);

    /*  Converts CBOR byte arrays into CBOR objects and processes the Voytalk request.

        The input block contains the byte array to be processed and the output block the
        response from the Voytalk request (if any).
    */
    void processCBOR(SharedPointer<Block>& input, BlockStatic* output);

    /*  Set and read the bit mask used to match intents.

        The state mask is AND'ed with each intent's bitmap
        and any intents with a non-zero mask is valid.
    */
    void setStateMask(uint32_t newState);
    uint32_t getStateMask();


    void processIntent(VoytalkIntent& intent);
    void processCoda(VoytalkCoda& code);
    void processResource(VoytalkResource& resource);

private:
    uint32_t stringToUINT32(std::string& number);

    uint16_t handleGET(VoytalkRequest* baseRequest);
    uint16_t handlePOST(VoytalkRequest* baseRequest);

    const char* name;
    uint32_t stateMask;
    IntentVectorType intentVector;
    ResourceMapType resourceMap;

    CborEncoder cborEncoder;
    uint32_t endpointCache;
    bool callbackToken;
};


#endif // __VOYTALKHUB_H__
