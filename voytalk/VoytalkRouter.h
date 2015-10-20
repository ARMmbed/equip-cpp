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



/**
 * The VoytalkRouter is the main API object exposed by the library; it is where routes
 * get associated with the middleware that handles them. It's main job is to store this
 * mapping between route path (e.g. /my/aweosme/resource) and a middleware function
 * (e.g. processAwesomeResource(req, res, next).
 * It is also responsible for handling the incoming data from the lower layer (BLE) and
 * parsing this into the request object.
 **/
class VoytalkRouter
{
public:
    class Next;

    /**
    * Expose the middleware next signature as a member of this API type.
    **/
    typedef Next next_t;

    /**
     * The middleware signature - this is the core of the framework. Each
     * piece of middleware gets passed a reference to a request and response
     * object, on which they should perform their actions. The response can
     * be written to (using a cbor encoder API plus some helper methods defined
     * on the request object).
     *
     * If at any point the response is complete, or an error occurrs such that
     * the currently executing middleware deems that no further middleware should
     * be run, it can call next(uint32_t status) to end the response with the given
     * status code. Status codes should follow HTTP status codes where possible.
     *
     * Once the middleware has finished executing, and wants to pass control on to
     * the next middleware, it should call next() with no arguments.
     *
     * Failure to call either next() or next(uint32_t status) will cause bad
     * things to happen, so you should ensure that either of these is caled
     * exactly once per execution of the middleware.
     *
     **/
    typedef void (*route_t)(VTRequest& req, VTResponse& res, next_t& next);


    /**
     * Intents are constructed lazily as they are required by the router to save memory. This
     * is the signature for the construction callback.
     **/
    typedef void (*intent_construction_delegate_t)(VTRequest& req, VTResponse& res);

private:
    /**
     * The routing stack represents the chain of middleware that will be executed for
     * each route (e.g. GET /my/cool/resource). This is not a class that should be
     * constructed or used outside of the Voytalk library.
     * This class is responsible for storing and tracking the state of a particular
     * middleware execution, (i.e. what should happen when next() is called).
     **/
    class VoytalkRoutingStack
    {
    public:
        VoytalkRoutingStack(VTRequest& _req, VTResponse& _res, std::vector<route_t>& _routes);
        void next(uint32_t status);
    private:
        VTRequest req;
        VTResponse res;
        std::vector<route_t>::iterator iter;
        std::vector<route_t>& routes;
    };


    // internal data structure for tracking intents
    typedef struct {
        intent_construction_delegate_t constructionCallback;
        uint32_t state;
    } intent_t;

    // vector containing all registered intents
    typedef std::vector<intent_t> IntentVectorType;

    // map containing registered routes
    typedef std::map<std::string, std::vector<route_t> > RouteMapType;

public:
    /**
     * A functor that allows for a bound next() or next(uint32_t status) function
     * to be passed into callback - this class should only be constructed by the library itself.
     **/
    class Next
    {

    public:
        Next(VoytalkRouter::VoytalkRoutingStack* _stack = NULL);
        void operator () (uint32_t status = 0);

    private:
        VoytalkRoutingStack* stack;
    };

    /**
     * Name is automatically added to home resource.
     * @param name The name of the device.
     * @param onResponseFinsihed A callback triggered each time a response completes.
     **/
    VoytalkRouter(const char *name = "VoytalkRouter", VTResponse::ended_callback_t onResponseFinished = NULL);

    ~VoytalkRouter();

    /**
     * Register intents with the router.
     *
     * Intent names are strings, e.g., com.arm.intent,
     * @param constructionCallback A function that is called when the router needs an intent object.
     * @param intentState A bitmap for grouping intents together in different states
    */
    void registerIntent(intent_construction_delegate_t constructionCallback,
                        uint32_t intentState = 0xFFFFFFFF);

    /**
     * Register routes in the router.
     *
     * Resources have an endpoint, e.g., /list, and a callback function
     * responsible for generating the resource and inserting it in the
     * resource map.
     *
     * @param endpoint The path to the resource
     * @param callback A number of middleware function to handle the route
     **/
    void get(const char* endpoint, route_t callback, ...);
    void post(const char* endpoint, route_t callback, ...);

    /**
     * Converts CBOR byte arrays into CBOR objects and processes the Voytalk request.
     * The input block contains the byte array to be processed and the output block the
     * response from the Voytalk request (if any).
     **/
    void processCBOR(BlockStatic* input, BlockStatic* output);

    /**
     * Set and read the bit mask used to match intents.
     * The state mask is AND'ed with each intent's bitmap and any intents with a
     * non-zero mask is valid.
     **/
    void setStateMask(uint32_t newState);
    uint32_t getStateMask() const;

    /**
     * Function for cleaning up after intent invocation callback chain has finished.
     **/
    void internalOnFinished(const VTResponse& res);

private:
    /**
     * Trigger a route. This will construct the routing stack to track the
     * state of the router invocation.
     **/
    void route(RouteMapType& routes, VTRequest& req, VTResponse& res);

    /**
     * The home resource (/) is a special case that the router handles directly.
     * This is the location where a client can discover the list of intents the
     * router is currently servicing.
     **/
    void homeResource(VTRequest& req, VTResponse& res) const;

    const char* name;
    uint32_t stateMask;
    IntentVectorType intentVector;
    RouteMapType getRoutes;
    RouteMapType postRoutes;
    VoytalkRoutingStack* stack;
    VTResponse::ended_callback_t onResponseFinished;
};

typedef VoytalkRouter::route_t route_t;

#endif // __VOYTALKROUTER_H__
