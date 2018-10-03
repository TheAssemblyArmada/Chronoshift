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
    AbstractClass(AbstractClass const &that);
    AbstractClass(NoInitClass const &noinit) {}
    virtual ~AbstractClass() {}

    virtual const char *Name() const { return ""; }
    virtual HousesType Owner() const { return HOUSES_NONE; }
    virtual uint32_t Center_Coord() const { return Coord; }
    virtual uint32_t Target_Coord() const { return Coord; }
    virtual MoveType Can_Enter_Cell(int16_t cellnum, FacingType facing = FACING_NONE) const { return MOVE_OK; }
    virtual void AI() {}

    BOOL Is_Techno() const;
    BOOL Is_Foot() const;
    int Get_Heap_ID() const { return HeapID; }
    BOOL Is_Active() const { return IsActive; }
    RTTIType What_Am_I() const { return RTTI; }
    uint32_t Get_Coord() const { return Coord; }
    int Get_Height() const { return Height; }

protected:
    RTTIType RTTI; // ID for this object type, set from derived type constructors.
    int HeapID;
    uint32_t Coord;
    int Height;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool IsActive : 1; // & 1
        };
        int Bitfield;
    };
#else
    bool IsActive;
#endif
};

inline BOOL AbstractClass::Is_Techno() const
{
    return RTTI == RTTI_BUILDING || RTTI == RTTI_UNIT || RTTI == RTTI_INFANTRY || RTTI == RTTI_VESSEL
        || RTTI == RTTI_AIRCRAFT;
}

inline BOOL AbstractClass::Is_Foot() const
{
    return RTTI == RTTI_UNIT || RTTI == RTTI_INFANTRY || RTTI == RTTI_VESSEL || RTTI == RTTI_AIRCRAFT;
}

#endif // ABSTRACT_H
