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

#ifndef __EQUIP_CONSTRAINT_H__
#define __EQUIP_CONSTRAINT_H__


#include "cborg/Cbor.h"
#include "equip/Resource.h"

#include <map>

namespace Equip {

/**
 * Constraints are a tree-based decription of the requirments for a set of parameters, for example
 * a constraint might describe a parameters must be an integer value, with a maximum of 5 and minimum
 * of 1.
 **/
class Constraint : public Resource
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

    Constraint(type_t _type)
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
    Constraint& title(const char (&_title)[I])
    {
        m_title = _title;
        m_titleLength = I-1;

        return *this;
    }

    /* optional */
    template <size_t I>
    Constraint& defaultValue(const char (&_defaultString)[I])
    {
        defaultString = _defaultString;
        defaultStringLength = I-1;

        return *this;
    }

    Constraint& defaultValue(int32_t _defaultInteger)
    {
        defaultInteger = _defaultInteger;

        return *this;
    }

    template <size_t I>
    Constraint& description(const char (&_description)[I])
    {
        m_description = _description;
        m_descriptionLength = I-1;

        return *this;
    }

    Constraint& addProperty(std::string name, Constraint& constraint)
    {
        std::pair<std::string, Constraint> pair(name, constraint);
        children.insert(pair);

        return *this;
    }

    template <size_t I>
    Constraint& icon(const char (&_icon)[I])
    {
        m_icon = _icon;
        m_iconLength = I-1;

        return *this;
    }

    bool isValid() const
    {
        return (m_title) && (type != TypeInvalid);
    }

    void encodeCBOR(Cbore& encoder) const
    {

        encoder.map()
            .key(ShortKeyTitle).value(m_title, m_titleLength)
            .key(ShortKeyType).value(type);

        // add optional default value
        if (defaultString || defaultInteger)
        {
            if (type == TypeString)
            {
                encoder.key(ShortKeyDefault).value(defaultString, defaultStringLength);
            }
            else if (type == TypeNumber)
            {
                encoder.key(ShortKeyDefault).value(defaultInteger);
            }
        }

        // add optional description
        if (m_description)
        {
            encoder.key(ShortKeyDescription).value(m_description, m_descriptionLength);
        }

        // add optional icon
        if (m_icon)
        {
            encoder.key(ShortKeyIcon).value(m_icon, m_iconLength);
        }

        // Add optional nested constraints
        if (children.size())
        {
            encoder.key(ShortKeyProperties);
            std::map<std::string, Constraint>::const_iterator iter = children.begin();
            encoder.map();
            while (iter != children.end())
            {
                encoder.key(iter->first.c_str(), iter->first.length());
                iter->second.encodeCBOR(encoder);
                iter++;
            }
            encoder.end();
        }

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

    std::map<std::string, Constraint> children;
};

}

#endif // __EQUIP_CONSTRAINT_H__
