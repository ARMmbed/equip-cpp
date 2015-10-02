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

#ifndef __VOYTALKREQUEST_H__
#define __VOYTALKREQUEST_H__

#include "cbor/Cbor.h"


enum {
    VOYTALK_REQUEST = 0x4010
};

typedef enum {
    VOYTALK_GET,
    VOYTALK_PUT,
    VOYTALK_POST
} method_t;

class VTRequest
{
public:

    VTRequest(CborMap* _map)
        : cborMap(_map)
    {
    }

    int32_t getID()
    {
        int32_t retval = -1;

        SharedPointer<CborBase> id = cborMap->find("id");

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

        SharedPointer<CborBase> method = cborMap->find("method");

        CborInteger* integer = static_cast<CborInteger*>(method.get());

        if (integer)
        {
            retval = integer->getInteger();
        }

        return retval;
    }

    std::string getURL()
    {
        std::string retval;

        SharedPointer<CborBase> url = cborMap->find("url");

        CborString* stringPointer = static_cast<CborString*>(url.get());

        if (stringPointer)
        {
            retval = stringPointer->getString();
        }

        return retval;
    }

    SharedPointer<CborBase> getBody()
    {
        SharedPointer<CborBase> body = cborMap->find("body");
        return body;
    }


private:
    CborMap * cborMap;

};

#endif // __VOYTALKREQUEST_H__
