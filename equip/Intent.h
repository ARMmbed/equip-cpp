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


#ifndef __EQUIP_INTENT_H__
#define __EQUIP_INTENT_H__

#include "equip/Equip.h"
#include "cborg/Cbor.h"

#include <stdio.h>
#include <string.h>

namespace Equip {

/**
 * An Intent encapsulates an action that a device can perform, contains the information
 * that allows an app to determine if it has the permissions to perform the action, and
 * details any parameters the app must collect to invoke the action.
 **/
class Intent : public Resource
{
public:
    enum {
        TAG = 0x400D
    };

    template <size_t I>
    Intent(const char (&_action)[I])
        :   m_action(_action),
            m_actionLength(I-1),
            m_knownParameters(NULL),
            m_knownParametersLength(0),
            m_endpoint(NULL),
            m_endpointLength(0),
            m_constraints(Constraint::TypeDictionary)
    {}


    template <size_t I>
    Intent& endpoint(const char (&_endpoint)[I])
    {
        m_endpoint = _endpoint;
        m_endpointLength = I-1;
        return *this;
    }

    template <size_t I>
    Intent& knownParameters(const char (&_knownParameters)[I])
    {
        m_knownParameters = _knownParameters;
        m_knownParametersLength = I-1;
        return *this;
    }

    Constraint& constraints()
    {
        return m_constraints;
    }


    void encodeCBOR(Cbore& encoder) const
    {
        encoder.tag(Intent::TAG)
            .map()
                .key(ShortKeyAction).value(m_action, m_actionLength)
                .key(ShortKeyEndpoint).value(m_endpoint, m_endpointLength);

            if (m_knownParameters)
            {
                encoder.key(ShortKeyKnownParameters).value(m_knownParameters, m_knownParametersLength);
            }

            if (m_constraints.isValid())
            {
                encoder.key(ShortKeyConstraints);
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

    Constraint m_constraints;
};

}

#endif // __EQUIP_INTENT_H__
