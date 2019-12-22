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
#pragma once

#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "always.h"
#include "target.h"
#include "coord.h"
#include "facing.h"
#include "gametypes.h"
#include "noinit.h"
#include "rtti.h"

class NoInitClass;
class MonoClass;

class AbstractClass
{
    friend void Setup_Hooks();

public:
    AbstractClass(RTTIType type = RTTI_NONE, int heap_id = -1);
    AbstractClass(const AbstractClass &that);
    AbstractClass(const NoInitClass &noinit) {}
    virtual ~AbstractClass() {}

#ifdef CHRONOSHIFT_DEBUG
#ifdef CHRONOSHIFT_STANDALONE
    virtual void Debug_Dump(MonoClass *mono) const;
#else
    void Debug_Dump(MonoClass *mono) const; // TODO: Remove this prototype when standalone, but retain aboves position in table.
#endif
#endif

    virtual const char *Name() const { return ""; } // Name of this object.
    virtual HousesType Owner() const { return HOUSES_NONE; } // What house(s) own this object.
    virtual coord_t Center_Coord() const { return m_Coord; } // Fetches current center coordinate in the game world.
    virtual coord_t Target_Coord() const { return m_Coord; } // Fetches target coordinate in the game world, for weapon targeting purposes.
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const { return MOVE_OK; } // Determine if specific cell can be entered.
    virtual void AI() {} // AI processing function (called per game tick).

    int Distance_To_Coord(coord_t coord) const { return Coord_Distance(Center_Coord(), coord); }
    int Distance_To_Cell(cell_t cell) const { return Cell_Distance(Coord_To_Cell(Center_Coord()), cell); }
    int Distance_To_Object(AbstractClass *abstract) const { return Coord_Distance(Center_Coord(), abstract->Get_Coord()); }
    int Distance_To_Object_Center(AbstractClass *abstract) const { return Coord_Distance(Center_Coord(), abstract->Center_Coord()); }
    int Distance_To_Target_Center(AbstractClass *abstract) const { return Coord_Distance(Center_Coord(), abstract->Target_Coord()); }
    int Distance_To_Target(target_t target) const;

    DirType Direction_To_Coord(coord_t coord) const { return Coord_Direction(Center_Coord(), coord); }
    DirType Direction_To_Cell(cell_t cell) const { return Cell_Direction(Coord_To_Cell(Center_Coord()), cell); }
    DirType Direction_To_Object(AbstractClass *abstract) const { return Coord_Direction(Center_Coord(), abstract->Get_Coord()); }
    DirType Direction_To_Object_Center(AbstractClass *abstract) const { return Coord_Direction(Center_Coord(), abstract->Center_Coord()); }
    DirType Direction_To_Target_Center(AbstractClass *abstract) const { return Coord_Direction(Center_Coord(), abstract->Target_Coord()); }
    DirType Direction_To_Target(target_t target) const { return Coord_Direction(Center_Coord(), As_Coord(target)); }

    cell_t Center_Cell() const { return Coord_To_Cell(Center_Coord()); } // Fetches current cell position in the game world, based on center coordinate.
    cell_t Target_Cell() const { return Coord_To_Cell(Target_Coord()); } // Fetches current cell position in the game world, for weapon targeting purposes.

    target_t As_Target() const { return Make_Target(m_RTTI, m_HeapID); } // Create a target identifier instance of this object.

    BOOL Is_Techno() const;
    BOOL Is_Foot() const;
    BOOL Is_Ground_Foot() const;

    RTTIType What_Am_I() const { return m_RTTI; } // Fetch the RTTI type ID number.
    int Get_Heap_ID() const { return m_HeapID; } // Fetch the heap ID number for this object.
    coord_t Get_Coord() const { return m_Coord; } // Fetches current coordinate in the game world.
    cell_t Get_Cell() const { return Coord_To_Cell(m_Coord); } // Fetches current cell position in the game world.
    void Set_Coord(coord_t coord) { m_Coord = coord; } // Assigns immediate coordinate in the game world.
    int Get_Height() const { return m_Height; } // Fetches current height in the game world.
    void Set_Height(int height) { m_Height = height; }  // Assigns immediate height in the game world.
    BOOL Is_Active() const { return m_IsActive; } // Query if this object is currently active.
    void Set_Inactive() { m_IsActive = false; } // Set this object to inactive.

protected:
    RTTIType m_RTTI; // RTTI type ID number for this object, set by derived class constructors.
    int m_HeapID; // Memory heap ID number of this object, set by derived class constructors.
    coord_t m_Coord; // The coord of this object in the game world.
    int m_Height; // Height of this object in the game world.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1;
#else
    bool m_IsActive; // Is this object allocated in memory and thus active?
#endif

#ifdef GAME_DLL
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

/**
 * Is this a techno object (i.e. buildable by the player/house)?
 */
inline BOOL AbstractClass::Is_Techno() const
{
    return m_RTTI == RTTI_BUILDING || m_RTTI == RTTI_UNIT || m_RTTI == RTTI_INFANTRY || m_RTTI == RTTI_VESSEL
        || m_RTTI == RTTI_AIRCRAFT;
}

/**
 * Is this a foot object (i.e. moveable object)?
 */
inline BOOL AbstractClass::Is_Foot() const
{
    return m_RTTI == RTTI_UNIT || m_RTTI == RTTI_INFANTRY || m_RTTI == RTTI_VESSEL || m_RTTI == RTTI_AIRCRAFT;
}

/**
 * Is this a grounded foot object (i.e. grounded moveable object)?
 */
inline BOOL AbstractClass::Is_Ground_Foot() const
{
    return m_RTTI == RTTI_UNIT || m_RTTI == RTTI_INFANTRY || m_RTTI == RTTI_VESSEL;
}

#endif // ABSTRACT_H
