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

#include "VTIntent.h"
#include "cbor/CborEncoder.h"

enum {
    VOYTALK_CODA = 0x400F
};

class VTCoda : public VTResource
{
public:
    VTCoda(uint32_t _id, bool _success, bool _more = false)
        :   id(_id),
            success(_success),
            more(_more),
            intents(NULL),
            intentSize(0)
    {}

    template <size_t I>
    VTCoda(uint32_t _id, bool _success, bool _more, VoytalkIntent* (&_intents)[I])
        :   id(_id),
            success(_success),
            more(_more),
            intents(_intents),
            intentSize(I)
    {}

    void encodeCBOR(CborEncoder& encode)
    {
        encode.writeTag(VOYTALK_CODA);

        encode.writeMap(2);
        encode.addKeyValue("invocation", id);
        encode.addKeyValue("success", success);

        if (more)
        {
            encode.addKeyValue("more", 1);
        }

        if (intents)
        {
            encode.writeArray(intentSize);

            for (size_t idx = 0; idx < intentSize; idx++)
            {
                intents[idx]->encodeCBOR(encode);
            }
        }
    }

private:
    uint32_t id;
    bool success;
    bool more;
    VoytalkIntent** intents;
    size_t intentSize;
};

#endif // __VOYTALKCODA_H__