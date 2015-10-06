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
#include "cborg/Cbor.h"

#include <stdio.h>
#include <string.h>


class VTIntent : public VTResource
{
public:
    enum {
        TAG = 0x400D
    };

    template <size_t I>
    VTIntent(const char (&_action)[I])
        :   m_action(_action),
            m_actionLength(I-1),
            m_knownParameters(NULL),
            m_knownParametersLength(0),
            m_endpoint(NULL),
            m_endpointLength(0)//,
            //constraints()
    {}


    template <size_t I>
    VTIntent* endpoint(const char (&_endpoint)[I])
    {
        m_endpoint = _endpoint;
        m_endpointLength = I-1;
        return this;
    }

    template <size_t I>
    VTIntent* knownParameters(const char (&_knownParameters)[I])
    {
        m_knownParameters = _knownParameters;
        m_knownParametersLength = I-1;
        return this;
    }


    void encodeCBOR(Cbore& encoder)
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
            encoder.tag(VTIntent::TAG)
                .map(m_knownParameters ? 3 : 2)
                    .key(VTShortKeyAction).value(m_action, m_actionLength)
                    .key(VTShortKeyEndpoint).value(m_endpoint, m_endpointLength);

            if (m_knownParameters) {
                encoder.key(VTShortKeyKnownParameters).value(m_knownParameters, m_knownParametersLength);
            }
        //}
    }

private:
    const char* m_action;
    std::size_t m_actionLength;

    const char* m_knownParameters;
    std::size_t m_knownParametersLength;

    const char* m_endpoint;
    std::size_t m_endpointLength;

    //VoytalkConstraint constraints;
};

#endif // __VOYTALKINTENT_H__
