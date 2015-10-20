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
#include "VTConstraint.h"
#include "cborg/Cbor.h"

#include <stdio.h>
#include <string.h>


/**
 * An Intent encapsulates an action that a device can perform, contains the information
 * that allows an app to determine if it has the permissions to perform the action, and
 * details any parameters the app must collect to invoke the action.
 **/
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
            m_endpointLength(0),
            m_constraints(VTConstraint::TypeDictionary)
    {}


    template <size_t I>
    VTIntent& endpoint(const char (&_endpoint)[I])
    {
        m_endpoint = _endpoint;
        m_endpointLength = I-1;
        return *this;
    }

    template <size_t I>
    VTIntent& knownParameters(const char (&_knownParameters)[I])
    {
        m_knownParameters = _knownParameters;
        m_knownParametersLength = I-1;
        return *this;
    }

    VTConstraint& constraints()
    {
        return m_constraints;
    }


    void encodeCBOR(Cbore& encoder) const
    {
        encoder.tag(VTIntent::TAG)
            .map()
                .key(VTShortKeyAction).value(m_action, m_actionLength)
                .key(VTShortKeyEndpoint).value(m_endpoint, m_endpointLength);

            if (m_knownParameters)
            {
                encoder.key(VTShortKeyKnownParameters).value(m_knownParameters, m_knownParametersLength);
            }

            if (m_constraints.isValid())
            {
                encoder.key(VTShortKeyConstraints);
                // value written by serialisaing constraints
                m_constraints.encodeCBOR(encoder);
            }
        encoder.end();
    }

private:
    const char* m_action;
    std::size_t m_actionLength;

    const char* m_knownParameters;
    std::size_t m_knownParametersLength;

    const char* m_endpoint;
    std::size_t m_endpointLength;

    VTConstraint m_constraints;
};

#endif // __VOYTALKINTENT_H__
