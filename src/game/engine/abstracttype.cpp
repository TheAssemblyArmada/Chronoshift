/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most in game objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "abstracttype.h"

AbstractTypeClass::AbstractTypeClass(RTTIType type, int id, int uiname, const char *name) :
    RTTI(type),
    HeapID(id),
    UIName(uiname)
{
    strlcpy(Name, name, sizeof(Name));
}

AbstractTypeClass::AbstractTypeClass(AbstractTypeClass const &that) :
    RTTI(that.RTTI),
    HeapID(that.HeapID),
    UIName(that.UIName)
{
    strlcpy(Name, that.Name, sizeof(Name));
}

int AbstractTypeClass::Full_Name() const
{
    for (int i = 0; i < 25; ++i) {
        if (100 * (RTTI + 1) + HeapID == NameIDOverride[i]) {
            return -(i + 1);
        }
    }

    return UIName;
}