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
#pragma once

#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "always.h"
#include "facing.h"
#include "gametypes.h"
#include "rtti.h"
#include "vector.h"

class NoInitClass;

class AbstractClass
{
public:
    AbstractClass(RTTIType type = RTTI_NONE, int id = -1);
    AbstractClass(const AbstractClass &that);
    AbstractClass(const NoInitClass &noinit) {}
    virtual ~AbstractClass() {}

    virtual const char *Name() const { return ""; }
    virtual HousesType Owner() const { return HOUSES_NONE; }
    virtual coord_t Center_Coord() const { return m_Coord; }
    virtual coord_t Target_Coord() const { return m_Coord; }
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const { return MOVE_OK; }
    virtual void AI() {}

    BOOL Is_Techno() const;
    BOOL Is_Foot() const;
    BOOL Is_Ground_Foot() const;

    int Get_Heap_ID() const { return m_HeapID; }
    BOOL Is_Active() const { return m_IsActive; }
    RTTIType What_Am_I() const { return m_RTTI; }
    coord_t Get_Coord() const { return m_Coord; }
    cell_t Get_Cell() const;
    void Set_Coord(coord_t coord) { m_Coord = coord; }
    int Get_Height() const { return m_Height; }
    void Set_Height(int height) { m_Height = height; }

protected:
    RTTIType m_RTTI; // ID for this object type, set from derived type constructors.
    int m_HeapID;
    coord_t m_Coord;
    int m_Height;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_IsActive : 1; // 1
        };
        int Bitfield;
    };
#else
    bool m_IsActive;
#endif
};

inline BOOL AbstractClass::Is_Techno() const
{
    return m_RTTI == RTTI_BUILDING || m_RTTI == RTTI_UNIT || m_RTTI == RTTI_INFANTRY || m_RTTI == RTTI_VESSEL
        || m_RTTI == RTTI_AIRCRAFT;
}

inline BOOL AbstractClass::Is_Foot() const
{
    return m_RTTI == RTTI_UNIT || m_RTTI == RTTI_INFANTRY || m_RTTI == RTTI_VESSEL || m_RTTI == RTTI_AIRCRAFT;
}

inline BOOL AbstractClass::Is_Ground_Foot() const
{
    return m_RTTI == RTTI_UNIT || m_RTTI == RTTI_INFANTRY || m_RTTI == RTTI_VESSEL;
}

#endif // ABSTRACT_H
