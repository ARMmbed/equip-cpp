#include "voytalk/VoytalkRouter.h"

#include "cbor/Cbor.h"
#include "voytalk/Voytalk.h"

#include <stdio.h>

#if 1
#warning debug enabled
#define DEBUGOUT(...) { printf(__VA_ARGS__); }
#else
#define DEBUGOUT(...) /* nothing */
#endif // DEBUGOUT

#define VERBOSE_DEBUG_OUT 1

VoytalkRouter::VoytalkRouter(const char* _name)
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

uint32_t VoytalkRouter::getStateMask()
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

void VoytalkRouter::get(const char* endpoint, route_t route)
{

    DEBUGOUT("configured route: GET %s\r\n", endpoint);

    std::string endpointString = endpoint;
    std::pair<std::string, route_t> pair(endpointString, route);
    
    getRoutes.insert(pair);
}

void VoytalkRouter::post(const char* endpoint, route_t route)
{
    DEBUGOUT("configured route: POST %s\r\n", endpoint);

    std::string endpointString = endpoint;
    std::pair<std::string, route_t> pair(endpointString, route);
    
    postRoutes.insert(pair);
}

void VoytalkRouter::homeResource(VTRequest& req, VTResponse& res)
{
    DEBUGOUT("home resource fetched\r\n");

    /*  home resource consists of 2 fields:
        name: of device
        intents: the list of intents currently offered
    */
    res.writeMap(2);

    res.addKeyValue("name", name);

    /*  find size of intent array
    */
    uint32_t size = 0;

    for(IntentVectorType::iterator iter = intentVector.begin();
        iter != intentVector.end();
        ++iter)
    {
        if (iter->state & stateMask)
        {
            size++;
        }
    }

    /*  insert intent array into VoytalkResource
    */
    res.addKey("intents");
    res.writeArray(size);

    if (size > 0)
    {

        /*  Iterate over all intents in vector, but only add those which
            bitmap matches the current stateMask.
        */
        for(IntentVectorType::iterator iter = intentVector.begin();
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

    DEBUGOUT("%s %s\r\n", req.getMethod() == VOYTALK_GET ? "GET" : "POST", req.getURL().c_str());

    // the path to the requested resource
    std::string url = req.getURL();


    // the root resource is special and lists the available intents
    if ((req.getMethod() == VOYTALK_GET) && (url.compare("/") == 0))
    {
        homeResource(req, res);
        // root resource always found
        return;
    }


    /*  Resource requested is not the root.
        Search the resource map for a matching callback function.
    */
    RouteMapType::const_iterator iter = routes.find(url);

    if (iter != routes.end())
    {
        /*  The callback is also responsible for checking whether
            the hub is in the correct state for this resource.
        */

        DEBUGOUT("  -> route found\r\n");
        done_t callback(&res, &VTResponse::end);
        iter->second(req, res, callback);

        return;
    }

    res.end(404);
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
    CborDecoder decoder(input);

    /*  The output length is non-zero when the CBOR processing generated a response.
    */
    output->setLength(0);

    /*  Process CBOR object assuming it is a CborMap.
    */
    SharedPointer<CborBase> baseObject = decoder.getCborBase();

    if (baseObject)
    {
        CborBase::cbor_type_t type = baseObject->getType();

        if (type == CborBase::TYPE_MAP)
        {
            CborMap* base = static_cast<CborMap*>(baseObject.get());

            /*  Use the CborBase tag to switch between Voytalk types.
            */
            int32_t tag = base->getTag();

            switch (tag)
            {
                case VOYTALK_REQUEST:
                    {
                        DEBUGOUT("hub: received VTRequest:\r\n");
#if VERBOSE_DEBUG_OUT
                        base->print();
                        DEBUGOUT("\r\n");
#endif
                        // provide request object
                        VTRequest req = VTRequest(base);
                        VTResponse res = VTResponse(req);

                        /*  Construct reply

                            A valid reply is VoytalkResponse
                            containing a VoytalkResource
                            containing an array of intents.
                        */
                        res.setBuffer(output->getData(), output->getMaxLength());

                        res.begin();

                        // route the request
                        switch (req.getMethod())
                        {
                            case VOYTALK_GET: {
                                this->route(getRoutes, req, res);
                                break;
                            }
                            case VOYTALK_POST: {
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
                    DEBUGOUT("hub: received unknown Voytalk Tag: %04lX\r\n", tag);
#if VERBOSE_DEBUG_OUT
                    base->print();
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
}

