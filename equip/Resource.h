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

#ifndef __EQUIP_RESOURCE_H__
#define __EQUIP_RESOURCE_H__

#include "cborg/Cbor.h"

namespace Equip {

/**
 * Abstract base class for CBOR-encodable resources.
 **/
class Resource
{
public:

    virtual void encodeCBOR(Cbore& encoder) const;

};

}

#endif // __EQUIP_RESOURCE_H__
