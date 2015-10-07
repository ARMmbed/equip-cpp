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

#define VERBOSE_DEBUG_OUT 1


VoytalkNext::VoytalkNext(VoytalkRoutingStack& _stack)
    : stack(_stack)
{}

void VoytalkNext::operator () (uint32_t status)
{
    stack.next(status);
}



VoytalkRoutingStack::VoytalkRoutingStack(VTRequest& _req, VTResponse& _res, std::vector<route_t>& _routes)
    : req(_req), res(_res), iter(), routes(_routes)
{
    iter = routes.begin();
}

void VoytalkRoutingStack::next(uint32_t status)
{
    if ((iter >= routes.end()) || status != 0) 
    {
        end(status ? status : 500);
        return;
    } else {
        route_t route = *iter;
        iter++;
        VoytalkNext callback(*this);
        route(req, res, callback);
    }
}

void VoytalkRoutingStack::end(uint32_t status) {
    iter = routes.end();
    res.end(status);
}



VoytalkRouter::VoytalkRouter(const char * _name)
    :   name(_name),
        stateMask(0xFFFFFFFF),
        intentVector(),
        getRoutes(),
        postRoutes()
{
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

        VoytalkRoutingStack stack(req, res, iter->second);
        // kick off middleware chain
        stack.next(0);

    } else {
        res.end(404);
    }

}


void VoytalkRouter::processCBOR(BlockStatic* input, BlockStatic* output)
{
    DEBUGOUT("hub: input buffer usage: %lu of %lu\r\n", input->getLength(), input->getMaxLength());

#if VERBOSE_DEBUG_OUT
    DEBUGOUT("hub-cbor:\r\n");
    for (size_t idx = 0; idx < input->getLength(); idx++)
    {
        DEBUGOUT("%02X", input->at(idx));
    }
    DEBUGOUT("\r\n\r\n");
#endif

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
                    VTResponse res = VTResponse(req, output->getData(), output->getMaxLength());

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

                    // set length in output block
                    output->setLength(res.getLength());
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

        DEBUGOUT("hub: output buffer usage: %lu of %lu\r\n", output->getLength(), output->getMaxLength());

#if VERBOSE_DEBUG_OUT
        /* print generated output */
        if (output->getLength() > 0)
        {
            DEBUGOUT("hub-cbor:\r\n");
            for (size_t idx = 0; idx < output->getLength(); idx++)
            {
                DEBUGOUT("%02X", output->at(idx));
            }
            DEBUGOUT("\r\n\r\n");
        }
#endif
    }
}
