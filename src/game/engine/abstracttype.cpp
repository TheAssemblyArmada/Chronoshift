/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most in game type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "abstracttype.h"

AbstractTypeClass::AbstractTypeClass(RTTIType type, int heap_id, int ui_name, const char *name) :
    m_RTTI(type),
    m_HeapID(heap_id),
    m_UIName(ui_name)
{
    strlcpy(m_Name, name, sizeof(m_Name));
}

AbstractTypeClass::AbstractTypeClass(const AbstractTypeClass &that) :
    m_RTTI(that.m_RTTI),
    m_HeapID(that.m_HeapID),
    m_UIName(that.m_UIName)
{
    strlcpy(m_Name, that.m_Name, sizeof(m_Name));
}

int AbstractTypeClass::Full_Name() const
{
    for (int i = 0; i < 25; ++i) {
        if (100 * (m_RTTI + 1) + m_HeapID == NameIDOverride[i]) {
            return -(i + 1);
        }
    }

    return m_UIName;
}