#include "voytalk/VoytalkHub.h"

#include "cbor/Cbor.h"
#include "voytalk/Voytalk.h"

#include <stdio.h>

#ifndef NDEBUG
#warning debug enabled
#define DEBUGOUT(...) { printf(__VA_ARGS__); }
#else
#define DEBUGOUT(...) /* nothing */
#endif // DEBUGOUT



VoytalkHub::VoytalkHub(const char* _name)
    :   name(_name),
        stateMask(0xFFFFFFFF),
        intentVector(),
        resourceMap(),
        cborEncoder(),
        endpointCache(0xFFFFFFFF),
        callbackToken(false)
{

}

void VoytalkHub::setStateMask(uint32_t _stateMask)
{
    stateMask = _stateMask;
}

uint32_t VoytalkHub::getStateMask()
{
    return stateMask;
}

void VoytalkHub::registerIntent(intent_construction_handler_t constructionCallback,
                                intent_invocation_handler_t invocationCallback,
                                uint32_t intentState)
{
    intent_t intentStruct = { .constructionCallback = constructionCallback,
                              .invocationCallback = invocationCallback,
                              .endpoint = intentVector.size(),
                              .state = intentState };

    intentVector.push_back(intentStruct);
}

void VoytalkHub::registerResourceCallback(const char* endpoint, resource_handler_t resourceCallback)
{
    std::string endpointString = endpoint;
    std::pair<std::string, resource_handler_t> pair(endpointString, resourceCallback);

    resourceMap.insert(pair);
}

void VoytalkHub::processIntent(VoytalkIntent& intent)
{
    if (callbackToken)
    {
        callbackToken = false;
        intent.setEndpoint(endpointCache);
        intent.encodeCBOR(cborEncoder);
    }
}

void VoytalkHub::processCoda(VoytalkCoda& coda)
{
    if (callbackToken)
    {
        callbackToken = false;
        coda.encodeCBOR(cborEncoder);
    }
}

void VoytalkHub::processResource(VoytalkResource& resource)
{
    if (callbackToken)
    {
        callbackToken = false;

        // warning: untested
        // serialize Voytalk object to CBOR
        cborEncoder.addKey("resource");
        resource.encodeCBORObject(cborEncoder);
    }
}


uint32_t VoytalkHub::stringToUINT32(std::string& number)
{
    std::size_t length = number.length();

    bool found = false;
    uint32_t output = 0;

    for (std::size_t idx = 0; idx < length; idx++)
    {
        uint8_t character = number[idx];

        // skip characters that are not numbers
        if ((character >= 48) && (character <= 57))
        {
            output *= 10;
            output += character - 48;

            found = true;
        }
    }

    return (found) ? output : 0xFFFFFFFF;
}


uint16_t VoytalkHub::handleGET(VoytalkRequest* baseRequest)
{
    // add body
    cborEncoder.addKey("body");

    /*  body consists of 2 fields:
        name: of device
        the resource
    */
    cborEncoder.writeMap(2);
    cborEncoder.addKeyValue("name", name);

    /******************************************************
        Insert resources
    */

    // the path to the requested resource
    std::string url = baseRequest->getURL();

    // the root resource is special and lists the available intents
    if (url.compare("/") == 0)
    {
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
        cborEncoder.addKey("intents");
        cborEncoder.writeArray(size);

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
                    // cache intent endpoint - will be used in processIntent
                    endpointCache = iter->endpoint;

                    // enable write in callback function
                    callbackToken = true;

                    // callback function is responsible for adding objects to encode
                    iter->constructionCallback(*this);
                }
            }
        }

        // root resource always found
        return 200;
    }
    else
    {
        /*  Resource requested is not the root.
            Search the resource map for a matching callback function.
        */
        ResourceMapType::const_iterator iter = resourceMap.find(url);

        if (iter != resourceMap.end())
        {
            /*  Callback inserts resource
            */

            // enable write in callback
            callbackToken = true;

            /*  The callback is also responsible for checking whether
                the hub is in the correct state for this resource.
            */
            iter->second(*this);

            // resource found
            return 200;
        }
    }
    
    return 404;
}

uint16_t VoytalkHub::handlePOST(VoytalkRequest* baseRequest)
{
    SharedPointer<CborBase> baseBody = baseRequest->getBody();

    cbor_type_t type = baseBody->getType();

    if (type != CBOR_TYPE_MAP) return 400;

    // add body
    cborEncoder.addKey("body");
    
    // provide invocation object
    VoytalkIntentInvocation* baseInvocation = static_cast<VoytalkIntentInvocation*>(baseBody.get());
    
    // Convert endpoint string to uint32_t and use as index in array
    std::string endpoint = baseRequest->getURL();
    uint32_t index = stringToUINT32(endpoint);

    if (index < intentVector.size())
    {
        intent_t intentStruct = intentVector[index];

        // only allow invocation if the state matches
        if (intentStruct.state & stateMask)
        {
            // enable write in callback
            callbackToken = true;

            /*  Pass invocation object to callback function
                since it might contain parameters.
            */
            intentStruct.invocationCallback(*this, *baseInvocation);
        }
    }

    return 200;
}

void VoytalkHub::processCBOR(block_t* input, block_t* output)
{
    DEBUGOUT("hub: input buffer usage: %lu of %lu\r\n", input->length, input->maxLength);
    DEBUGOUT("hub-cbor:\r\n");
    for (size_t idx = 0; idx < input->length; idx++)
    {
        DEBUGOUT("%02X", input->data[idx]);
    }
    DEBUGOUT("\r\n\r\n");

    /*  Decode CBOR array into CBOR objects
    */
    CborDecoder decoder(input->data, input->length);

    /*  The output length is non-zero when the CBOR processing generated a response.
    */
    output->length = 0;

    /*  Process CBOR object assuming it is a CborMap.
    */
    SharedPointer<CborBase> baseObject = decoder.getCborBase();

    cbor_type_t type = baseObject->getType();

    if (type == CBOR_TYPE_MAP)
    {
        CborMap* base = static_cast<CborMap*>(baseObject.get());

        if (base)
        {
            /*  Use the CborBase tag to switch between Voytalk types.
            */
            int32_t tag = base->getTag();

            switch (tag)
            {
                case VOYTALK_REQUEST:
                    {
                        DEBUGOUT("hub: received VoytalkRequest:\r\n");
                        base->print();
                        DEBUGOUT("\r\n");

                        // provide request object
                        VoytalkRequest* baseRequest = static_cast<VoytalkRequest*>(base);

                        // default return code - 404 resource not found
                        uint16_t statusCode = 404;

                        /*  Construct reply

                            A valid reply is VoytalkResponse
                            containing a VoytalkResource
                            containing an array of intents.
                        */
                        cborEncoder.setBuffer(output->data, output->maxLength);

                        // set tag to response type
                        cborEncoder.writeTag(VOYTALK_RESPONSE);

                        /*  Voytalk response consists of 3 fields.
                            id:     the request this reply is for
                            body:   reply
                            status: of the request
                        */
                        cborEncoder.writeMap(3);

                        switch (baseRequest->getMethod())
                        {
                            case VOYTALK_GET: {
                                statusCode = this->handleGET(baseRequest);
                                break;
                            }
                            case VOYTALK_POST: {
                                statusCode = this->handlePOST(baseRequest);
                                break;
                            }
                            default: {   
                                DEBUGOUT("hub: received unknown method in request: %04lX\r\n", baseRequest->getMethod());
                                DEBUGOUT("\r\n");
                                break;
                            }
                        }


                        // VoytalkResponse to VoytalkRequest with the same ID
                        uint32_t requestID = baseRequest->getID();
                        DEBUGOUT("request id:%lu\r\n", requestID);
                        cborEncoder.addKeyValue("id", requestID);

                        // insert the status code for the request
                        cborEncoder.addKeyValue("status", statusCode);

                        // set length in output block
                        output->length = cborEncoder.getLength();
                    }
                    break;

                default:
                    DEBUGOUT("hub: received unknown Voytalk Tag: %04lX\r\n", tag);
                    base->print();
                    DEBUGOUT("\r\n");
                    break;
            }

            /* print generated output */
            if (output->length > 0)
            {
                DEBUGOUT("hub: output buffer usage: %lu of %lu\r\n", output->length, output->maxLength);
                DEBUGOUT("hub-cbor:\r\n");
                for (size_t idx = 0; idx < output->length; idx++)
                {
                    DEBUGOUT("%02X", output->data[idx]);
                }
                DEBUGOUT("\r\n\r\n");
            }
        }
    }
}


