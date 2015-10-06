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

#include "cborg/Cbor.h"

class VoytalkKnownParameters
{
public:

	VoytalkKnownParameters(Cbore& _encoder, std::size_t _size)
		: encoder(_encoder)
    {
    	encoder.array(_size);
    }

    Cbore& parameter(uint32_t priority)
    {
    	return encoder.map(2)
    		.key("priority").value(priority)
    		.key("value");
    }

    template <size_t I>
    Cbore& parameter(const char (&action)[I], uint32_t priority)
    {
    	return encoder.map(3)
    		.key("action").value(action, I-1)
    		.key("priority").value(priority)
    		.key("value");
    }

private:
	Cbore& encoder;
};

#endif // __VOYTALKKNOWNPARAMETERS_H__