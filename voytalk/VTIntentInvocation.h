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

#include "cborg/Cbor.h"

class VTIntentInvocation
{
public:

    enum {
        TAG = 0x400E
    };

    VTIntentInvocation(Cborg _decoder)
        : decoder(_decoder)
    {
    }

    int32_t getID()
    {
        uint32_t retval;
        if (decoder.find("id").getUnsigned(&retval))
            return retval;
        else return 0;
    }

    std::string getAction()
    {
        std::string retval;
        decoder.find("action").getString(retval);
        return retval;
    }

    Cborg getParameters()
    {
        return decoder.find("parameters");
    }

private:
    Cborg decoder;

};

#endif // __VOYTALKINTENTINVOCATION_H__
