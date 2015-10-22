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

#ifndef __EQUIP_SHORTKEYS_H__
#define __EQUIP_SHORTKEYS_H__

/**
 * Short keys are used as keys in CBOR dicotionaries to replace common values that 
 * would otherwise use a string key. This is an optimisation to reduce the wire size of 
 * the protocol.
 **/
typedef enum ShortKey {
    // Up to 24 can be CBOR-encoded in a single byte, so they're the high-priority ones
    // NOTE: if we want to be more compact, we can also encode -5 to -25 in a single-byte
    ShortKeyId = 0,
    ShortKeyMethod = 1,
    ShortKeyUrl = 2,
    ShortKeyBody = 3,
    ShortKeyStatus = 4,
    ShortKeyAction = 5,
    ShortKeyName = 6,
    ShortKeyIntents = 7,
    ShortKeyEndpoint = 8,
    ShortKeyResponseUrl = 9,
    ShortKeyParameters = 10,
    ShortKeyKnownParameters = 11,
    ShortKeyAuthority = 12,
    ShortKeyInvocation = 13,
    ShortKeySuccess = 14,
    ShortKeyValue = 15,
    ShortKeyPriority = 16,
    ShortKeyConstraints = 17,
    
    // Up to 256 can be CBOR-encoded in two bytes
    
    // Constraints: generic
    ShortKeyTitle = 64,
    ShortKeyDescription = 65,
    ShortKeyIcon = 66,
    ShortKeyOrdering = 67,
    ShortKeyDefault = 68,
    // Known parameters saving
    ShortKeyShouldSave = 69,
    ShortKeyUniqueKey = 70,

    // Constraints: universal
    ShortKeyType = 80,
    ShortKeySemanticType = 81,
    ShortKeyFormatHint = 82,
    ShortKeyProperties = 83,
    ShortKeyItems = 84,
    ShortKeyEquals = 85,

    // Numeric
    ShortKeyMin = 92,
    ShortKeyMax = 93,
    ShortKeyDenominator = 94,

    // Not for actual use - this should never be encoded
    InvalidShortKey = 65536
} short_key_t;


#endif /* __EQUIP_SHORTKEYS_H__ */
