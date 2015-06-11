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

class VoytalkResponse : public CborMap
{
public:
    VoytalkResponse(uint32_t id, uint32_t status, SharedPointer<CborBase>& body) : CborMap(3)
    {
        CborMap::setTag(VOYTALK_RESPONSE);
        CborMap::insert("id", id);
        CborMap::insert("status", status);
        CborMap::insert("body", body);
    }

    VoytalkResponse(uint32_t id, uint32_t status) : CborMap(2)
    {
        CborMap::setTag(VOYTALK_RESPONSE);
        CborMap::insert("id", id);
        CborMap::insert("status", status);
    }

    int32_t getID()
    {
        int32_t retval = -1;

        SharedPointer<CborBase> id = CborMap::find("id");

        if (id)
        {
            CborInteger* integer = static_cast<CborInteger*>(id.get());

            retval = integer->getInteger();
        }

        return retval;
    }

    int32_t getMethod()
    {
        int32_t retval = -1;

        SharedPointer<CborBase> method = CborMap::find("method");

        CborInteger* integer = static_cast<CborInteger*>(method.get());

        if (integer)
        {
            retval = integer->getInteger();
        }

        return retval;
    }

    SharedPointer<CborBase> getBody()
    {
        return CborMap::find("body");
    }

};

#endif // __VOYTALKRESPONSE_H__
