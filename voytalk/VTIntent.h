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
//#include "VoytalkConstraint.h"
#include "cbor/CborEncoder.h"

#include <stdio.h>
#include <string.h>

enum {
    VOYTALK_INTENT = 0x400D
};

class VTIntent : public VTResource
{
public:
    VTIntent(const char* _action)
        :   action(_action),
            m_endpoint()//,
            //constraints()
    {}

    VTIntent* endpoint(const char *  _endpoint)
    {
        m_endpoint = _endpoint;
        return this;
    }

    VTIntent* knownParameters(const char * _knownParameters)
    {
        m_knownParameters = _knownParameters;
        return this;
    }

    
    void encodeCBOR(CborEncoder& encoder)
    {

        // if (constraints.isValid())
        // {
        //     encoder.writeTag(VOYTALK_INTENT);
        //     encoder.writeMap(m_knownParameters ? 4 : 3);

        //     // 1. add action
        //     encoder.addKeyValue("action", action);

        //     // 2. add endpoint
        //     // endpoints are stored as uint32_t but transmitted as strings
        //     char endpointString[11] = {0};
        //     snprintf(endpointString, 11, "/%lu", endpoint);

        //     encoder.addKeyValue("endpoint", endpointString);
        //     if (m_knownParameters) {
        //         encoder.writeString("knownParameters", 15);
        //         encoder.writeString(m_knownParameters, strlen(m_knownParameters));
        //     }

        //     // 3. add constraints
        //     constraints.encodeCBOR(encoder);
        // }
        // else
        // {
            encoder.writeTag(VOYTALK_INTENT);
            encoder.writeMap(m_knownParameters ? 3 : 2);

            // 1. add action
            encoder.addKeyValue("action", action);

            // 2. add endpoint
            encoder.writeString("endpoint", 8);
            encoder.writeString(m_endpoint, strlen(m_endpoint));

            if (m_knownParameters) {
                encoder.writeString("knownParameters", 15);
                encoder.writeString(m_knownParameters, strlen(m_knownParameters));
            }
        //}
    }

private:
    const char * action;
    const char * m_knownParameters;
    const char * m_endpoint;
    //VoytalkConstraint constraints;
};

#endif // __VOYTALKINTENT_H__
