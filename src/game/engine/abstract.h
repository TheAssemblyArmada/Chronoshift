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
#include "noinit.h"
#include "rtti.h"

class NoInitClass;

class AbstractClass
{
public:
    AbstractClass(RTTIType type = RTTI_NONE, int heap_id = -1);
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
    int Distance_To_Target(target_t target) const;
    int Distance_To_Cell(cell_t cell) const;
    cell_t Center_Cell() const;
    cell_t Target_Cell() const;
    target_t As_Target() const;
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
    BOOL m_IsActive : 1; // 1
#else
    bool m_IsActive;
#endif

#ifndef CHRONOSHIFT_STANDALONE
public:
    static void Hook_Me();

private:
    AbstractClass *Hook_Ctor_NoInit(const NoInitClass &noinit) { return new (this) AbstractClass(noinit); }
    void Hook_Dtor() { AbstractClass::~AbstractClass(); }
    const char *Hook_Name() { return AbstractClass::Name(); }
    HousesType Hook_Owner() { return AbstractClass::Owner(); }
    coord_t Hook_Center_Coord() { return AbstractClass::Center_Coord(); }
    coord_t Hook_Target_Coord() { return AbstractClass::Target_Coord(); }
    MoveType Hook_Can_Enter_Cell(cell_t cellnum, FacingType facing) { return AbstractClass::Can_Enter_Cell(cellnum, facing); }
    int Hook_Distance_To_Target(target_t target) { return AbstractClass::Distance_To_Target(target); }
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void AbstractClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004CD8E0, *AbstractClass::Hook_Ctor_NoInit);
    Hook_Function(0x00423CB0, *AbstractClass::Hook_Dtor);
    Hook_Function(0x004CD8F0, *AbstractClass::Hook_Name);
    Hook_Function(0x00423CE0, *AbstractClass::Hook_Owner);
    Hook_Function(0x004CD900, *AbstractClass::Hook_Center_Coord);
    Hook_Function(0x004CD910, *AbstractClass::Hook_Target_Coord);
    Hook_Function(0x00423CF0, *AbstractClass::Hook_Can_Enter_Cell);
    Hook_Function(0x004CD920, *AbstractClass::AI);
    Hook_Function(0x00405540, *AbstractClass::Hook_Distance_To_Target);
#endif
}
#endif

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
