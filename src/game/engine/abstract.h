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
#pragma once

#ifndef _ABSTRACT_H_
#define _ABSTRACT_H_

#include "always.h"
#include "cctype.h"
#include "facing.h"
#include "language.h"
#include "rtti.h"
#include "stringex.h"
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

    int Get_Heap_ID() const { return HeapID; }
    BOOL Is_Active() const { return IsActive; }

protected:
    RTTIType RTTI; // ID for this object type, set from derived type constructors.
    int HeapID;
    uint32_t Coord;
    int Height;
#ifndef RAPP_NO_BITFIELDS
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

class AbstractTypeClass
{
public:
    AbstractTypeClass(RTTIType type, int id = -1, int uiname = TXT_NULL, const char *name = nullptr);
    AbstractTypeClass(AbstractTypeClass const &that);
    AbstractTypeClass(NoInitClass const &noinit) {}
    ~AbstractTypeClass() {}

    virtual uint32_t Coord_Fixup(uint32_t coord) const { return coord; }
    virtual int Full_Name() const;
    virtual int Get_Ownable() const { return 0xFFFFF; } // This is a bit for each of the normal houses, pluse 8 multiplayer.

    AbstractTypeClass &operator=(AbstractTypeClass &that);

    int Get_Heap_ID() const { return HeapID; }
    RTTIType const What_Am_I() const { return RTTI; }
    const char *Get_Name() { return Name; }
    
protected:
    RTTIType RTTI;
    int HeapID;
    char Name[24];
    int UIName;
};

inline AbstractTypeClass &AbstractTypeClass::operator=(AbstractTypeClass &that)
{
    if (this != &that) {
        RTTI = that.RTTI;
        HeapID = that.HeapID;
        UIName = that.UIName;
        strlcpy(Name, that.Name, sizeof(Name));
    }

    return *this;
}



#endif // _ABSTRACT_H_
