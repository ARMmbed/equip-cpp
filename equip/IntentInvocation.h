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

#ifndef __EQUIP_INTENTINVOCATION_H__
#define __EQUIP_INTENTINVOCATION_H__

#include "cborg/Cbor.h"

namespace Equip {

/**
 * An Invocation enapsulates a request to perform an action, along with the parameters 
 * for the action.
 **/
class IntentInvocation
{
public:

    enum {
        TAG = 0x400E
    };

    IntentInvocation(Cborg _decoder)
        : decoder(_decoder)
    {
    }

    int32_t getID() // const - todo: encoder methods should be const
    {
        uint32_t retval;
        if (decoder.find(ShortKeyId).getUnsigned(&retval))
            return retval;
        else return 0;
    }

    std::string getAction() // const - todo: encoder methods should be const
    {
        std::string retval;
        decoder.find(ShortKeyAction).getString(retval);
        return retval;
    }

    Cborg getParameters() // const - todo: encoder methods should be const
    {
        return decoder.find(ShortKeyParameters);
    }

private:
    Cborg decoder;

};

}

#endif // __EQUIP_INTENTINVOCATION_H__
