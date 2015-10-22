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


#ifndef __EQUIP_RESPONSE_H__
#define __EQUIP_RESPONSE_H__

#include "cborg/Cbor.h"
#include "mbed-block/BlockStatic.h"

namespace Equip {

/**
 * An encapulation of a response to or from a client, responses have at least a status code
 * and likely a body. This should be a familar concept to thos that have used HTTP.
 **/
class Response : public Cbore
{
public:

    enum {
        TAG = 0x4011
    };

    typedef void (*ended_callback_t)(const Response& res);

    Response(Request& _req, BlockStatic* _block, ended_callback_t _ended)
        : Cbore(_block->getData(), _block->getMaxLength()), req(_req), block(_block), ended(_ended)
    {
        begin();
    }

    void begin() {
        // Response consists of 3 fields.
        //  id:     the request this reply is for
        //  body:   reply or null
        //  status: of the request

        // set tag to response type
        tag(Response::TAG)
            .map(3)
                .key(ShortKeyId).value(req.getID())
                .key(ShortKeyBody);

        // this is where the hard work is done
        // the object tree that represents the resource
        // is serialised out to the CBOR encoder

        // update current block length
        block->setLength(getLength());
    }

    void end(uint32_t status) {
        // check that a body was actually written
        // the length of the buffer is saved in the begin() call
        // if it's not grown by the time end() is called then
        // no body was written by the middleware chain
        // so we should fill in a NULL body so the cbor parses correctly
        if (getLength() == block->getLength()) {
            // if not then send an empty string as the body
            value(Cbor::TypeNull);
        }

        // insert the status code for the request
        key(ShortKeyStatus).value(status);

        // update block length with length from cbor encoder
        block->setLength(getLength());

        // if an callback has been registered for handling ended event, it gets
        // triggered here
        if (ended) ended(*this);
    }

    /**
     * A helper method for serialising resouces
     * to response object.
     **/
    void write(Resource& resource) {
        resource.encodeCBOR(*this);
    }

private:
    Request req;
    BlockStatic* block;
    ended_callback_t ended;
};

}

#endif // __EQUIP_RESPONSE_H__
