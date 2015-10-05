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

#ifndef __VOYTALKCONSTRAINT_H__
#define __VOYTALKCONSTRAINT_H__


#include "cbor/CborEncoder.h"


class VoytalkConstraint : public VTResource
{
public:

    typedef enum {
        OperatorType = 0,
        OperatorSemanticType = 1,
        OperatorFormatHint = 2,
        OperatorDefault = 3,
        OperatorEquals = 4,
        OperatorProperties = 8,
        OperatorItems = 9,
        OperatorMin = 16,
        OperatorMax = 17,
        OperatorDenominator = 20,
        // Meta-data
        OperatorTitle = 64,
        OperatorDescription = 65,
        OperatorIcon = 66,
        // Not for actual use
        OperatorInvalid = 127,
    } operator_t;

    typedef enum {
        TypeBoolean    = 1,
        TypeNumber     = 2,
        TypeString     = 3,
        TypeDictionary = 4,
        TypeArray      = 5,
        TypeInvalid    = 127, // Not for actual use
    } type_t;

    VoytalkConstraint(type_t _type)
        :   m_title(NULL),
            m_variable(NULL),
            m_type(_type),

            // optional
            m_defaultString(NULL),
            m_defaultInteger(0),

            m_description(NULL),
            m_icon(NULL),
            
            m_children()
    {}

    VoytalkConstraint& title(const char* _variable)
    {
        variable = _variable;

        return *this;
    }

    /* optional */
    VoytalkConstraint& default(const char* _defaultString)
    {
        defaultString = _defaultString;

        return *this;
    }

    VoytalkConstraint& default(int32_t _defaultInteger)
    {
        defaultInteger = _defaultInteger;

        return *this;
    }

    VoytalkConstraint& description(const char* _description)
    {
        description = _description;

        return *this;
    }

    template <size_t I>
    VoytalkConstraint& setProperties(VoytalkConstraint* (&_properties)[I])
    {
        properties = _properties;
        propertySize = I;

        return *this;
    }

    VoytalkConstraint& setIcon(const char* _icon)
    {
        icon = _icon;

        return *this;
    }

    bool isValid()
    {
        return (title) && (variable) && (type != TypeInvalid);
    }

    void encodeCBOR(CborEncoder& encoder)
    {
        // insert variable name
        encoder.addKey(variable);

        // calculate size of constraint and insert constraint map
        if (defaultString || defaultInteger) constraintSize++;
        if (description) constraintSize++;
        if (properties) constraintSize++;
        if (icon) constraintSize ++;

        encoder.writeMap(constraintSize);

        // mandatory fields
        encoder.addKeyValue(OperatorTitle, title);
        encoder.addKeyValue(OperatorType, type);

        // add optional default value
        if (defaultString || defaultInteger)
        {
            if (type == TypeString)
            {
                encoder.addKeyValue(OperatorDefault, defaultString);
            }
            else if (type == TypeNumber)
            {
                encoder.addKeyValue(OperatorDefault, defaultInteger);
            }
        }

        // add optional description
        if (description)
        {
            encoder.addKeyValue(OperatorDescription, description);
        }

        // add optional icon
        if (icon)
        {
            encoder.addKeyValue(OperatorIcon, icon);
        }

        // Add optional nested constraints
        if (properties)
        {
            encoder.addKey(OperatorProperties);
            encoder.writeMap(propertySize);

            for (size_t idx = 0; idx < propertySize; idx++)
            {
                (properties[idx])->encodeCBOR(encoder);
            }
        }
    }

private:
    const char* m_title;
    const char* m_id;
    type_t type;

    const char* m_defaultString;
    int32_t m_defaultInteger;

    const char* m_description;
    const char* m_icon;

    std::vector<VTConstraint*> m_children;
};

#endif // __VOYTALKCONSTRAINT_H__
