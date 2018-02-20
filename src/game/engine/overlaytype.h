/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about terrain overlays.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
    OVERLAY_NONE = -1,

    OVERLAY_FIRST = 0,

    // Walls / Fences
    OVERLAY_SANDBAG = 0,
    OVERLAY_CYCLONE_FENCE = 1,
    OVERLAY_BRICK_WALL = 2,
    OVERLAY_BARB_WIRE = 3,
    OVERLAY_WOOD_FENCE = 4,

    // Ore and Gems
    OVERLAY_GOLD_01 = 5,
    OVERLAY_GOLD_02 = 6,
    OVERLAY_GOLD_03 = 7,
    OVERLAY_GOLD_04 = 8,
    OVERLAY_GEM_01 = 9,
    OVERLAY_GEM_02 = 10,
    OVERLAY_GEM_03 = 11,
    OVERLAY_GEM_04 = 12,

    // Village and farmland
    OVERLAY_V12 = 13,
    OVERLAY_V13 = 14,
    OVERLAY_V14 = 15,
    OVERLAY_V15 = 16,
    OVERLAY_V16 = 17,
    OVERLAY_V17 = 18,
    OVERLAY_V18 = 19,

    // CTF flag base/spot	//Flag Pedestal?
    OVERLAY_FPLS = 20,

    // Crates
    OVERLAY_WOOD_FENCE_CRATE = 21,
    OVERLAY_STEEL_CRATE = 22,

    // Barbwire fence
    OVERLAY_FENC = 23,

    // Water wooden crate
    OVERLAY_WATER_WOOD_CRATE = 24,

    OVERLAY_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(OverlayType);

class CCINIClass;

class OverlayTypeClass : public ObjectTypeClass
{
public:
    OverlayTypeClass(OverlayType overlay, const char *name, int uiname, LandType land, int damage_levels, int strength, BOOL a7, BOOL a8, BOOL legal_target, BOOL crushable, BOOL a11, BOOL a12, BOOL a13, BOOL wall, BOOL crate);
    OverlayTypeClass(OverlayTypeClass const &that);
    OverlayTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit) {}
    ~OverlayTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual uint32_t Coord_Fixup(uint32_t coord) const override;
    virtual BOOL Create_And_Place(int16_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual int16_t *Occupy_List(BOOL a1 = false) const override;
    virtual void Draw_It(int x, int y, int frame) const;
    virtual uint8_t *Radar_Icon(int frame = 0) const;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    LandType Get_Land() { return Land; }
    int Get_Damage_Levels() { return DamageLevels; }
    int Get_Overlay_Strength() { return OverlayStrength; }
    BOOL Is_Wall() { return Wall; }

    static void Init_Heap();
    static void Init(TheaterType theater);
    static void One_Time() {}
    static OverlayType From_Name(const char *name);
    static const char *Name_From(OverlayType overlay);
    static OverlayTypeClass &As_Reference(OverlayType overlay);
    static OverlayTypeClass *As_Pointer(OverlayType overlay);

private:
    OverlayType Type;
    LandType Land;
    int DamageLevels; // think how many damage stages walls have for example.
    int OverlayStrength;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Theater : 1; // 1
            bool Wall : 1; // 2
            bool High : 1; // 4
            bool Ore : 1; // 8
            bool Boolean16 : 1; // 16		//this is checked in Explosion_Damage() if warhead has wall or wood to see if destroyed.
            bool Crate : 1; // 32
            bool RadarVisible : 1; // 64
        };
        int Bitfield;
    };
#else
    bool Theater; // 1
    bool Wall; // 2
    bool High; // 4
    bool Ore; // 8
    bool Boolean16; // 16		//this is checked in Explosion_Damage() if warhead has wall or wood to see if destroyed.
    bool Crate; // 32
    bool RadarVisible; // 64
#endif
};

#ifndef RAPP_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<OverlayTypeClass> &OverlayTypes;
#else
extern TFixedIHeapClass<OverlayTypeClass> OverlayTypes;
#endif

#endif // OVERLAYTYPE_H
