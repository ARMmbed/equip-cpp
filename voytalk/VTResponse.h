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


#ifndef __VOYTALKRESPONSE_H__
#define __VOYTALKRESPONSE_H__

#include "cbor/Cbor.h"

enum {
    VOYTALK_RESPONSE = 0x4011
};

class VTResponse : public CborEncoder
{
public:

    VTResponse(VTRequest& _req)
        : req(_req), headerLength(0)
    {
    }

    void begin() {
        // set tag to response type
        writeTag(VOYTALK_RESPONSE);

        /*  Voytalk response consists of 3 fields.
            id:     the request this reply is for
            body:   reply or null
            status: of the request
        */
        writeMap(3);

        // the id to request that's we're responding to
        uint32_t requestID = req.getID();
        addKeyValue("id", requestID);

        // this is where the hard work is done
        // the object tree that represents the resource
        // is serialised out to the CBOR encoder
        addKey("body");

        headerLength = getLength();
    }

    void end(uint32_t status) {
        // check that a body was actually written
        if (getLength() == headerLength) {
            // if not then send an empty string as the body
            writeString("", 0); // todo: should this be NULL?
        }

        // insert the status code for the request
        addKeyValue("status", status);
    }

    void write(VTResource& resource) {
        resource.encodeCBOR(*this);
    }

private: 
    VTRequest& req;
    size_t headerLength;
};


#endif // __VOYTALKRESPONSE_H__
