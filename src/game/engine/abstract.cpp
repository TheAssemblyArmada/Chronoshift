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
#include "abstract.h"
#include "building.h"
#include "coord.h"
#include "target.h"
#include <algorithm>

AbstractClass::AbstractClass(RTTIType type, int heap_id) :
    m_RTTI(type),
    m_HeapID(heap_id),
    m_Coord(-1),
    m_Height(0),
    m_IsActive(true)
{
}

AbstractClass::AbstractClass(AbstractClass const &that) :
    m_RTTI(that.m_RTTI),
    m_HeapID(that.m_HeapID),
    m_Coord(that.m_Coord),
    m_Height(that.m_Height),
    m_IsActive(that.m_IsActive)
{
}

cell_t AbstractClass::Get_Cell() const
{
    return Coord_To_Cell(m_Coord);
}

int AbstractClass::Distance_To_Target(target_t target) const
{
    int distance = Distance(Center_Coord(), As_Coord(target));
    BuildingClass *bptr = As_Building(target);

    if (bptr != nullptr) {
        distance -= (bptr->Class_Of().Width() + bptr->Class_Of().Height()) * 64;
        distance = std::max(0, distance);
    }

    return distance;
}

int AbstractClass::Distance_To_Cell(cell_t cell) const
{
    return Distance(Center_Coord(), Cell_To_Coord(cell));
}

cell_t AbstractClass::Center_Cell() const
{
    return Coord_To_Cell(Center_Coord());
}

cell_t AbstractClass::Target_Cell() const
{
    return Coord_To_Cell(Target_Coord());
}

target_t AbstractClass::As_Target() const
{
    return Make_Target(m_RTTI, m_HeapID);
    //return ((m_RTTI & 0xFF) << 24) & (m_HeapID & 0xFFFFFF);
}
