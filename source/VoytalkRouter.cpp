#include "voytalk/VoytalkRouter.h"

#include "cborg/Cbor.h"
#include "voytalk/Voytalk.h"

#include <stdio.h>
#include <stdarg.h>

#if 1
#warning debug enabled
#define DEBUGOUT(...) { printf(__VA_ARGS__); }
#else
#define DEBUGOUT(...) /* nothing */
#endif // DEBUGOUT


/*****************************************************************************/
/* VoytalkNext                                                               */
/*****************************************************************************/

/**
 * Create a callback functor that will dvance the state in the routing stack.
 **/
VoytalkNext::VoytalkNext(VoytalkRoutingStack* _stack)
    : stack(_stack)
{}


/**
 * Advances the state of the routing stack, triggering the next middleware
 * in the chain to get exectuted (if it exists).
 **/
void VoytalkNext::operator () (uint32_t status)
{
    if (stack)
    {
        stack->next(status);
    }
}

/*****************************************************************************/
/* VoytalkRoutingStack                                                       */
/*****************************************************************************/

/**
 * Initialises the state of the routing stack by creating an iterator on the routes
 * held by the voytalk router.
 **/
VoytalkRoutingStack::VoytalkRoutingStack(VTRequest& _req, VTResponse& _res, std::vector<route_t>& _routes)
    : req(_req), res(_res), iter(), routes(_routes)
{
    iter = routes.begin();
}

/**
 * Advances the iterator state if the status is not set (i.e. status == 0).
 * If a status is set (i.e. status != 0) moves the iterator to the end and
 * signals the response to end with the given status code.
 **/
void VoytalkRoutingStack::next(uint32_t status)
{
    if ((iter >= routes.end()) || status != 0)
    {
        iter = routes.end();
        res.end(status ? status : 500);
    } else {
        route_t route = *iter;
        iter++;
        VoytalkNext callback(this);
        route(req, res, callback);
    }
}


/*****************************************************************************/
/* VoytalkRouter                                                             */
/*****************************************************************************/

/**
 * Bridging function between object function calls and non-object ones.
 **/
static VoytalkRouter* bridge;
static void VoytalkRouterInternalOnFinished(const VTResponse& res)
{
    bridge->internalOnFinished(res);
}

VoytalkRouter::VoytalkRouter(const char * _name, VTResponse::ended_callback_t _onResponseFinished)
    :   name(_name),
        stateMask(0xFFFFFFFF),
        intentVector(),
        getRoutes(),
        postRoutes(),
        stack(NULL),
        onResponseFinished(_onResponseFinished)
{
    // save object to global variable
    bridge = this;
}

VoytalkRouter::~VoytalkRouter()
{
    delete stack;
}

void VoytalkRouter::setStateMask(uint32_t _stateMask)
{
    stateMask = _stateMask;
}

uint32_t VoytalkRouter::getStateMask() const
{
    return stateMask;
}

void VoytalkRouter::registerIntent(intent_construction_delegate_t constructionCallback,
                                uint32_t intentState)
{
    intent_t intentStruct = { .constructionCallback = constructionCallback,
                              .state = intentState };

    intentVector.push_back(intentStruct);
}

void VoytalkRouter::get(const char* endpoint, route_t route, ...)
{
    DEBUGOUT("configured route: GET %s ", endpoint);

   /**
    * Process the variadic arguments until a NULL entry is reached.
    * Each callback found is added a list of middleware for that path.
    * todo: the variadic arguments used here lead to a bad API (i.e. it
    * requres a NULL termination and isn't type safe). Ideally we'd use
    * an initializer list or templated function call here.
    **/
    std::vector<route_t> routes;
    va_list va;
    va_start(va, endpoint);
    for (route_t i = route; i; i = va_arg(va, route_t))
    {
        if (i) routes.push_back(i);
    }
    va_end(va);

    routes.push_back(route);

    DEBUGOUT(" - %u middlewares regsitered\r\n", routes.size());

    std::string endpointString = endpoint;
    std::pair<std::string, std::vector<route_t> > pair(endpointString, routes);

    getRoutes.insert(pair);
}

void VoytalkRouter::post(const char* endpoint, route_t route, ...)
{
    DEBUGOUT("configured route: POST %s ", endpoint);

   /**
    * Process the variadic arguments until a NULL entry is reached.
    * Each callback found is added a list of middleware for that path.
    * todo: the variadic arguments used here lead to a bad API (i.e. it
    * requres a NULL termination and isn't type safe). Ideally we'd use
    * an initializer list or templated function call here.
    **/
    std::vector<route_t> routes;
    va_list va;
    va_start(va, endpoint);
    for (route_t i = route; i; i = va_arg(va, route_t))
    {
        if (i) routes.push_back(i);
    }
    va_end(va);

    DEBUGOUT(" - %u middlewares regsitered\r\n", routes.size());

    std::string endpointString = endpoint;
    std::pair<std::string, std::vector<route_t> > pair(endpointString, routes);

    postRoutes.insert(pair);
}

void VoytalkRouter::homeResource(VTRequest& req, VTResponse& res) const
{
    DEBUGOUT("home resource fetched\r\n");

    /*  find size of intent array
    */
    uint32_t size = 0;

    for(IntentVectorType::const_iterator iter = intentVector.begin();
        iter != intentVector.end();
        ++iter)
    {
        if (iter->state & stateMask)
        {
            size++;
        }
    }

    /*  home resource consists of 2 fields:
        name: of device
        intents: the list of intents currently offered
    */
    res.map(2)
        .key(VTShortKeyName).value(name, strlen(name)) // todo: remove this strlen
        .key(VTShortKeyIntents).array(size);

    if (size > 0)
    {
        /*  Iterate over all intents in vector, but only add those which
            bitmap matches the current stateMask.
        */
        for(IntentVectorType::const_iterator iter = intentVector.begin();
            iter != intentVector.end();
            ++iter)
        {
            if (iter->state & stateMask)
            {
                // callback function is responsible for adding objects to encode
                iter->constructionCallback(req, res);
            }
        }
    }

    res.end(200);
}


void VoytalkRouter::route(RouteMapType& routes, VTRequest& req, VTResponse& res)
{

    DEBUGOUT("%s %s\r\n", req.getMethod() == VTRequest::GET ? "GET" : "POST", req.getURL().c_str());

    // the path to the requested resource
    std::string url = req.getURL();


    // the root resource is special and lists the available intents
    if ((req.getMethod() == VTRequest::GET) && (url.compare("/") == 0))
    {
        homeResource(req, res);
        // root resource always found
        return;
    }


    /*  Resource requested is not the root.
        Search the resource map for a matching callback function.
    */
    RouteMapType::iterator iter = routes.find(url);

    if (iter != routes.end())
    {
        /*  The callback is also responsible for checking whether
            the hub is in the correct state for this resource.
        */

        DEBUGOUT("  -> route found\r\n");

        stack = new VoytalkRoutingStack(req, res, iter->second);
        // kick off middleware chain
        // a status code of 0 isn't a valid HTTP status code, so we use this
        // to signal that execution should continue.
        // any non-zero status code here would end execution of the middleware.
        stack->next(0);

    } else {
        res.end(404);
    }

}


void VoytalkRouter::processCBOR(BlockStatic* input, BlockStatic* output)
{

    /*  Decode CBOR array into CBOR objects
    */
    Cborg decoder(input->getData(), input->getLength());

    /*  The output length is non-zero when the CBOR processing generated a response.
    */
    output->setLength(0);


    if (decoder.getType() == Cbor::TypeMap)
    {
        switch (decoder.getTag())
        {
            case VTRequest::TAG:
                {
                    DEBUGOUT("hub: received VTRequest:\r\n");
#if VERBOSE_DEBUG_OUT
                    decoder.print();
                    DEBUGOUT("\r\n");
#endif
                    // provide request object
                    VTRequest req = VTRequest(decoder);
                    // construct a response object with pointers to the output buffer
                    // also forward the response-finished callback to be executed by the response itself
                    // once the end method is called.
                    // todo: pass the output biffer as a Block object rather than pointer + length
                    VTResponse res = VTResponse(req, output, VoytalkRouterInternalOnFinished);

                    // route the request
                    switch (req.getMethod())
                    {
                        case VTRequest::GET: {
                            this->route(getRoutes, req, res);
                            break;
                        }
                        case VTRequest::POST: {
                            this->route(postRoutes, req, res);
                            break;
                        }
                        default: {
                            DEBUGOUT("hub: received unknown method in request: %04lX\r\n", req.getMethod());
                            DEBUGOUT("\r\n");
                            res.end(405);
                            break;
                        }
                    }
                }
                break;

            default:
                DEBUGOUT("hub: received unknown Voytalk Tag: %04lX\r\n", decoder.getTag());
#if VERBOSE_DEBUG_OUT
                decoder.print();
                DEBUGOUT("\r\n");
#endif
                break;
        }
    }
}

void VoytalkRouter::internalOnFinished(const VTResponse& res)
{
    // clean up stack
    delete stack;
    stack = NULL;

    // call user function
    onResponseFinished(res);
}

