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

#ifndef __VOYTALKROUTER_H__
#define __VOYTALKROUTER_H__

#include <stdint.h>
#include <vector>
#include <map>

#include "voytalk/Voytalk.h"
#include "mbed-block/BlockStatic.h"

class VoytalkNext;

typedef void (*route_t)(VTRequest& req, VTResponse& res, VoytalkNext& next);


class VoytalkRoutingStack 
{
public:
    VoytalkRoutingStack(VTRequest& _req, VTResponse& _res, std::vector<route_t>& _routes);

    void next(uint32_t status);

    void end(uint32_t status);

private:
    VTRequest& req;
    VTResponse& res;
    std::vector<route_t>::iterator iter;
    std::vector<route_t>& routes;
};



class VoytalkRouter
{
public:
    typedef VoytalkNext& next_t;

    typedef void (*intent_construction_delegate_t)(VTRequest& req, VTResponse& res);

    // internal data structure for tracking intents
    typedef struct {
        intent_construction_delegate_t constructionCallback;
        uint32_t state;
    } intent_t;

    // vector containing all registered intents
    typedef std::vector<intent_t> IntentVectorType;

    // map containing registered routes
    typedef std::map<std::string, std::vector<route_t> > RouteMapType;

    /*  Name is automatically added to home resource.
    */
    VoytalkRouter(const char *name = "VoytalkRouter");

    /*  Register intent in the hub.

        Intent names are strings, e.g., com.arm.intent,
        intentCallback is called when the intent is invoked,
        and the intentState is a bitmap for grouping intents together in different states.
    */
    void registerIntent(intent_construction_delegate_t constructionCallback,
                        uint32_t intentState = 0xFFFFFFFF);

    /*  Register routes in the router.

        Resources have an endpoint, e.g., /list, and a callback function
        responsible for generating the resource and inserting it in the
        resource map.
    */
    void get(const char* endpoint, route_t callback, ...);
    void post(const char* endpoint, route_t callback, ...);
    
    /*  Converts CBOR byte arrays into CBOR objects and processes the Voytalk request.

        The input block contains the byte array to be processed and the output block the
        response from the Voytalk request (if any).
    */
    void processCBOR(BlockStatic* input, BlockStatic* output);

    /*  Set and read the bit mask used to match intents.

        The state mask is AND'ed with each intent's bitmap
        and any intents with a non-zero mask is valid.
    */
    void setStateMask(uint32_t newState);
    uint32_t getStateMask() const;


private:
    void route(RouteMapType& routes, VTRequest& req, VTResponse& res);
    void homeResource(VTRequest& req, VTResponse& res) const;
    
    const char* name;
    uint32_t stateMask;
    IntentVectorType intentVector;
    RouteMapType getRoutes;
    RouteMapType postRoutes;
};



class VoytalkNext
{
public:
    VoytalkNext(VoytalkRoutingStack& _stack);
    void operator () (uint32_t status = 0);

private:
    VoytalkRoutingStack& stack;
};

#endif // __VOYTALKROUTER_H__
