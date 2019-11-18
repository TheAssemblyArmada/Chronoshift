/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about terrain overlays.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef OVERLAYTYPE_H
#define OVERLAYTYPE_H

#include "always.h"
#include "heap.h"
#include "land.h"
#include "objecttype.h"
#include "theater.h"

enum OverlayType
{
    // Walls / Fences
    OVERLAY_SANDBAG,
    OVERLAY_CYCLONE_FENCE,
    OVERLAY_BRICK_WALL,
    OVERLAY_BARB_WIRE,
    OVERLAY_WOOD_FENCE,

    // Ore and Gems
    OVERLAY_GOLD_01,
    OVERLAY_GOLD_02,
    OVERLAY_GOLD_03,
    OVERLAY_GOLD_04,
    OVERLAY_GEM_01,
    OVERLAY_GEM_02,
    OVERLAY_GEM_03,
    OVERLAY_GEM_04,

    // Village and farmland
    OVERLAY_V12,
    OVERLAY_V13,
    OVERLAY_V14,
    OVERLAY_V15,
    OVERLAY_V16,
    OVERLAY_V17,
    OVERLAY_V18,

    // CTF flag base/spot    //Flag Pedestal?
    OVERLAY_FPLS,

    // Crates
    OVERLAY_WOOD_CRATE,
    OVERLAY_STEEL_CRATE,

    // Barbwire fence
    OVERLAY_FENCE,

    // Water wooden crate
    OVERLAY_WATER_WOOD_CRATE,

    OVERLAY_COUNT,

    OVERLAY_NONE = -1,
    OVERLAY_FIRST = OVERLAY_SANDBAG,
};

DEFINE_ENUMERATION_OPERATORS(OverlayType);

class GameINIClass;

class OverlayTypeClass : public ObjectTypeClass
{
public:
    OverlayTypeClass(OverlayType overlay, const char *name, int uiname, LandType land, int damage_levels, int strength,
        BOOL a7, BOOL a8, BOOL legal_target, BOOL crushable, BOOL a11, BOOL a12, BOOL a13, BOOL wall, BOOL crate);
    OverlayTypeClass(const OverlayTypeClass &that);
    OverlayTypeClass(const NoInitClass &noinit) : ObjectTypeClass(noinit) {}
    ~OverlayTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual coord_t Coord_Fixup(coord_t coord) const override;
    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
    virtual void Draw_It(int x, int y, int frame) const;
    virtual uint8_t *Radar_Icon(int frame = 0) const;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    LandType Get_Land() const { return m_Land; }
    int Get_Damage_Levels() const { return m_DamageLevels; }
    int Get_Overlay_Strength() const { return m_OverlayStrength; }
    BOOL Is_Wall() const { return m_Wall; }
    BOOL Is_Crate() const { return m_Crate; }
    BOOL Is_Radar_Visible() const { return m_RadarVisible; }
    BOOL Is_Ore() const { return m_Ore; }
    BOOL Is_Theater() const { return m_Theater; }

    OverlayType What_Type() const { return m_Type; }

    static void Init_Heap();
    static void Init(TheaterType theater);
    static void One_Time() {}
    static OverlayType From_Name(const char *name);
    static const char *Name_From(OverlayType overlay);
    static OverlayTypeClass &As_Reference(OverlayType overlay);
    static OverlayTypeClass *As_Pointer(OverlayType overlay);

    static void Prep_For_Add();

private:
    OverlayType m_Type;
    LandType m_Land; // The land type of this overlay.
    int m_DamageLevels; // Number of damage levels [think how many damage stages walls have for example].
    int m_OverlayStrength; // Strength (hit points) of this overlay.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Theater : 1; // 1
    BOOL m_Wall : 1; // 2
    BOOL m_High : 1; // 4
    BOOL m_Ore : 1; // 8
    BOOL m_Boolean16 : 1; // 16
    BOOL m_Crate : 1; // 32
    BOOL m_RadarVisible : 1; // 64
#else
    bool m_Theater; // Does it have theater specific imagery?
    bool m_Wall; // 
    bool m_High; // 
    bool m_Ore; // Is this ore or gem [growth (ore only) and graphic logic applies]?
    bool m_Boolean16; // TODO: this is checked in Explosion_Damage() if warhead has wall or wood to see if destroyed.
    bool m_Crate; // Is this overlay a crate?
    bool m_RadarVisible; // 
#endif
};

#ifdef GAME_DLL
extern TFixedIHeapClass<OverlayTypeClass> &g_OverlayTypes;
#else
extern TFixedIHeapClass<OverlayTypeClass> g_OverlayTypes;
#endif

#endif // OVERLAYTYPE_H
