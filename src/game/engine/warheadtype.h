/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about warhead behaviour.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef WARHEADTYPE_H
#define WARHEADTYPE_H

#include "always.h"
#include "armor.h"
#include "fixed.h"
#include "heap.h"

enum WarheadType
{
    WARHEAD_NONE = -1,
    WARHEAD_FIRST = 0,
    WARHEAD_SA = 0,
    WARHEAD_HE = 1,
    WARHEAD_AP = 2,
    WARHEAD_FIRE = 3,
    WARHEAD_HOLLOWPOINT = 4,
    WARHEAD_SUPER = 5,
    WARHEAD_ORGANIC = 6,
    WARHEAD_NUKE = 7,
    WARHEAD_MECHANICAL = 8,
    WARHEAD_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(WarheadType);

class CCINIClass;

class WarheadTypeClass
{
public:
    WarheadTypeClass(WarheadType warhead, const char *name);
    WarheadTypeClass(WarheadTypeClass const &that);
    WarheadTypeClass(NoInitClass const &noinit) {}
    ~WarheadTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    void Code_Pointers() {}
    void Decode_Pointers() {}
    BOOL Read_INI(CCINIClass &ini);
    const char *Get_Name() const { return Name; }
    int Get_Type() const { return Type; }
    BOOL Is_Wall_Destroyer() const { return Wall; }

    static void Init_Heap();
    static WarheadType From_Name(const char *name);
    static const char *Name_From(WarheadType warhead);
    static WarheadTypeClass &As_Reference(WarheadType warhead);
    static WarheadTypeClass *As_Pointer(WarheadType warhead);

private:
    int Type;
    const char *Name;
    int Spread; // damage spread factor[larger means greater spread]. Damage halves every n pixels from center.
#ifndef RAPP_NO_BITFIELDS
                // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Wall : 1; // Does this warhead damage concrete walls?
            bool Wood : 1; // Does this warhead damage wood walls?
            bool Ore : 1; // Does this warhead destroy ore?
            bool UnkBool : 1; // Only checked in InfantryClass::Greatest_Threat. Removes targets except infantry and harvester
        };
        int Bitfield;
    };
#else
    bool Wall; // Does this warhead damage concrete walls?
    bool Wood; // Does this warhead damage wood walls?
    bool Ore; // Does this warhead destroy ore?
    bool UnkBool; // Only checked in InfantryClass::Greatest_Threat. Removes targets except infantry and harvester
#endif
    fixed Verses[ARMOR_COUNT]; // Damage verses various armor types (as percentage of full damage). eg, Verses[ARMOR_WOOD];
    int Explosion; // Which explosion set to use when warhead of this type impacts.
    int Death; // Which infantry death animation to use.
};

#ifndef RAPP_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<WarheadTypeClass> &WarheadTypes;
#else
extern TFixedIHeapClass<WarheadTypeClass> WarheadTypes;
#endif

#endif // WARHEADTYPE_H
