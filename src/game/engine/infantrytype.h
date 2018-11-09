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
    INFANTRY_NONE = -1,
    INFANTRY_FIRST = 0,
    INFANTRY_E1 = 0, // Rifle Infantry
    INFANTRY_E2 = 1, // Grenadier
    INFANTRY_E3 = 2, // Rocket Soldier
    INFANTRY_FLAME_TROOPER = 3,
    INFANTRY_ENGINEER = 4,
    INFANTRY_TANYA = 5,
    INFANTRY_SPY = 6,
    INFANTRY_THIEF = 7,
    INFANTRY_MEDIC = 8,
    INFANTRY_GENERAL = 9,
    INFANTRY_DOG = 10,
    INFANTRY_C1 = 11,
    INFANTRY_C2 = 12,
    INFANTRY_C3 = 13,
    INFANTRY_C4 = 14,
    INFANTRY_C5 = 15,
    INFANTRY_C6 = 16,
    INFANTRY_C7 = 17,
    INFANTRY_C8 = 18,
    INFANTRY_C9 = 19,
    INFANTRY_C10 = 20,
    INFANTRY_EINSTEIN = 21,
    INFANTRY_DELPHI = 22,
    INFANTRY_CHAN = 23,
    INFANTRY_SHOCK_TROOPER = 24,
    INFANTRY_MECHANIC = 25,
    INFANTRY_COUNT = 26
};

DEFINE_ENUMERATION_OPERATORS(InfantryType);

enum DoType
{
    DO_NONE = -1,
    DO_FIRST = 0,
    DO_READY = 0,
    DO_GUARD = 1,
    DO_PRONE = 2,
    DO_WALK = 3,
    DO_FIRE_UP = 4,
    DO_DOWN = 5,
    DO_CRAWL = 6,
    DO_UP = 7,
    DO_FIRE_PRONE = 8,
    DO_IDLE1 = 9,
    DO_IDLE2 = 10,
    DO_DIE1 = 11,
    DO_DIE2 = 12,
    DO_DIE3 = 13,
    DO_DIE4 = 14,
    DO_DIE5 = 15,
    DO_16 = 16,
    DO_17 = 17,
    DO_18 = 18,
    DO_19 = 19,
    DO_20 = 20,
    DO_COUNT = 21
};

DEFINE_ENUMERATION_OPERATORS(DoType);

struct DoInfoStruct
{
    int m_StartingFrames; // Where in the shape does it start.
    uint8_t m_NumFrames; // How many frames does the anim last.
    uint8_t m_FacingMultiplier; // How many frames to the next facing.
};

class InfantryTypeClass : public TechnoTypeClass
{
public:
    InfantryTypeClass(InfantryType type, int uiname, const char *name, int def_fire_coord, int rot_count, BOOL female,
        BOOL crawls, BOOL civilian, BOOL alt_remap, BOOL nominal, BOOL theater, PipEnum pip, DoInfoStruct *sequence,
        int fire_up, int fire_prone, const uint8_t *remap);
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
    
    static InfantryTypeClass &As_Reference(InfantryType type);
    static InfantryType From_Name(const char *name);
    static const char *Name_From(InfantryType type) { return As_Reference(type).Name; }

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_FemaleVoice : 1; // & 1 Uses the civilian female voice (def = false)?
            bool m_IsCrawler : 1; // & 2 Does this infantry crawl (go prone) when under fire (def = false)?
            bool m_IsInfiltrator : 1; // & 4 Can it enter a building like a spy or thief (def = false)?
            bool m_IsFraidycat : 1; // & 8 Is it inherently afraid and will panic easily (def = false)?
            bool m_IsCivilian : 1; // & 16 Counts a civilian for evac and kill tracking (def = false)?
            bool m_HasC4 : 1; // & 32 Equipped with explosives [presumes Infiltrate is also true] (def = false)?
            bool m_IsCanine : 1; // & 64 Should special case dog logic be applied to this (def = false)?
            bool m_HasAltRemap : 1; // & 128
        };
        int Bitfield;
    };
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
    DoInfoStruct *m_DoControl;
    uint8_t m_FireUp;
    uint8_t m_FireProne;
    const uint8_t *m_Remap;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<InfantryTypeClass> &g_InfantryTypes;
#else
extern TFixedIHeapClass<InfantryTypeClass> g_InfantryTypes;
#endif

#endif // INFANTRYTYPE_H
