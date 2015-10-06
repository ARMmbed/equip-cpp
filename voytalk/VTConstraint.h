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


#include "cborg/Cbor.h"
#include "voytalk/VTResource.h"

#include <map>

class VTConstraint : public VTResource
{
public:

    typedef enum {
        TypeBoolean    = 1,
        TypeNumber     = 2,
        TypeString     = 3,
        TypeDictionary = 4,
        TypeArray      = 5,
        TypeInvalid    = 127, // Not for actual use
    } type_t;

    VTConstraint(type_t _type)
        :   m_title(NULL),
            type(_type),

            // optional
            defaultString(NULL),
            defaultInteger(0),

            m_description(NULL),
            m_icon(NULL),
            
            children()
    {}

    template <size_t I>
    VTConstraint& title(const char (&_title)[I])
    {
        m_title = _title;
        m_titleLength = I-1;

        return *this;
    }

    /* optional */
    template <size_t I>
    VTConstraint& defaultValue(const char (&_defaultString)[I])
    {
        defaultString = _defaultString;
        defaultStringLength = I-1;

        return *this;
    }

    VTConstraint& defaultValue(int32_t _defaultInteger)
    {
        defaultInteger = _defaultInteger;

        return *this;
    }

    template <size_t I>
    VTConstraint& description(const char (&_description)[I])
    {
        m_description = _description;
        m_descriptionLength = I-1;

        return *this;
    }

    VTConstraint& addConstraint(std::string name, VTConstraint& constraint)
    {
        std::pair<std::string, VTConstraint> pair(name, constraint);
        children.insert(pair);

        return *this;
    }

    template <size_t I>
    VTConstraint& setIcon(const char (&_icon)[I])
    {
        m_icon = _icon;
        m_iconLength = I-1;

        return *this;
    }

    bool isValid()
    {
        return (m_title) && (type != TypeInvalid);
    }

    void encodeCBOR(Cbore& encoder)
    {

        encoder.map()
            .key(VTShortKeyTitle).value(m_title, m_titleLength)
            .key(VTShortKeyType).value(type);

        // add optional default value
        if (defaultString || defaultInteger)
        {
            if (type == TypeString)
            {
                encoder.key(VTShortKeyDefault).value(defaultString, defaultStringLength);
            }
            else if (type == TypeNumber)
            {
                encoder.key(VTShortKeyDefault).value(defaultInteger);
            }
        }

        // add optional description
        if (m_description)
        {
            encoder.key(VTShortKeyDescription).value(m_description, m_descriptionLength);
        }

        // add optional icon
        if (m_icon)
        {
            encoder.key(VTShortKeyIcon).value(m_icon, m_iconLength);
        }

        // Add optional nested constraints
        /*if (children.count())
        {
            encoder.key(VTShortKeyProperties);
            std::map<std::string, VTConstraint>::const_iterator iter = children.begin();

            if (iter != children.end())
            {
                encoder.key(iter->first);
                iter->second.encodeCBOR(encoder);
            }

            encoder.end();
        }*/

        encoder.end();
    }

private:
    const char* m_title;
    size_t m_titleLength;

    type_t type;

    const char* defaultString;
    size_t defaultStringLength;

    int32_t defaultInteger;

    const char* m_description;
    size_t m_descriptionLength;

    const char* m_icon;
    size_t m_iconLength;

    std::map<std::string, VTConstraint> children;
};

#endif // __VOYTALKCONSTRAINT_H__
