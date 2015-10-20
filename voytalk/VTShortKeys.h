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

#ifndef __VOYTALKSHORTKEYS_H__
#define __VOYTALKSHORTKEYS_H__

/**
 * Short keys are used as keys in CBOR dicotionaries to replace common values that 
 * would otherwise use a string key. This is an optimisation to reduce the wire size of 
 * the protocol.
 **/
typedef enum VTShortKey {
    // Up to 24 can be CBOR-encoded in a single byte, so they're the high-priority ones
    // NOTE: if we want to be more compact, we can also encode -5 to -25 in a single-byte
    VTShortKeyId = 0,
    VTShortKeyMethod = 1,
    VTShortKeyUrl = 2,
    VTShortKeyBody = 3,
    VTShortKeyStatus = 4,
    VTShortKeyAction = 5,
    VTShortKeyName = 6,
    VTShortKeyIntents = 7,
    VTShortKeyEndpoint = 8,
    VTShortKeyResponseUrl = 9,
    VTShortKeyParameters = 10,
    VTShortKeyKnownParameters = 11,
    VTShortKeyAuthority = 12,
    VTShortKeyInvocation = 13,
    VTShortKeySuccess = 14,
    VTShortKeyValue = 15,
    VTShortKeyPriority = 16,
    VTShortKeyConstraints = 17,
    
    // Up to 256 can be CBOR-encoded in two bytes
    
    // Constraints: generic
    VTShortKeyTitle = 64,
    VTShortKeyDescription = 65,
    VTShortKeyIcon = 66,
    VTShortKeyOrdering = 67,
    VTShortKeyDefault = 68,
    // Known parameters saving
    VTShortKeyShouldSave = 69,
    VTShortKeyUniqueKey = 70,

    // Constraints: universal
    VTShortKeyType = 80,
    VTShortKeySemanticType = 81,
    VTShortKeyFormatHint = 82,
    VTShortKeyProperties = 83,
    VTShortKeyItems = 84,
    VTShortKeyEquals = 85,

    // Numeric
    VTShortKeyMin = 92,
    VTShortKeyMax = 93,
    VTShortKeyDenominator = 94,

    // Not for actual use - this should never be encoded
    VTInvalidShortKey = 65536
} voytalk_short_key_t;


#endif /* __VOYTALKSHORTKEYS_H__ */
