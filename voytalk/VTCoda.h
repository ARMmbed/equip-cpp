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

#ifndef __VOYTALKCODA_H__
#define __VOYTALKCODA_H__

#include "voytalk/VTIntent.h"
#include "cborg/Cbor.h"

/**
 * A Coda is a verifiable receipt that a device received and performed an Invocation.
 **/
class VTCoda : public VTResource
{
public:
    enum {
        TAG = 0x400F
    };

    VTCoda(uint32_t _id)
        :   m_id(_id),
            intentVector()
    {}

    VTCoda& intent(VTIntent intent)
    {
        intentVector.push_back(intent);
        return *this;
    }

    VTCoda& success(bool _success)
    {
        m_success = _success;
        return *this;
    }

    void encodeCBOR(Cbore& encode) const
    {

        encode.tag(VTCoda::TAG)
            .map()
                .key(VTShortKeyInvocation).value(m_id)
                .key(VTShortKeySuccess).value(m_success);

        if (intentVector.size() != 0)
        {
            encode.key(VTShortKeyIntents)
                .array();

            for (size_t idx = 0; idx < intentVector.size(); idx++)
            {
                intentVector[idx].encodeCBOR(encode);
            }

            encode.end();
        }

        encode.end();
    }

private:
    uint32_t m_id;
    bool m_success;
    std::vector<VTIntent> intentVector;
};

#endif // __VOYTALKCODA_H__
