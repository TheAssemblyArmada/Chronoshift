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
#include "facing.h"
#include "language.h"
#include "rtti.h"
#include "stringex.h"
#include "vector.h"

class NoInitClass;

enum HousesType
{
    HOUSES_NONE = -1,

    HOUSES_FIRST = 0,

    HOUSES_SPN = 0, //"Spain"
    HOUSES_GRE = 1, //"Greece"
    HOUSES_RED = 2, //"USSR"
    HOUSES_ENG = 3, //"England"
    HOUSES_UKA = 4, //"Ukraine"		//HOUSES_ITA = 4,	//"Italy"		//from EDWIN
    HOUSES_GER = 5, //"Germany"
    HOUSES_FRA = 6, //"France"
    HOUSES_TRK = 7, //"Turkey"

    HOUSES_GDI = 8, //"GoodGuy"
    HOUSES_NOD = 9, //"BadGuy"

    HOUSES_CIV = 10, //"Neutral"
    HOUSES_JP = 11, //"Special"

    HOUSES_LAST = 11,

    HOUSES_MULTI_FIRST = 12,

    HOUSES_MULTI_1 = 12, //"Multi1"
    HOUSES_MULTI_2 = 13, //"Multi2"
    HOUSES_MULTI_3 = 14, //"Multi3"
    HOUSES_MULTI_4 = 15, //"Multi4"
    HOUSES_MULTI_5 = 16, //"Multi5"
    HOUSES_MULTI_6 = 17, //"Multi6"
    HOUSES_MULTI_7 = 18, //"Multi7"
    HOUSES_MULTI_8 = 19, //"Multi8"

    HOUSES_MULTI_LAST = 19,

    HOUSES_ALL_LAST = 19,

    HOUSES_MULTI_COUNT = 8,

    HOUSES_COUNT = 20
};

DEFINE_ENUMERATION_OPERATORS(HousesType);

// TODO move to where ever MoveType ends up.
// https://github.com/Ares-Developers/YRpp/blob/d387fbb9d36e4645d93caf87be502d6d87d042c3/GeneralDefinitions.h#L1359
enum MoveType
{
    MOVE_NONE = -1,
    MOVE_OK,
    MOVE_CLOAK,
    MOVE_MOVING_BLOCK,
    MOVE_DESTROYABLE,
    MOVE_TEMP,
    MOVE_NO,
    MOVE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MoveType);

class AbstractClass
{
public:
    AbstractClass(RTTIType type = RTTI_NONE, int id = -1);
    AbstractClass(AbstractClass const &that);
    AbstractClass(NoInitClass const &noinit) {}
    virtual ~AbstractClass() {}

    virtual const char *Name() const { return ""; }
    virtual HousesType Owner() const { return HOUSES_NONE; }
    virtual uint32_t const Center_Coord() const { return Coord; }
    virtual uint32_t const Target_Coord() const { return Coord; }
    virtual MoveType Can_Enter_Cell(int16_t cellnum, FacingType facing = FACING_NONE) const { return MOVE_OK; }
    virtual void AI() {}

    int Get_Heap_ID() const { return HeapID; }

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
