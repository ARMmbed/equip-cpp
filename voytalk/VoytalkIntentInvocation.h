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

#ifndef __VOYTALKINTENTINVOCATION_H__
#define __VOYTALKINTENTINVOCATION_H__

#include "cbor/Cbor.h"


enum {
    VOYTALK_INTENTINVOCATION = 0x400E
};

class VoytalkIntentInvocation : public CborMap
{
public:

    int32_t getID()
    {
        int32_t retval = -1;

        SharedPointer<CborBase> id = CborMap::find("id");

        CborInteger* integer = static_cast<CborInteger*>(id.get());

        if (integer)
        {
            retval = integer->getInteger();
        }

        return retval;
    }


    std::string getAction()
    {
        std::string retval;

        SharedPointer<CborBase> action = CborMap::find("action");

        CborString* actionPointer = static_cast<CborString*>(action.get());

        if (actionPointer)
        {
            retval = actionPointer->getString();
        }

        return retval;
    }

    SharedPointer<CborBase> getParameters()
    {
        return CborMap::find("parameters");
    }

};

#endif // __VOYTALKINTENTINVOCATION_H__
