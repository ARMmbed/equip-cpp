#include "voytalk/VoytalkHub.h"

#include "cbor/Cbor.h"
#include "voytalk/Voytalk.h"

#include <stdio.h>

#ifdef DEBUG
#warning debug enabled
#define DEBUGOUT(...) { printf(__VA_ARGS__); }
#else
#define DEBUGOUT(...) /* nothing */
#endif // DEBUGOUT

#define AUTODELETE (true)

VoytalkHub::VoytalkHub(const char* _name)
    : name(_name), stateMask(0xFFFFFFFF), intentVector(), resourceMap()
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

void VoytalkHub::registerIntent(const char* intentName, intent_invocation_handler_t intentCallback, uint32_t intentState)
{
    intent_t intentStruct = { .intent = intentName,
                              .callback = intentCallback,
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

uint32_t VoytalkHub::stringToUINT32(std::string& number)
{
    std::size_t length = number.length();

    uint32_t output = 0;

    for (std::size_t idx = 0; idx < length; idx++)
    {
        uint8_t character = number[idx];

        if ((character >= 48) && (character <= 57))
        {
            output *= 10;
            output += character - 48;
        }
        else
        {
            break;
        }
    }

    return output;
}

void VoytalkHub::processCBOR(block_t* input, block_t* output)
{
    /*  Decode CBOR array into CBOR objects
    */
    CborDecoder decoder(input->data, input->length);

    /*  input and output can point to the same block.
        After being decoded, the input block is not used and can safely be overwritten.
        The output length is non-zero when the CBOR processing generated a response.
    */
    output->length = 0;

    /*  Process CBOR object assuming it is a CborMap.
        Use the CborBase tag to switch between Voytalk types.
    */
    CborMap* base = static_cast<CborMap*>(decoder.getCborBase());

    if (base)
    {
        int32_t tag = base->getTag();

        switch (tag)
        {
            case VOYTALK_REQUEST:
                {
                    DEBUGOUT("Received Voytalk Request\r\n");
#ifdef DEBUG
                    base->print();
#endif
                    DEBUGOUT("\r\n");

                    // default return code - 200 success
                    uint16_t statusCode = 200;

                    /*  Construct reply

                        A valid reply is VoytalkResponse
                        containing a VoytalkResource
                        containing an array of intents.
                    */
                    CborMap resource(2);
                    resource.insert("name", name);

                    // the path to the requested resource
                    std::string url = VoytalkRequest::getURL(base);

                    // the root resource is special and lists the available intents
                    if (url.compare("/") == 0)
                    {
                        /*  prepare intent array
                            set initial size to 1 and instruct container classes to
                            automatically delete the object when destroyed.
                        */
                        CborArray* intentArray = new CborArray(1, AUTODELETE);
//                        DEBUGOUT("new: %p\r\n", intentArray);

                        /*  Iterate over all intents in vector, but only add those which
                            bitmap matches the current stateMask.
                        */
                        for(IntentVectorType::iterator iter = intentVector.begin(); iter != intentVector.end(); ++iter)
                        {
                            if (iter->state & stateMask)
                            {
                                // endpoints are stored as uint32_t but transmitted as strings
                                char endpoint[11] = {0};
                                snprintf(endpoint, 11, "%lu", iter->endpoint);

                                // create new intent object and set it to be autodeleted by container
                                VoytalkIntent* item = new VoytalkIntent(iter->intent, endpoint, AUTODELETE);

//                                DEBUGOUT("new: %p\r\n", item);
                                intentArray->insert(item);
                            }
                        }

                        // insert intent array into VoytalkResource
                        resource.insert("intents", intentArray);
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
                                The callback is also responsible for checking whether
                                the hub is in the correct state for this resource.
                            */
                            iter->second(&resource);
                        }
                        else
                        {
                            // resource not found - return 404
                            statusCode = 404;
                        }
                    }

                    // VoytalkResponse to VoytalkRequest with the same ID
                    uint32_t requestID = VoytalkRequest::getID(base);
                    VoytalkResponse response(requestID, statusCode, &resource);

                    DEBUGOUT("Voytalk Hub Response\r\n");
                    response.print();
                    DEBUGOUT("\r\n");

                    // Serialize CBOR objects in output block
                    CborEncoder encode(&response, output->data, output->maxLength);
                    output->length = encode.getLength();
                }
                break;

            case VOYTALK_INTENTINVOCATION:
                {
                    DEBUGOUT("Received Voytalk Intent Invocation\r\n");
#ifdef DEBUG
                    base->print();
#endif
                    DEBUGOUT("\r\n");

                    // Convert endpoint string to uint32_t and use as index in array
                    std::string endpoint = VoytalkIntentInvocation::getEndpoint(base);

                    uint32_t index = stringToUINT32(endpoint);

                    if (index < intentVector.size())
                    {
                        intent_t intentStruct = intentVector[index];

                        // only allow invocation if the state matches
                        if (intentStruct.state & stateMask)
                        {
                            VoytalkIntentInvocation* object = static_cast<VoytalkIntentInvocation*>(base);

                            /*  Pass invocation object to callback function
                                since it might contain parameters.
                            */
                            intentStruct.callback(object);
                        }
                    }
                }
                break;

            default:
                DEBUGOUT("Received Unknown Voytalk Tag: %04lX\r\n", tag);
#ifdef DEBUG
                base->print();
#endif
                DEBUGOUT("\r\n");
                break;
        }
    }
}


