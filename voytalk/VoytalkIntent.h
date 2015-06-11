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


#ifndef __VOYTALKINTENT_H__
#define __VOYTALKINTENT_H__

#include "Voytalk.h"
#include "VoytalkConstraint.h"
#include "CborEncoder.h"

#include <stdio.h>

enum {
    VOYTALK_INTENT = 0x400D
};

class VoytalkIntent
{
public:
    VoytalkIntent(const char* _action)
        :   action(_action),
            endpoint(0xFFFFFFFF),
            constraints()
    {}

    VoytalkIntent(const char* _action,
                  VoytalkConstraint& _constraints)
        :   action(_action),
            endpoint(0xFFFFFFFF),
            constraints(_constraints)
    {
        // top level constraint is named constraints
        constraints.setVariable("constraints");
    }

    void setEndpoint(uint32_t _endpoint)
    {
        endpoint = _endpoint;
    }

    void encodeCBOR(CborEncoder& encoder)
    {

        if (constraints.isValid())
        {
            encoder.writeTag(VOYTALK_INTENT);
            encoder.writeMap(3);

            // 1. add action
            encoder.addKeyValue("action", action);

            // 2. add endpoint
            // endpoints are stored as uint32_t but transmitted as strings
            char endpointString[11] = {0};
            snprintf(endpointString, 11, "%lu", endpoint);

            encoder.addKeyValue("endpoint", endpointString);

            // 3. add constraints
            constraints.encodeCBOR(encoder);
        }
        else
        {
            encoder.writeTag(VOYTALK_INTENT);
            encoder.writeMap(2);

            // 1. add action
            encoder.addKeyValue("action", action);

            // 2. add endpoint
            // endpoints are stored as uint32_t but transmitted as strings
            char endpointString[11] = {0};
            snprintf(endpointString, 11, "/%lu", endpoint);

            encoder.addKeyValue("endpoint", endpointString);
        }
    }

private:
    const char* action;
    uint32_t endpoint;
    VoytalkConstraint constraints;
};

#endif // __VOYTALKINTENT_H__
