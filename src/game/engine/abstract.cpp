/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for in game objects.
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
#include "target.h"
#include <algorithm>

/**
 * Class constructor.
 */
AbstractClass::AbstractClass(RTTIType type, int heap_id) :
    m_RTTI(type),
    m_HeapID(heap_id),
    m_Coord(-1),
    m_Height(0),
    m_IsActive(true)
{
}

/**
 * Copy constructor.
 */
AbstractClass::AbstractClass(const AbstractClass &that) :
    m_RTTI(that.m_RTTI),
    m_HeapID(that.m_HeapID),
    m_Coord(that.m_Coord),
    m_Height(that.m_Height),
    m_IsActive(that.m_IsActive)
{
}

/**
 * Dump and print realtime info to the debug console.
 */
#ifdef CHRONOSHIFT_DEBUG
void AbstractClass::Debug_Dump(MonoClass *mono) const
{
    DEBUG_ASSERT(mono != nullptr);
    DEBUG_ASSERT(m_IsActive);

    /*mono->Set_Cursor(11, 5);
    mono->Printf("%08X", As_Target());

    mono->Set_Cursor(20, 1);
    mono->Printf("%08X", m_Coord);

    mono->Set_Cursor(29, 1);
    mono->Printf("%3d", m_Height);

    if (Owner() != HOUSES_NONE) {
        mono->Set_Cursor(1, 3);
        TextEnum name = HouseTypeClass::As_Reference(Owner())->Get_UIName();
        mono->Printf("%-18s", Text_String(name));
    }*/
}
#endif

/**
 * Distance from this object to the specificed target.
 */
int AbstractClass::Distance_To_Target(target_t target) const
{
    int distance = Coord_Distance(Center_Coord(), As_Coord(target));

    // If the target is a building, we need to fake its dimensions in the
    // game world, so adjust distance accordingly.
    BuildingClass *bptr = As_Building(target);

    if (bptr != nullptr) {
        distance -= (bptr->Class_Of().Width() + bptr->Class_Of().Height()) * 64;
        distance = std::max(0, distance);
    }

    return distance;
}
