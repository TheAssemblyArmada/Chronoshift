/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 * @author AlexB
 *
 * @brief Class containing information about terrain object behaviour.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "terraintype.h"
#include "terraindata.h"
#include "gamefile.h"
#include "coord.h"
#include "lists.h"
#include "mixfile.h"
#include "shape.h"
#include <cstdio>

using std::snprintf;

#ifdef GAME_DLL
#else
TFixedIHeapClass<TerrainTypeClass> g_TerrainTypes;
#endif

/**
 * 0x005563EC
 */
TerrainTypeClass::TerrainTypeClass(TerrainType type, int theater, uint32_t a3, BOOL immune, BOOL waterbound,
    const char *name, int uiname, const int16_t *occupy_list, const int16_t *overlap_list) :
    ObjectTypeClass(RTTI_TERRAINTYPE, type, true, true, false, false, true, immune, true, uiname, name),
    Type(type),
    UnkInt(a3),
    Theater(theater),
    WaterBound(waterbound),
    OccupyList(occupy_list),
    OverlapList(overlap_list)
{
    Strength = 800;
    Armor = ARMOR_WOOD;
    // Bit64 = true;    //TS shows what classes set this to true, needs debuging once logics are up and running.
}

/**
 * 0x0055BAC0
 */
TerrainTypeClass::TerrainTypeClass(TerrainTypeClass const &that) :
    ObjectTypeClass(that),
    Type(that.Type),
    UnkInt(that.UnkInt),
    Theater(that.Theater),
    WaterBound(that.WaterBound),
    OccupyList(that.OccupyList),
    OverlapList(that.OverlapList)
{
}

/**
 * 0x0055BD10
 */
TerrainTypeClass::~TerrainTypeClass()
{
    OccupyList = nullptr;
    OverlapList = nullptr;
}

/**
 * 0x00556490
 */
void *TerrainTypeClass::operator new(size_t size)
{
    return g_TerrainTypes.Allocate();
}

/**
 * 0x005564A4
 */
void TerrainTypeClass::operator delete(void *ptr)
{
    g_TerrainTypes.Free(ptr);
}

/**
 * @brief Gets the occupy list for this object to calculate the cells it covers logically.
 *
 * 0x0055BA4C
 */
const int16_t *TerrainTypeClass::Occupy_List(BOOL a1) const
{
    static int16_t _simple[] = { LIST_END };

    if (OccupyList != nullptr) {
        return OccupyList;
    }

    return _simple;
}

/**
 * @brief Gets the overlap list for this object to calculate the cells it covers visually.
 *
 * 0x0055BA68
 */
const int16_t *TerrainTypeClass::Overlap_List() const
{
    static const int16_t _simple[] = { LIST_END };

    if (OverlapList != nullptr) {
        return OverlapList;
    }

    return _simple;
}

/**
 * @brief Creates an instance of TerrainClass at the specified cell for the specified house.
 *
 * 0x0055B9E4
 */
BOOL TerrainTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
    // TODO requires TerrainClass
#ifdef GAME_DLL
    BOOL(*func)
    (const TerrainTypeClass *, cell_t, HousesType) =
        reinterpret_cast<BOOL (*)(const TerrainTypeClass *, cell_t, HousesType)>(0x0055B9E4);
    return func(this, cellnum, house);
#elif 0
    return new TerrainClass(Type, Cell_To_Coord(cellnum)) != nullptr;
#else
    return false;
#endif
}

/**
 * @brief Creates an instance of TerrainClass for the specified house.
 *
 * 0x0055BA1C
 */
ObjectClass *TerrainTypeClass::Create_One_Of(HouseClass *house) const
{
    // TODO requires TerrainClass
#ifdef GAME_DLL
    ObjectClass *(*func)(const TerrainTypeClass *, HouseClass *) =
        reinterpret_cast<ObjectClass *(*)(const TerrainTypeClass *, HouseClass *)>(0x0055BA1C);
    return func(this, house);
#elif 0
    DEBUG_ASSERT(house != nullptr);

    return new TerrainClass(Type);
#else
    return nullptr;
#endif
}

/**
 * @brief Fixes up a coord to point to the top left corner of the cell it is within.
 *
 * 0x0055BAA0
 */
coord_t TerrainTypeClass::Coord_Fixup(coord_t coord) const
{
    return Coord_Top_Left(coord);
}

/**
 * @brief Initialises the object heap.
 *
 * 0x005564BC
 */
void TerrainTypeClass::Init_Heap()
{
    // Heap init order should match the enum order in terrain.h
    new TerrainTypeClass(TerrainTree1);
    new TerrainTypeClass(TerrainTree2);
    new TerrainTypeClass(TerrainTree3);
    new TerrainTypeClass(TerrainTree5);
    new TerrainTypeClass(TerrainTree6);
    new TerrainTypeClass(TerrainTree7);
    new TerrainTypeClass(TerrainTree8);
    new TerrainTypeClass(TerrainTree10);
    new TerrainTypeClass(TerrainTree11);
    new TerrainTypeClass(TerrainTree12);
    new TerrainTypeClass(TerrainTree13);
    new TerrainTypeClass(TerrainTree14);
    new TerrainTypeClass(TerrainTree15);
    new TerrainTypeClass(TerrainTree16);
    new TerrainTypeClass(TerrainTree17);

    new TerrainTypeClass(TerrainTreeClump1);
    new TerrainTypeClass(TerrainTreeClump2);
    new TerrainTypeClass(TerrainTreeClump3);
    new TerrainTypeClass(TerrainTreeClump4);
    new TerrainTypeClass(TerrainTreeClump5);

    new TerrainTypeClass(TerrainIce01);
    new TerrainTypeClass(TerrainIce02);
    new TerrainTypeClass(TerrainIce03);
    new TerrainTypeClass(TerrainIce04);
    new TerrainTypeClass(TerrainIce05);

    new TerrainTypeClass(TerrainBoxes01);
    new TerrainTypeClass(TerrainBoxes02);
    new TerrainTypeClass(TerrainBoxes03);
    new TerrainTypeClass(TerrainBoxes04);
    new TerrainTypeClass(TerrainBoxes05);
    new TerrainTypeClass(TerrainBoxes06);
    new TerrainTypeClass(TerrainBoxes07);
    new TerrainTypeClass(TerrainBoxes08);
    new TerrainTypeClass(TerrainBoxes09);

    new TerrainTypeClass(TerrainMine);

    //new TerrainTypeClass(TerrainFlagFly);
}

/**
 * @brief Initialises theater specific data.
 *
 * 0x0055B8A8
 */
void TerrainTypeClass::Init(TheaterType theater)
{
    DEBUG_ASSERT(theater != THEATER_NONE);
    DEBUG_ASSERT(theater < THEATER_COUNT);

    char filename[512];

    if (theater != g_lastTheater) {
        for (TerrainType i = TERRAIN_FIRST; i < TERRAIN_COUNT; ++i) {
            TerrainTypeClass &terrain = As_Reference(i);
            terrain.ImageData = nullptr;

            // TODO, im pretty certain, but needs checking
            if (terrain.Theater & (1 << theater)) {
                snprintf(filename,
                    sizeof(filename),
                    "%s.%s",
                    /*terrain.ImageName[0] != '\0' ? terrain.ImageName :*/ terrain.m_Name,
                    /*i == TERRAIN_FLAGFLY ? "shp" :*/ g_theaters[theater].ext);
                terrain.ImageData = GameFileClass::Retrieve(filename);
                g_isTheaterShape = true;

                if (terrain.RadarIconData != nullptr) {
                    delete[] static_cast<char *>(terrain.RadarIconData);
                }

                terrain.RadarIconData = Get_Radar_Icon(terrain.ImageData, 0, 1, 3);
                g_isTheaterShape = false;
            }
        }
    }
}

/**
 * @brief Get type enum value from string.
 *
 * 0x0055B994
 */
TerrainType TerrainTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return TERRAIN_NONE;
    }

    if (name != nullptr) {
        for (TerrainType terrain = TERRAIN_FIRST; terrain < TERRAIN_COUNT; ++terrain) {
            if (strcasecmp(name, Name_From(terrain)) == 0) {
                return terrain;
            }
        }
    }
    return TERRAIN_NONE;
}

/**
 * @brief Get name string from type enum value.
 */
const char *TerrainTypeClass::Name_From(TerrainType terrain)
{
    if (terrain != TERRAIN_NONE && terrain < TERRAIN_COUNT) {
        return As_Reference(terrain).m_Name;
    }

    return "<none>";
}

/**
 * @brief Get reference to object from type enum value.
 *
 * 0x0055BA88
 */
TerrainTypeClass &TerrainTypeClass::As_Reference(TerrainType terrain)
{
    return g_TerrainTypes[terrain];
}

/**
 * @brief Get pointer to object from type enum value.
 */
TerrainTypeClass *TerrainTypeClass::As_Pointer(TerrainType terrain)
{
    if (terrain != TERRAIN_NONE && terrain < TERRAIN_COUNT) {
        TerrainTypeClass *ptr = &g_TerrainTypes[terrain];
        DEBUG_ASSERT(ptr != nullptr);
        return ptr;
    }

    return nullptr;
}
