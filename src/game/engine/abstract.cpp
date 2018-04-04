/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most in game objects.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "abstract.h"
#include "globals.h"
#include "minmax.h"

AbstractClass::AbstractClass(RTTIType type, int id) :
    IsActive(true),
    RTTI(type),
    HeapID(id),
    Coord(-1),
    Height(0)
{
}

AbstractClass::AbstractClass(AbstractClass const &that) :
    IsActive(that.IsActive),
    RTTI(that.RTTI),
    HeapID(that.HeapID),
    Coord(that.Coord),
    Height(that.Height)
{
}
