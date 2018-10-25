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
#include "overlaytype.h"
#include "gamefile.h"
#include "coord.h"
#include "drawshape.h"
#include "globals.h"
#include "lists.h"
#include "mixfile.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<OverlayTypeClass> &OverlayTypes = Make_Global<TFixedIHeapClass<OverlayTypeClass> >(0x0065E01C);
#else
TFixedIHeapClass<OverlayTypeClass> OverlayTypes;
#endif

const OverlayTypeClass OverlaySandbag(
    OVERLAY_SANDBAG, "SBAG", TXT_SANDBAG_WALL, LAND_WALL, 1, 20, true, false, true, true, false, false, false, true, false);
const OverlayTypeClass OverlayCyclone(OVERLAY_CYCLONE_FENCE, "CYCL", TXT_CYCLONE_WALL, LAND_WALL, 2, 10, true, false, true,
    true, false, false, false, true, false);
const OverlayTypeClass OverlayBrick(OVERLAY_BRICK_WALL, "BRIK", TXT_CONCRETE_WALL, LAND_WALL, 3, 70, true, false, true,
    false, false, true, false, true, false);
const OverlayTypeClass OverlayBarbwire(OVERLAY_BARB_WIRE, "BARB", TXT_BARBWIRE_WALL, LAND_WALL, 1, 2, true, false, true,
    true, false, false, false, true, false);
const OverlayTypeClass OverlayWood(
    OVERLAY_WOOD_FENCE, "WOOD", TXT_WOOD_WALL, LAND_WALL, 1, 2, true, true, true, true, false, false, false, true, false);
const OverlayTypeClass OverlayFence(
    OVERLAY_FENC, "FENC", TXT_FENC, LAND_WALL, 2, 10, true, false, true, true, false, false, false, true, false);
const OverlayTypeClass OverlayGold1(
    OVERLAY_GOLD_01, "GOLD01", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayGold2(
    OVERLAY_GOLD_02, "GOLD02", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayGold3(
    OVERLAY_GOLD_03, "GOLD03", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayGold4(
    OVERLAY_GOLD_04, "GOLD04", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayGems1(
    OVERLAY_GEM_01, "GEM01", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayGems2(
    OVERLAY_GEM_02, "GEM02", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayGems3(
    OVERLAY_GEM_03, "GEM03", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayGems4(
    OVERLAY_GEM_04, "GEM04", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);
const OverlayTypeClass OverlayV12(
    OVERLAY_V12, "V12", TXT_HAYSTACKS, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);
const OverlayTypeClass OverlayV13(
    OVERLAY_V13, "V13", TXT_HAYSTACK, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);
const OverlayTypeClass OverlayV14(
    OVERLAY_V14, "V14", TXT_WHEAT_FIELD, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);
const OverlayTypeClass OverlayV15(
    OVERLAY_V15, "V15", TXT_FALLOW_FIELD, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);
const OverlayTypeClass OverlayV16(
    OVERLAY_V16, "V16", TXT_CORN_FIELD, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);
const OverlayTypeClass OverlayV17(
    OVERLAY_V17, "V17", TXT_CELERY_FIELD, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);
const OverlayTypeClass OverlayV18(
    OVERLAY_V18, "V18", TXT_POTATO_FIELD, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);
const OverlayTypeClass OverlayFlagSpot(
    OVERLAY_FPLS, "FPLS", TXT_FLAG_SPOT, LAND_CLEAR, 0, 0, true, false, false, false, false, false, false, false, false);
const OverlayTypeClass OverlayWoodCrate(OVERLAY_WOOD_FENCE_CRATE, "WCRATE", TXT_WOOD_CRATE, LAND_CLEAR, 0, 0, false, false,
    false, false, false, false, false, false, true);
const OverlayTypeClass OverlayWaterCrate(OVERLAY_WATER_WOOD_CRATE, "WWCRATE", TXT_WATER_CRATE, LAND_WATER, 0, 0, false,
    false, false, false, false, false, false, false, true);
const OverlayTypeClass OverlaySteelCrate(OVERLAY_STEEL_CRATE, "SCRATE", TXT_STEEL_CRATE, LAND_CLEAR, 0, 0, false, false,
    false, false, false, false, false, false, true);

OverlayTypeClass::OverlayTypeClass(OverlayType overlay, const char *name, int uiname, LandType land, int damage_levels,
    int strength, BOOL radar_visible, BOOL a8, BOOL legal_target, BOOL crushable, BOOL ore, BOOL high, BOOL theater,
    BOOL wall, BOOL crate) :
    ObjectTypeClass(RTTI_OVERLAYTYPE, overlay, crushable, true, false, legal_target, true, false, false, uiname, name),
    Type(overlay),
    Land(land),
    DamageLevels(damage_levels),
    OverlayStrength(strength),
    Theater(theater),
    Wall(wall),
    High(high),
    Ore(ore),
    Boolean16(a8),
    Crate(crate),
    RadarVisible(radar_visible)
{
}

OverlayTypeClass::OverlayTypeClass(const OverlayTypeClass &that) :
    ObjectTypeClass(that),
    Type(that.Type),
    Land(that.Land),
    DamageLevels(that.DamageLevels),
    OverlayStrength(that.OverlayStrength),
    Theater(that.Theater),
    Wall(that.Wall),
    High(that.High),
    Ore(that.Ore),
    Boolean16(that.Boolean16),
    Crate(that.Crate),
    RadarVisible(that.RadarVisible)
{
}

void *OverlayTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(OverlayTypeClass));
    return OverlayTypes.Allocate();
}

void OverlayTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    OverlayTypes.Free(ptr);
}

void OverlayTypeClass::Init_Heap()
{
    // Allocation order MUST match OverlayTypes enum.
    new OverlayTypeClass(OverlaySandbag);
    new OverlayTypeClass(OverlayCyclone);
    new OverlayTypeClass(OverlayBrick);
    new OverlayTypeClass(OverlayBarbwire);
    new OverlayTypeClass(OverlayWood);
    new OverlayTypeClass(OverlayGold1);
    new OverlayTypeClass(OverlayGold2);
    new OverlayTypeClass(OverlayGold3);
    new OverlayTypeClass(OverlayGold4);
    new OverlayTypeClass(OverlayGems1);
    new OverlayTypeClass(OverlayGems2);
    new OverlayTypeClass(OverlayGems3);
    new OverlayTypeClass(OverlayGems4);
    new OverlayTypeClass(OverlayV12);
    new OverlayTypeClass(OverlayV13);
    new OverlayTypeClass(OverlayV14);
    new OverlayTypeClass(OverlayV15);
    new OverlayTypeClass(OverlayV16);
    new OverlayTypeClass(OverlayV17);
    new OverlayTypeClass(OverlayV18);
    new OverlayTypeClass(OverlayFlagSpot);
    new OverlayTypeClass(OverlayWoodCrate);
    new OverlayTypeClass(OverlaySteelCrate);
    new OverlayTypeClass(OverlayFence);
    new OverlayTypeClass(OverlayWaterCrate);
}

OverlayType OverlayTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return OVERLAY_NONE;
    }

    if (name != nullptr) {
        for (OverlayType overlay = OVERLAY_FIRST; overlay < OVERLAY_COUNT; ++overlay) {
            if (strcasecmp(name, Name_From(overlay)) == 0) {
                return overlay;
            }
        }
    }

    return OVERLAY_NONE;
}

const char *OverlayTypeClass::Name_From(OverlayType overlay)
{
    DEBUG_ASSERT(overlay != OVERLAY_NONE);
    DEBUG_ASSERT(overlay < OVERLAY_COUNT);

    return overlay != OVERLAY_NONE && overlay < OVERLAY_COUNT ? As_Reference(overlay).Get_Name() : "<none>";
}

void OverlayTypeClass::Init(TheaterType theater)
{
    DEBUG_ASSERT(theater < THEATER_COUNT);
    DEBUG_ASSERT(theater != THEATER_NONE);

    char filename[512];

    if (theater != g_lastTheater) {
        for (OverlayType i = OVERLAY_FIRST; i < OVERLAY_COUNT; ++i) {
            OverlayTypeClass &overlay = As_Reference(i);

            snprintf(filename,
                sizeof(filename),
                "%s.%s",
                overlay.Get_Name(),
                overlay.Theater ? g_theaters[theater].ext : "shp");
            overlay.ImageData = MixFileClass<GameFileClass>::Retrieve(filename);

            g_isTheaterShape = overlay.Theater;
            overlay.RadarIconData = Get_Radar_Icon(overlay.ImageData, 0, -1, 3);
            g_isTheaterShape = false;
        }
    }
}

OverlayTypeClass &OverlayTypeClass::As_Reference(OverlayType overlay)
{
    DEBUG_ASSERT(overlay < OVERLAY_COUNT);
    DEBUG_ASSERT(&OverlayTypes[overlay] != nullptr);

    return OverlayTypes[overlay];
}

OverlayTypeClass *OverlayTypeClass::As_Pointer(OverlayType overlay)
{
    DEBUG_ASSERT(overlay != OVERLAY_NONE);
    DEBUG_ASSERT(overlay < OVERLAY_COUNT);

    return overlay < OVERLAY_COUNT && overlay != OVERLAY_NONE ? &OverlayTypes[overlay] : nullptr;
}

uint32_t OverlayTypeClass::Coord_Fixup(uint32_t coord) const
{
    return Coord_Top_Left(coord);
}

BOOL OverlayTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
    // TODO requires OverlayClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(const OverlayTypeClass*, int16_t, HousesType) = reinterpret_cast<BOOL(*)(const OverlayTypeClass*, int16_t, HousesType)>(0x00524A5C);
    return func(this, cellnum, house);
#elif 0
    DEBUG_ASSERT(this != nullptr);

    return new OverlayClass(Type, cellnum, house) != nullptr;
#else
    return false;
#endif
}

ObjectClass *OverlayTypeClass::Create_One_Of(HouseClass *house) const
{
    // TODO requires OverlayClass
#ifndef CHRONOSHIFT_STANDALONE
    ObjectClass *(*func)(const OverlayTypeClass*, HouseClass*) = reinterpret_cast<ObjectClass *(*)(const OverlayTypeClass*, HouseClass*)>(0x00524A98);
    return func(this, house);
#elif 0
    DEBUG_ASSERT(this != nullptr);
    DEBUG_ASSERT(house != nullptr);

    return new OverlayClass(Type);
#else
    return nullptr;
#endif
}

const int16_t *OverlayTypeClass::Occupy_List(BOOL a1) const
{
    static const int16_t _simple[] = { 0, LIST_END };

    return _simple;
}

void OverlayTypeClass::Draw_It(int x, int y, int frame) const
{
    // TODO Needs DisplayClass
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const OverlayTypeClass*, int, int, int) = reinterpret_cast<void(*)(const OverlayTypeClass*, int, int, int)>(0x00524ACC);
    func(this, x, y, frame);
#elif 0
    if (ImageData != nullptr) {
        g_isTheaterShape = Theater;
        CC_Draw_Shape(ImageData,
            frame,
            x + Map.TacOffsetX + 12,
            y + Map.TacOffsetY + 12,
            WINDOW_TACTICAL,
            SHAPE_GHOST | SHAPE_WIN_REL | SHAPE_CENTER,
            0,
            DisplayClass::UnitShadow);
        g_isTheaterShape = false;
    }
#endif
}

uint8_t *OverlayTypeClass::Radar_Icon(int frame) const
{
    if (RadarIconData != nullptr) {
        // TODO, correct casting, find out what stuct it is casting to.
        // Each radar icon is 3 x 3 bytes (hence 9 * frame number to get to relevant frame).
        // + 2 appears to account for some kind of header for when the data is generated.
        return (static_cast<uint8_t *>(RadarIconData) + 9 * frame + 2);
    }

    return nullptr;
}

#if 0
BOOL OverlayTypeClass::Read_INI(GameINIClass &ini)
{
    //
    //
    //
    if (ObjectTypeClass::Read_INI(ini)) {
        // The land type of this overlay [def = LAND_CLEAR].
        Land = ini.Get_LandType(Get_Name(), "Land", Land);

        // Strength (hit points) of this overlay.
        OverlayStrength = ini.Get_Int(Get_Name(), "Strength", OverlayStrength);

        //
        Wall = ini.Get_BOOL(Get_Name(), "Wall", Wall);

        //
        High = ini.Get_BOOL(Get_Name(), "High", High);

        // Is this ore or gem [growth (ore only) and graphic logic applies] (def = false)?
        Ore = ini.Get_BOOL(Get_Name(), "Ore", Ore);

        // Is this overlay a crate (def = false)?
        Crate = ini.Get_BOOL(Get_Name(), "Crate", Crate);

        //
        DamageLevels = ini.Get_Int(Get_Name(), "DamageLevels", DamageLevels);

        // Does it have theater specific imagery (def = false)?
        Theater = ini.Get_BOOL(Get_Name(), "Theater", Theater);

        // <TODO> (def = ANIM_NONE)?
        CellAnim = ini.Get_AnimType(Get_Name(), "CellAnim", CellAnim);

        //
        //
        //
        return true;
    }

    //
    //
    //
    return false;
}
#endif
