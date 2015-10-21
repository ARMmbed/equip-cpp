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

/**
 * Known parameters reprent a set of possible parameters that a clien tpay want to use 
 * when invoking an action. You can think of these as auto-fill fields that you might
 * find when filling in a form in a web browser. They are intended to provide the user
 * with a fast way to fill out forms with known or pre-entered values. The app may cache
 * values (and porbably will) provided via known parameters.
 **/
class VoytalkKnownParameters
{
public:

	VoytalkKnownParameters(Cbore& _encoder)
		: encoder(_encoder)
    {}

    Cbore& begin()
    {
        return encoder.array();
    }

    Cbore& parameter(uint32_t priority)
    {
    	return encoder.map(2)
    		.key(VTShortKeyPriority).value(priority)
    		.key(VTShortKeyValue);
    }

    template <size_t I>
    Cbore& parameter(const char (&action)[I], uint32_t priority)
    {
    	return encoder.map(3)
    		.key(VTShortKeyAction).value(action, I-1)
    		.key(VTShortKeyPriority).value(priority)
    		.key(VTShortKeyValue);
    }

    void end()
    {
        encoder.end();
    }

private:
	Cbore& encoder;
};

#endif // __VOYTALKKNOWNPARAMETERS_H__