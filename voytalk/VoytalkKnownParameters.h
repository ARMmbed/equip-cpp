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

#ifndef __VOYTALKKNOWNPARAMETERS_H__
#define __VOYTALKKNOWNPARAMETERS_H__

#include "cbor/CborEncoder.h"

class VoytalkKnownParameters : public CborMap
{
public:
    VoytalkKnownParameters(const char* action, SharedPointer<CborBase> &value, float priority)
        :   CborMap(3)
    {
        CborMap::insert("action", 6, action, strlen(action));
        CborMap::insert("value", value);
        CborMap::insert("priority", (uint32_t)8, (uint32_t)1);
    }
};

#endif // __VOYTALKKNOWNPARAMETERS_H__