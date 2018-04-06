/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 * @author AlexB
 *
 * @brief Class containing information about terrain object behaviour.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "terraintype.h"
#include "ccfileclass.h"
#include "coord.h"
#include "lists.h"
#include "mixfile.h"
#include "shape.h"
#include <cstdio>

using std::snprintf;

#ifndef RAPP_STANDALONE
TFixedIHeapClass<TerrainTypeClass> &TerrainTypes = Make_Global<TFixedIHeapClass<TerrainTypeClass> >(0x0065DFD0);
#else
TFixedIHeapClass<TerrainTypeClass> TerrainTypes;
#endif

// Added fix by AlexB (changed MINE from Coord_From_Pixel_XY(12, 24) to Coord_From_Pixel_XY(12, 12)).
const TerrainTypeClass TerrainMine(TERRAIN_MINE, "MINE", TXT_ORE_MINE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(12, 12), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes01(TERRAIN_BOXES01, "BOXES01", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes02(TERRAIN_BOXES02, "BOXES02", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes03(TERRAIN_BOXES03, "BOXES03", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes04(TERRAIN_BOXES04, "BOXES04", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes05(TERRAIN_BOXES05, "BOXES05", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes06(TERRAIN_BOXES06, "BOXES06", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes07(TERRAIN_BOXES07, "BOXES07", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes08(TERRAIN_BOXES08, "BOXES08", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainBoxes09(TERRAIN_BOXES09, "BOXES09", TXT_CRATES, THEATER_BIT_TEMPERATE,
    Coord_From_Pixel_XY(12, 24), true, false, List10, nullptr);
const TerrainTypeClass TerrainIce01(
    TERRAIN_ICE01, "ICE01", TXT_ICE, THEATER_BIT_SNOW, Coord_From_Pixel_XY(24, 24), true, true, List1111, nullptr);
const TerrainTypeClass TerrainIce02(
    TERRAIN_ICE02, "ICE02", TXT_ICE, THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 24), true, true, List1010, nullptr);
const TerrainTypeClass TerrainIce03(
    TERRAIN_ICE03, "ICE03", TXT_ICE, THEATER_BIT_SNOW, Coord_From_Pixel_XY(24, 12), true, true, List11, nullptr);
const TerrainTypeClass TerrainIce04(
    TERRAIN_ICE04, "ICE04", TXT_ICE, THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 12), true, true, List10, nullptr);
const TerrainTypeClass TerrainIce05(
    TERRAIN_ICE05, "ICE05", TXT_ICE, THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 12), true, true, List10, nullptr);
const TerrainTypeClass TerrainTree1(TERRAIN_TREE1, "T01", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(11, 41), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree2(TERRAIN_TREE2, "T02", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(11, 44), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree3(TERRAIN_TREE3, "T03", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(12, 45), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree5(TERRAIN_TREE5, "T05", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(15, 41), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree6(TERRAIN_TREE6, "T06", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(16, 37), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree7(TERRAIN_TREE7, "T07", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(15, 41), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree8(TERRAIN_TREE8, "T08", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(14, 22), false, false, List10, OList01);
const TerrainTypeClass TerrainTree10(TERRAIN_TREE10, "T10", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(25, 43), false, false, List0011, OList1100);
const TerrainTypeClass TerrainTree11(TERRAIN_TREE11, "T11", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(23, 44), false, false, List0011, OList1100);
const TerrainTypeClass TerrainTree12(TERRAIN_TREE12, "T12", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(14, 36), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree13(TERRAIN_TREE13, "T13", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(19, 40), false, false, List0010, OList1101);
const TerrainTypeClass TerrainTree14(TERRAIN_TREE14, "T14", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(19, 40), false, false, List0011, OList1100);
const TerrainTypeClass TerrainTree15(TERRAIN_TREE15, "T15", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(19, 40), false, false, List0011, OList1100);
const TerrainTypeClass TerrainTree16(TERRAIN_TREE16, "T16", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(13, 36), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTree17(TERRAIN_TREE17, "T17", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(18, 44), false, false, List0010, OList1001);
const TerrainTypeClass TerrainTreeClump1(TERRAIN_TREECLUMP1, "TC01", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(28, 41), true, false, List000110, OList110001);
const TerrainTypeClass TerrainTreeClump2(TERRAIN_TREECLUMP2, "TC02", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(38, 41), true, false, List010110, OList101001);
const TerrainTypeClass TerrainTreeClump3(TERRAIN_TREECLUMP3, "TC03", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(33, 35), true, false, List110110, OList001);
const TerrainTypeClass TerrainTreeClump4(TERRAIN_TREECLUMP4, "TC04", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(44, 49), true, false, List000011101000, OList111000010110);
const TerrainTypeClass TerrainTreeClump5(TERRAIN_TREECLUMP5, "TC05", TXT_TREE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(49, 58), true, false, List001011100110, OList110000011001);
const TerrainTypeClass TerrainFlagFly(
    TERRAIN_FLAGFLY, "FLAGFLY", TXT_NULL, THEATER_BIT_ANY, Coord_From_Pixel_XY(15, 41), true, false, List10, OList01);

/**
 * 0x005563EC
 */
TerrainTypeClass::TerrainTypeClass(TerrainType type, const char *name, int uiname, int theater, uint32_t a3, BOOL immune,
    BOOL waterbound, const int16_t *occupy_list, const int16_t *overlap_list) :
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
    // Bit64 = true;	//TS shows what classes set this to true, needs debuging once logics are up and running.
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
    return TerrainTypes.Allocate();
}

/**
 * 0x005564A4
 */
void TerrainTypeClass::operator delete(void *ptr)
{
    TerrainTypes.Free(ptr);
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
BOOL TerrainTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
    // TODO requires TerrainClass
#ifndef RAPP_STANDALONE
    BOOL(*func)(const TerrainTypeClass*, int16_t, HousesType) = reinterpret_cast<BOOL(*)(const TerrainTypeClass*, int16_t, HousesType)>(0x0055B9E4);
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
#ifndef RAPP_STANDALONE
    ObjectClass *(*func)(const TerrainTypeClass*, HouseClass*) = reinterpret_cast<ObjectClass *(*)(const TerrainTypeClass*, HouseClass*)>(0x0055BA1C);
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
uint32_t TerrainTypeClass::Coord_Fixup(uint32_t coord) const
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

    new TerrainTypeClass(TerrainFlagFly);
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

            // TODO, im pretty certain, but needs checking
            if (terrain.Theater & (1 << theater)) {
                snprintf(filename,
                    sizeof(filename),
                    "%s.%s",
                    terrain.ImageName[0] != '\0' ? terrain.ImageName : terrain.Get_Name(),
                    i == TERRAIN_FLAGFLY ? "shp" : g_theaters[theater].ext);
                terrain.ImageData = MixFileClass<CCFileClass>::Retrieve(filename);
                g_isTheaterShape = true;
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
        return As_Reference(terrain).Get_Name();
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
    return TerrainTypes[terrain];
}

/**
 * @brief Get pointer to object from type enum value.
 */
TerrainTypeClass *TerrainTypeClass::As_Pointer(TerrainType terrain)
{
    if (terrain != TERRAIN_NONE && terrain < TERRAIN_COUNT) {
        TerrainTypeClass *ptr = &TerrainTypes[terrain];
        DEBUG_ASSERT(ptr != nullptr);
        return ptr;
    }

    return nullptr;
}
