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
#include "overlay.h"
#include "overlaydata.h"
#include "gamefile.h"
#include "coord.h"
#include "drawshape.h"
#include "globals.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include <cstdio>

using std::snprintf;

#ifndef GAME_DLL
TFixedIHeapClass<OverlayTypeClass> g_OverlayTypes;
#endif

OverlayTypeClass::OverlayTypeClass(OverlayType overlay, const char *name, int uiname, LandType land, int damage_levels,
    int strength, BOOL radar_visible, BOOL a8, BOOL legal_target, BOOL crushable, BOOL ore, BOOL high, BOOL theater,
    BOOL wall, BOOL crate) :
    ObjectTypeClass(RTTI_OVERLAYTYPE, overlay, crushable, true, false, legal_target, true, false, false, uiname, name),
    m_Type(overlay),
    m_Land(land),
    m_DamageLevels(damage_levels),
    m_OverlayStrength(strength),
    m_Theater(theater),
    m_Wall(wall),
    m_High(high),
    m_Ore(ore),
    m_Boolean16(a8),
    m_Crate(crate),
    m_RadarVisible(radar_visible)
{
}

OverlayTypeClass::OverlayTypeClass(const OverlayTypeClass &that) :
    ObjectTypeClass(that),
    m_Type(that.m_Type),
    m_Land(that.m_Land),
    m_DamageLevels(that.m_DamageLevels),
    m_OverlayStrength(that.m_OverlayStrength),
    m_Theater(that.m_Theater),
    m_Wall(that.m_Wall),
    m_High(that.m_High),
    m_Ore(that.m_Ore),
    m_Boolean16(that.m_Boolean16),
    m_Crate(that.m_Crate),
    m_RadarVisible(that.m_RadarVisible)
{
}

void *OverlayTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(OverlayTypeClass));
    return g_OverlayTypes.Allocate();
}

void OverlayTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_OverlayTypes.Free(ptr);
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

    return overlay != OVERLAY_NONE && overlay < OVERLAY_COUNT ? As_Reference(overlay).m_Name : "<none>";
}

void OverlayTypeClass::Init(TheaterType theater)
{
    DEBUG_ASSERT(theater < THEATER_COUNT);
    DEBUG_ASSERT(theater != THEATER_NONE);

    char filename[512];

    if (theater != g_LastTheater) {
        for (OverlayType i = OVERLAY_FIRST; i < OVERLAY_COUNT; ++i) {
            OverlayTypeClass &overlay = As_Reference(i);

            snprintf(filename,
                sizeof(filename),
                "%s.%s",
                overlay.m_Name,
                overlay.m_Theater ? g_Theaters[theater].ext : "shp");
            overlay.m_ImageData = GameFileClass::Retrieve(filename);

            g_IsTheaterShape = overlay.m_Theater;
            overlay.m_RadarIconData = Get_Radar_Icon(overlay.m_ImageData, 0, -1, 3);
            g_IsTheaterShape = false;
        }
    }
}

OverlayTypeClass &OverlayTypeClass::As_Reference(OverlayType overlay)
{
    return g_OverlayTypes[overlay];
}

OverlayTypeClass *OverlayTypeClass::As_Pointer(OverlayType overlay)
{
    return overlay < OVERLAY_COUNT && overlay != OVERLAY_NONE ? &g_OverlayTypes[overlay] : nullptr;
}

/**
 * @brief 
 *
 * @address 0x00549CF8 (beta)
 */
void OverlayTypeClass::Prep_For_Add()
{
    for (OverlayType i = OVERLAY_FIRST; i < OVERLAY_COUNT; ++i) {
        OverlayTypeClass *otptr = As_Pointer(i);
        if (otptr != nullptr) {
            if (otptr->m_ImageData != nullptr) {
                if (!otptr->m_Wall && (!otptr->m_Ore || i == OVERLAY_GOLD_01 || i == OVERLAY_GEM_01)) {
                    g_Map.Add_To_List(otptr);
                }
            }
        }
    }
}

coord_t OverlayTypeClass::Coord_Fixup(coord_t coord) const
{
    return Coord_Top_Left(coord);
}

BOOL OverlayTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
    return new OverlayClass(m_Type, cellnum, house) != nullptr;
}

ObjectClass *OverlayTypeClass::Create_One_Of(HouseClass *house) const
{
    return new OverlayClass(m_Type);
}

const int16_t *OverlayTypeClass::Occupy_List(BOOL a1) const
{
    static const int16_t _simple[] = { 0, LIST_END };

    return _simple;
}

void OverlayTypeClass::Draw_It(int x, int y, int frame) const
{
    if (m_ImageData != nullptr) {
        g_IsTheaterShape = m_Theater;
        CC_Draw_Shape(m_ImageData,
            frame,
            x + g_Map.Tac_Offset_X() + 12,
            y + g_Map.Tac_Offset_Y() + 12,
            WINDOW_0,
            SHAPE_GHOST | SHAPE_WIN_REL | SHAPE_CENTER,
            0,
            DisplayClass::s_UnitShadow);
        g_IsTheaterShape = false;
    }
}

uint8_t *OverlayTypeClass::Radar_Icon(int frame) const
{
    if (m_RadarIconData != nullptr) {
        // TODO, correct casting, find out what stuct it is casting to.
        // Each radar icon is 3 x 3 bytes (hence 9 * frame number to get to relevant frame).
        // + 2 appears to account for some kind of header for when the data is generated.
        return (static_cast<uint8_t *>(m_RadarIconData) + 9 * frame + 2);
    }

    return nullptr;
}

#if 0
BOOL OverlayTypeClass::Read_INI(GameINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        m_Land = ini.Get_LandType(m_Name, "Land", m_Land);
        m_OverlayStrength = ini.Get_Int(m_Name, "Strength", m_OverlayStrength);
        m_Wall = ini.Get_BOOL(m_Name, "Wall", m_Wall);
        m_High = ini.Get_BOOL(m_Name, "High", m_High);
        m_Ore = ini.Get_BOOL(m_Name, "Ore", m_Ore);
        m_Crate = ini.Get_BOOL(m_Name, "Crate", m_Crate);
        m_DamageLevels = ini.Get_Int(m_Name, "DamageLevels", m_DamageLevels);
        m_Theater = ini.Get_BOOL(m_Name, "Theater", m_Theater);
        CellAnim = ini.Get_AnimType(m_Name, "CellAnim", CellAnim);

        return true;
    }

    return false;
}
#endif
