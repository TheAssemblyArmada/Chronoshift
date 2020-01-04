/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about warhead behaviour.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
    WARHEAD_SA,
    WARHEAD_HE,
    WARHEAD_AP,
    WARHEAD_FIRE,
    WARHEAD_HOLLOWPOINT,
    WARHEAD_SUPER,
    WARHEAD_ORGANIC,
    WARHEAD_NUKE,
    WARHEAD_MECHANICAL,

    WARHEAD_COUNT,

    WARHEAD_NONE = -1,
    WARHEAD_FIRST = WARHEAD_SA,
};

DEFINE_ENUMERATION_OPERATORS(WarheadType);

class GameINIClass;

class WarheadTypeClass
{
public:
    WarheadTypeClass(WarheadType warhead, const char *name);
    WarheadTypeClass(const WarheadTypeClass &that);
    WarheadTypeClass(const NoInitClass &noinit) {}
    ~WarheadTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    void Code_Pointers() {}
    void Decode_Pointers() {}
    BOOL Read_INI(GameINIClass &ini);
    const char *Get_Name() const { return m_Name; }
    int Get_Spread() const { return m_Spread; }
    WarheadType What_Type() const { return (WarheadType)m_Type; }
    BOOL Is_Wall_Destroyer() const { return m_Wall; }
    const fixed_t &Get_Verses(ArmorType armor) const { return m_Verses[armor]; }

    static void Init_Heap();
    static WarheadType From_Name(const char *name);
    static const char *Name_From(WarheadType warhead);
    static WarheadTypeClass &As_Reference(WarheadType warhead);
    static WarheadTypeClass *As_Pointer(WarheadType warhead);

private:
    int m_Type; // WarheadType
    const char *m_Name;
    int m_Spread; // damage spread factor[larger means greater spread]. Damage halves every n pixels from center.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Wall : 1;
    BOOL m_Wood : 1;
    BOOL m_Ore : 1;
    BOOL m_UnkBool : 1;
#else
    bool m_Wall; // Does this warhead damage concrete walls?
    bool m_Wood; // Does this warhead damage wood walls?
    bool m_Ore; // Does this warhead destroy ore?
    bool m_UnkBool; // TODO: Only checked in InfantryClass::Greatest_Threat. Removes targets except infantry and harvester?
#endif
    fixed_t m_Verses[ARMOR_COUNT]; // Damage verses various armor types (as percentage of full damage). eg, Verses[ARMOR_WOOD];
    int m_Explosion; // Which explosion set to use when warhead of this type impacts.
    int m_Death; // Which infantry death animation to use.
};

#ifdef GAME_DLL
extern TFixedIHeapClass<WarheadTypeClass> &g_WarheadTypes;
#else
extern TFixedIHeapClass<WarheadTypeClass> g_WarheadTypes;
#endif

#endif // WARHEADTYPE_H
