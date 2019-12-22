/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on infantry objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef INFANTRYTYPE_H
#define INFANTRYTYPE_H

#include "always.h"
#include "heap.h"
#include "technotype.h"

enum InfantryType
{
    INFANTRY_E1, // Rifle Infantry
    INFANTRY_E2, // Grenadier
    INFANTRY_E3, // Rocket Soldier
    INFANTRY_FLAME_TROOPER,
    INFANTRY_ENGINEER,
    INFANTRY_TANYA,
    INFANTRY_SPY,
    INFANTRY_THIEF,
    INFANTRY_MEDIC,
    INFANTRY_GENERAL,
    INFANTRY_DOG,
    INFANTRY_C1,
    INFANTRY_C2,
    INFANTRY_C3,
    INFANTRY_C4,
    INFANTRY_C5,
    INFANTRY_C6,
    INFANTRY_C7, // Technician
    INFANTRY_C8,
    INFANTRY_C9,
    INFANTRY_C10,
    INFANTRY_EINSTEIN,
    INFANTRY_DELPHI,
    INFANTRY_CHAN,
    INFANTRY_SHOCK_TROOPER,
    INFANTRY_MECHANIC,

    INFANTRY_COUNT,

    INFANTRY_NONE = -1,
    INFANTRY_FIRST = INFANTRY_E1,

    INFANTRY_NOEXP_COUNT = 24, // Used for arrays that exclude new expansion infantry.
};

DEFINE_ENUMERATION_OPERATORS(InfantryType);

enum DoType
{
    DO_READY,
    DO_GUARD,
    DO_PRONE,
    DO_WALK,
    DO_FIRE_UP,
    DO_DOWN,
    DO_CRAWL,
    DO_UP,
    DO_FIRE_PRONE,
    DO_IDLE1,
    DO_IDLE2,
    DO_DIE1,
    DO_DIE2,
    DO_DIE3,
    DO_DIE4,
    DO_DIE5,
    DO_16,
    DO_17,
    DO_18,
    DO_19,
    DO_20,

    DO_COUNT,

    DO_NONE = -1,
    DO_FIRST = DO_READY,
};

DEFINE_ENUMERATION_OPERATORS(DoType);

struct DoInfoStruct
{
    int m_StartingFrame; // Where in the shape does it start.
    uint8_t m_NumFrames; // How many frames does the anim last.
    uint8_t m_FacingMultiplier; // How many frames to the next facing.
};

class InfantryTypeClass : public TechnoTypeClass
{
public:
    InfantryTypeClass(InfantryType type, int uiname, const char *name, int def_fire_coord, int rot_count, BOOL female,
        BOOL crawls, BOOL civilian, BOOL has_alt_remap, BOOL nominal, BOOL theater, PipEnum pip, const DoInfoStruct *do_info,
        int fire_up, int fire_prone, const uint8_t *alt_remap);
    InfantryTypeClass(const InfantryTypeClass &that);
    InfantryTypeClass(const NoInitClass &noinit) : TechnoTypeClass(noinit) {}
    ~InfantryTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual int Full_Name() const override;
    virtual void Dimensions(int &w, int &h) const override;
    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
    virtual BOOL Read_INI(GameINIClass &ini) override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    InfantryType What_Type() const { return m_Type; }

    BOOL Is_Infiltrator() const { return m_IsInfiltrator; }
    BOOL Is_Fraidycat() const { return m_IsFraidycat; }
    BOOL Is_Civilian() const { return m_IsCivilian; }
    BOOL Has_C4() const { return m_HasC4; }
    BOOL Is_Canine() const { return m_IsCanine; }
    BOOL Has_Alt_Remap() const { return m_HasAltRemap; }

    const uint8_t *Alt_Remap_Table() const { return m_AltRemap; }

    const DoInfoStruct &Fetch_DoInfo(DoType type) const { return m_DoControl[type]; }

    static void Init_Heap();
    static InfantryTypeClass &As_Reference(InfantryType type);
    static InfantryTypeClass *As_Pointer(InfantryType type);
    static InfantryType From_Name(const char *name);
    static const char *Name_From(InfantryType type) { return As_Reference(type).m_Name; }

    static void One_Time();
    static void Prep_For_Add();

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_FemaleVoice : 1; // & 1 Uses the civilian female voice (def = false)?
    BOOL m_IsCrawler : 1; // & 2 Does this infantry crawl (go prone) when under fire (def = false)?
    BOOL m_IsInfiltrator : 1; // & 4 Can it enter a building like a spy or thief (def = false)?
    BOOL m_IsFraidycat : 1; // & 8 Is it inherently afraid and will panic easily (def = false)?
    BOOL m_IsCivilian : 1; // & 16 Counts a civilian for evac and kill tracking (def = false)?
    BOOL m_HasC4 : 1; // & 32 Equipped with explosives [presumes Infiltrate is also true] (def = false)?
    BOOL m_IsCanine : 1; // & 64 Should special case dog logic be applied to this (def = false)?
    BOOL m_HasAltRemap : 1; // & 128
#else
    bool m_FemaleVoice; // Uses the civilian female voice (def = false)?
    bool m_IsCrawler; // Does this infantry crawl (go prone) when under fire (def = false)?
    bool m_IsInfiltrator; // Can it enter a building like a spy or thief (def = false)?
    bool m_IsFraidycat; // Is it inherently afraid and will panic easily (def = false)?
    bool m_IsCivilian; // Counts a civilian for evac and kill tracking (def = false)?
    bool m_HasC4; // Equipped with explosives [presumes Infiltrate is also true] (def = false)?
    bool m_IsCanine; // Should special case dog logic be applied to this (def = false)?
    bool m_HasAltRemap;
#endif
    InfantryType m_Type;
    PipEnum m_Pip;
    const DoInfoStruct *m_DoControl;
    uint8_t m_FireUp;
    uint8_t m_FireProne;
    const uint8_t *m_AltRemap;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<InfantryTypeClass> &g_InfantryTypes;
#else
extern TFixedIHeapClass<InfantryTypeClass> g_InfantryTypes;
#endif

#endif // INFANTRYTYPE_H
