/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling map loading and logic.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "map.h"
#include "abs.h"
#include "coord.h"
#include "globals.h"
#include "lcwpipe.h"
#include "lcwstraw.h"
#include "lists.h"
#include "minmax.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "target.h"
#include "tileset.h"

// TODO this is require for some none virtual calls to functions higher in the stack.
#include "iomap.h"

//This appears to be missing the entries for 309+ which TS has for sight 11
const int MapClass::RadiusOffset[] = {
    0, -129, -128, -127, -1, 1, 127, 128,
    129, -257, -256, -255, -130, -126, -2, 2,
    126, 130, 255, 256, 257, -385, -384, -383,
    -258, -254, -131, -125, -3, 3, 125, 131,
    254, 258, 383, 384, 385, -513, -512, -511,
    -387, -386, -382, -381, -259, -253, -132, -124,
    -4, 4, 124, 132, 253, 259, 381, 382,
    386, 387, 511, 512, 513, -641, -640, -639,
    -515, -514, -510, -509, -388, -380, -260, -252,
    -133, -123, -5, 5, 123, 133, 252, 260,
    380, 388, 509, 510, 514, 515, 639, 640,
    641, -769, -768, -767, -643, -642, -638, -637,
    -516, -508, -389, -379, -261, -251, -134, -122,
    -6, 6, 122, 134, 251, 261, 379, 389,
    508, 516, 637, 638, 642, 643, 767, 768,
    769, -897, -896, -895, -771, -770, -766, -765,
    -645, -644, -636, -635, -517, -507, -390, -378,
    -262, -250, -135, -121, -7, 7, 121, 135,
    250, 262, 378, 390, 507, 517, 635, 636,
    644, 645, 765, 766, 770, 771, 895, 896,
    897, -1025, -1024, -1023, -899, -898, -894, -893,
    -773, -772, -764, -763, -646, -634, -518, -506,
    -391, -377, -263, -249, -136, -120, -8, 8,
    120, 136, 249, 263, 377, 391, 506, 518,
    634, 646, 763, 764, 772, 773, 893, 894,
    898, 899, 1023, 1024, 1025, -1153, -1152, -1151,
    -1027, -1026, -1022, -1021, -901, -900, -892, -891,
    -774, -762, -647, -633, -519, -505, -392, -376,
    -264, -248, -137, -119, -9, 9, 119, 137,
    248, 264, 376, 392, 505, 519, 633, 647,
    762, 774, 891, 892, 900, 901, 1021, 1022,
    1026, 1027, 1151, 1152, 1153, -1281, -1280, -1279,
    -1155, -1154, -1150, -1149, -1029, -1028, -1020, -1019,
    -903, -902, -890, -889, -775, -761, -648, -632,
    -520, -504, -393, -375, -265, -247, -138, -118,
    -10, 10, 118, 138, 247, 265, 375, 393,
    504, 520, 632, 648, 761, 775, 889, 890,
    902, 903, 1019, 1020, 1028, 1029, 1149, 1150,
    1154, 1155, 1279, 1280, 1281
};

// This relates to sight, sight > 11 == overflow == bad.
// give this a read, it shows how it works with visual help http://modenc.renegadeprojects.com/CellSpread
const int MapClass::RadiusCount[] = {
    1,
    9,
    21,
    37,
    61,
    89,
    121,
    161,
    205,
    253,
    309
};

MapClass::MapClass() :
    MapCellX(0),
    MapCellY(0),
    MapCellWidth(0),
    MapCellHeight(0),
    TotalValue(0),
    Array(),
    XSize(0),
    YSize(0),
    TotalSize(0)
{
}

/**
 * @brief One time class initialisation.
 *
 * 0x004FE310
 */
void MapClass::One_Time()
{
    GameScreenClass::One_Time();
    XSize = MAP_MAX_WIDTH;
    YSize = MAP_MAX_HEIGHT;
    TotalSize = YSize * XSize;
    Alloc_Cells();

    // Placement new init the crates array.
    for (int i = 0; i < MAP_MAX_CRATES; ++i) {
        new(&Crates[i]) CrateClass;
    }
}

/**
 * @brief Clear the game state from the class.
 *
 * 0x004FE344
 */
void MapClass::Init_Clear()
{
    GameScreenClass::Init_Clear();
    Init_Cells();
    OreLogicPos = 0;
    OreGrowthCount = 0;
    OreGrowthExcess = 0;
    OreSpreadCount = 0;
    OreSpreadExcess = 0;
}

/**
 * @brief Allocate the CellClass vector.
 *
 * 0x004FE3B0
 */
void MapClass::Alloc_Cells()
{
    // Reconstruct the vector (incase it has garbage in it from a save load) and then resize to total cell count.
    new(&Array) VectorClass<CellClass>;
    Array.Resize(TotalSize);
}

/**
 * @brief Free the CellClass vector.
 *
 * 0x004FE3DC
 */
void MapClass::Free_Cells()
{
    // Clear all cells in the vector.
    Array.Clear();
}

/**
 * @brief Reinitialise the CellClass array with default constructor.
 *
 * 0x004FE3EC
 */
void MapClass::Init_Cells()
{
    // Reset the total cell count
    TotalValue = 0;

    // Loop through the whole vector and call the the constructor for each cell with placement new.
    for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        new(&Array[cellnum]) CellClass;
    }
}

/**
 * @brief Performs a map logic tick for crate and ore growth handling.
 *
 * 0x004FEE94
 */
void MapClass::Logic_AI()
{
    if (Session.Game_To_Play() != GAME_CAMPAIGN && Session.MPlayer_Goodies_Allowed()) {
        for (int i = 0; i < ARRAY_SIZE(Crates); ++i) {
            if (Crates[i].Get_Cell() != -1) {
                if (Crates[i].Timer_Expired()) {
                    Crates[i].Remove_It();
                    Place_Random_Crate();
                }
            }
        }
    }

    if (Rule.Ore_Grows() || Rule.Ore_Spreads()) {
        // Limit how much of the map we try to mark at any one time to limit how often the growth logic runs.
        int growth_limit = Max(MAP_MAX_AREA / (Rule.Ore_Growth_Rate() * GAME_TICKS_PER_MINUTE), 1);
        int index;

        // Build lists of cells that need growth and spreading applied to them.
        for (index = OreLogicPos; index < MAP_MAX_AREA; ++index) {
            if (In_Radar(index)) {
                if (Array[index].Can_Ore_Grow()) {
                    if (Scen.Get_Random_Value(0, OreGrowthExcess) <= OreGrowthCount) {
                        if (OreGrowthCount >= ARRAY_SIZE(OreGrowth)) {
                            OreGrowth[Scen.Get_Random_Value(0, OreGrowthCount - 1)] = index;
                        } else {
                            OreGrowth[OreGrowthCount++] = index;
                        }
                    }

                    ++OreGrowthExcess;
                }

                if (Array[index].Can_Ore_Spread()) {
                    if (Scen.Get_Random_Value(0, OreSpreadExcess) <= OreSpreadCount) {
                        if (OreSpreadCount >= ARRAY_SIZE(OreSpread)) {
                            OreSpread[Scen.Get_Random_Value(0, OreSpreadCount - 1)] = index;
                        } else {
                            OreSpread[OreSpreadCount++] = index;
                        }
                    }

                    ++OreSpreadExcess;
                }
            }

            if (--growth_limit == 0) {
                break;
            }
        }

        OreLogicPos = index;

        // If we've finished marking the whole map, reset our logic position and action what we marked.
        if (OreLogicPos >= MAP_MAX_AREA) {
            OreLogicPos = 0;

            for (int index = 0; index < OreGrowthCount; ++index) {
                Array[OreGrowth[index]].Grow_Ore();
            }

            OreGrowthCount = 0;
            OreGrowthExcess = 0;

            for (int index = 0; index < OreSpreadCount; ++index) {
                Array[OreSpread[index]].Spread_Ore(false);
            }

            OreSpreadCount = 0;
            OreSpreadExcess = 0;
        }
    }
}

/**
 * @brief Sets the maps playable dimensions.
 *
 * 0x004FE420
 */
void MapClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
    MapCellX = x;
    MapCellY = y;
    MapCellWidth = w;
    MapCellHeight = h;
}

/**
 * @brief Generates a crate at a random location.
 *
 * 0x004FF218
 */
BOOL MapClass::Place_Random_Crate()
{
    for (int i = 0; i < ARRAY_SIZE(Crates); ++i) {
        // Check if current crate cell is invalid and thus instance can be used for new crate.
        if (Crates[i].Get_Cell() == -1) {
            // Try to place crate up to 1000 times, return result of final attempt if all others fail.
            for (int i = 0; i < 1000; ++i) {
                if (Crates[i].Create_Crate(Pick_Random_Location())) {
                    return true;
                }
            }

            return Crates[i].Create_Crate(Pick_Random_Location());
        }
    }

    return false;
}

BOOL MapClass::Place_Random_Crate_At_Cell(cell_t cellnum)
{
    for (int i = 0; i < ARRAY_SIZE(Crates); ++i) {
        // Check if current crate cell is invalid and thus instance can be used for new crate.
        if (Crates[i].Get_Cell() == -1) {
            // Try to place crate up to 1000 times, return result of final attempt if all others fail.
            for (int i = 0; i < 1000; ++i) {
                if (Crates[i].Create_Crate(cellnum)) {
                    return true;
                }
            }

            return Crates[i].Create_Crate(cellnum);
        }
    }

    return false;
}

/**
 * @brief Remove a crate from a given location.
 *
 * 0x004FF2A0
 */
BOOL MapClass::Remove_Crate(cell_t cellnum)
{
    if (Session.Game_To_Play() != GAME_CAMPAIGN) {
        for (int index = 0; index < ARRAY_SIZE(Crates); ++index) {
            if (Crates[index].Get_Cell() != -1 && Crates[index].Get_Cell() == cellnum) {
                return Crates[index].Remove_It();
            }
        }
    }

    CellClass &cell = Array[cellnum];

    if (cell.Get_Overlay() != OVERLAY_NONE) {
        if (OverlayTypeClass::As_Reference(cell.Get_Overlay()).Is_Crate()) {
            cell.Set_Overlay(OVERLAY_NONE);
            cell.Set_Overlay_Frame(-1);

            return true;
        }
    }

    return false;

}

/**
 * @brief Get a random location within the playable map.
 *
 * 0x005008A4
 */
cell_t MapClass::Pick_Random_Location() const
{
    return Cell_From_XY(MapCellX + Scen.Get_Random_Value(0, MapCellWidth - 1), MapCellY + Scen.Get_Random_Value(0, MapCellHeight - 1));
}

/**
 * @brief Check if a location is within the playable map.
 *
 * 0x004FE8AC
 */
BOOL MapClass::In_Radar(cell_t cellnum) const
{
    if (cellnum <= MAP_MAX_AREA) {

        unsigned cell_x = Cell_Get_X(cellnum);
        unsigned cell_y = Cell_Get_Y(cellnum);

        // Treat as unsigned to check value is within both bounds for only one check by using unsigned underflow behaviour.
        if ((cell_x - (unsigned)MapCellX) < (unsigned)MapCellWidth && (cell_y - (unsigned)MapCellY) < (unsigned)MapCellHeight) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Calculates sight from a given cell.
 *
 * 0x004FE438
 */
void MapClass::Sight_From(cell_t cellnum, int radius, HouseClass *house, BOOL a4)
{
    // TODO requires RadarClass
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const MapClass*, cell_t, int, HouseClass *, BOOL) = reinterpret_cast<void(*)(const MapClass*, cell_t, int, HouseClass *, BOOL)>(0x004FE438);
    func(this, cellnum, radius, house, a4);
#elif 0
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(radius < ARRAY_SIZE(RadiusCount));
    DEBUG_ASSERT(house != nullptr);

    // NOTE: In C&C/Sole, this doesn't take a house and instead always passes PlayerPtr to Map_Cell().
    // Sole adjusts radius to be 10 if its greater than 10. Possible fix to avoid function treating radius > 10 as 0?
    // "!!!going past ten is a hard code Vegas crash!!!"

    if (In_Radar(cellnum)) {
        if (radius >= 0 && radius < ARRAY_SIZE(RadiusCount)) {
            const int *offset_ptr = RadiusOffset;
            int radius_count = RadiusCount[radius];

            // In Sole/C&C the check is for radius > 1 and the adjustment is - 1. Not sure what significance that has, but a4 appears the be a bool that flags if this check and adjustment is made.
            if (a4 && radius > 2) {
                radius_count -= RadiusCount[radius];
                offset_ptr = &RadiusOffset[RadiusCount[radius]];
            }

            CellClass *cell = &Array[cellnum];
            DEBUG_ASSERT(cell != nullptr);

            while (--radius_count != -1) {
                cell_t offset_cellnum = *offset_ptr + cellnum;
                ++offset_ptr;

                // If the cell we are considering is within the map and the distance in the X axis is less than radius, calc actual distance.
                if (offset_cellnum < MAP_MAX_AREA) {
                    if (Abs(Cell_Get_X(offset_cellnum) - Cell_Get_X(cellnum)) <= radius) {
                        // In SS/C&C, distance uses raw cell numbers into the int16_t int version of distance and checks <= radius, not radius * 256.
                        if (Distance(Cell_To_Coord(offset_cellnum), Cell_To_Coord(cellnum)) <= (radius * 256)) {
                            if (!cell->Is_Visible()) {
                                // TODO Calls to RadarClass function higherup class stack, bad juju, should be virtual.
                                Map.Map_Cell(offset_cellnum, house);
                            }
                        }
                    }
                }
            }
        }
    }
#endif
}

/**
 * @brief Shroud a given radius around a cell.
 *
 * 0x004FE588
 */
void MapClass::Shroud_From(cell_t cellnum, int radius)
{
    // TODO requires RadarClass
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const MapClass*, cell_t, int) = reinterpret_cast<void(*)(const MapClass*, cell_t, int)>(0x004FE588);
    func(this, cellnum, radius);
#elif 0
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(radius < ARRAY_SIZE(RadiusCount));

    // Original code does not check radius is with the array bounds
    if (In_Radar(cellnum)) {
        if (radius > 0 && radius <= Rule.Get_Gap_Radius() && radius < ARRAY_SIZE(RadiusCount)) {
            const int32_t *offset_ptr = RadiusOffset;
            int32_t radius_count = RadiusCount[radius];

            while (radius_count-- >= 0) {
                cell_t offset_cellnum = offset_ptr[cellnum];
                ++offset_ptr;

                // TODO, same loop as above pretty much...

                // checks the result of a distance check.
                if (offset_cellnum < MAP_MAX_AREA && Abs(Cell_Get_X(offset_cellnum) - Cell_Get_X(cellnum)) <= radius) {
                    // In SS/C&C, distance uses raw cell numbers into the int16_t int version of distance and checks <=
                    // radius, not radius * 256.
                    if (Distance(Cell_To_Coord(cellnum), Cell_To_Coord(offset_cellnum)) <= (radius * 256)) {
                        Map.Shroud_Cell(offset_cellnum); // TODO call from display class, should be virtual?
                    }
                }
            }
        }
    }
#endif
}

/**
 * @brief Radar jam a given radius around a cell.
 *
 * 0x004FE68C
 */
void MapClass::Jam_From(cell_t cellnum, int radius, HouseClass *house)
{
    // TODO requires HouseClass, RadarClass
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const MapClass*, cell_t, int, HouseClass *) = reinterpret_cast<void(*)(const MapClass*, cell_t, int, HouseClass *)>(0x004FE68C);
    func(this, cellnum, radius, house);
#elif 0
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(radius < ARRAY_SIZE(RadiusCount));
    DEBUG_ASSERT(house != nullptr);

    //this function is empty in EDWIN, ensure function behaves correctly for map editing mode.
    if (!g_inMapEditor) {
        // Original does not check radius is within array bounds
        if (radius >= 0 && radius <= Rule.Get_Gap_Radius() && radius < ARRAY_SIZE(RadiusCount)) {
            const int32_t *offset_ptr = RadiusOffset;
            int32_t radius_count = RadiusCount[radius];

            while (radius_count-- >= 0) {
                cell_t offset_cellnum = *offset_ptr + cellnum;
                ++offset_ptr;

                if (offset_cellnum < MAP_MAX_AREA &&
                    Abs(Cell_Get_X(offset_cellnum) - Cell_Get_X(cellnum)) <= radius) {

                    if (Distance(Cell_To_Coord(cellnum), Cell_To_Coord(offset_cellnum)) <= (radius * 256)) {
                        Map.Jam_Cell(offset_cellnum, house);
                    }
                }
            }

            if (house->PlayerControl) {
                Map.Constrained_Look(Cell_To_Coord(cellnum), Rule.Get_Gap_Radius() * 256);
            }
        }
    }
#endif
}

/**
 * @brief Radar jam a given radius around a cell.
 *
 * 0x004FE7C8
 */
void MapClass::UnJam_From(cell_t cellnum, int radius, HouseClass *house)
{
    // TODO requires RadarClass
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const MapClass*, cell_t, int, HouseClass *) = reinterpret_cast<void(*)(const MapClass*, cell_t, int, HouseClass *)>(0x004FE7C8);
    func(this, cellnum, radius, house);
#elif 0
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(radius < ARRAY_SIZE(RadiusCount));
    DEBUG_ASSERT(house != nullptr);

    //this function is empty in EDWIN, ensure function behaves correctly for map editing mode.
    if (!g_inMapEditor) {
        // Original does not check radius is within array bounds
        if (radius >= 0 && radius <= Rule.Get_Gap_Radius() && radius < ARRAY_SIZE(RadiusCount)) {
            const int32_t *offset_ptr = RadiusOffset;
            int32_t radius_count = RadiusCount[radius];

            while (radius_count-- >= 0) {
                cell_t offset_cellnum = *offset_ptr + cellnum;
                ++offset_ptr;

                if (offset_cellnum < MAP_MAX_AREA &&
                    Abs(Cell_Get_X(offset_cellnum) - Cell_Get_X(cellnum)) <= radius) {

                    if (Distance(Cell_To_Coord(cellnum), Cell_To_Coord(offset_cellnum)) <= (radius * 256)) {
                        Map.UnJam_Cell(offset_cellnum, house);
                    }
                }
            }
        }
    }
#endif
}

/**
 * @brief Places down a unit from the cell after performing movement logic...  I think.
 *
 * 0x004FE918
 */
void MapClass::Place_Down(cell_t cellnum, ObjectClass *object)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(object != nullptr);

    int16_t tmp_list[60];

    if (object != nullptr) {
        if (object->Class_Of().Get_Bit128() && object->In_Which_Layer() == LAYER_GROUND) {
            List_Copy(tmp_list, object->Occupy_List(), ARRAY_SIZE(tmp_list));
            int16_t *list_ptr = tmp_list;

            while (*list_ptr != LIST_END) {
                cell_t occupy_cell = cellnum + *list_ptr++;

                if (occupy_cell < MAP_MAX_AREA) {
                    CellClass &cell = Array[occupy_cell];
                    cell.Occupy_Down(object);
                    cell.Recalc_Attributes();
                    cell.Redraw_Objects(false);
                }
            }

            List_Copy(tmp_list, object->Overlap_List(false), ARRAY_SIZE(tmp_list));
            list_ptr = tmp_list;

            while (*list_ptr != LIST_END) {
                cell_t occupy_cell = cellnum + *list_ptr++;

                if (occupy_cell < MAP_MAX_AREA) {
                    CellClass &cell = Array[occupy_cell];
                    cell.Overlap_Down(object);
                    cell.Redraw_Objects(false);
                }
            }
        }
    }
}

/**
 * @brief Picks up a unit from the cell to perform movement logic...  I think.
 *
 * 0x004FEA28
 */
void MapClass::Pick_Up(cell_t cellnum, ObjectClass *object)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(object != nullptr);

    int16_t tmp_list[60];

    if (object != nullptr) {
        if (object->Class_Of().Get_Bit128() && object->In_Which_Layer() == LAYER_GROUND) {
            List_Copy(tmp_list, object->Occupy_List(), ARRAY_SIZE(tmp_list));
            int16_t *list_ptr = tmp_list;

            while (*list_ptr != LIST_END) {
                cell_t occupy_cell = cellnum + *list_ptr++;

                if (occupy_cell < MAP_MAX_AREA) {
                    CellClass &cell = Array[occupy_cell];
                    cell.Occupy_Up(object);
                    cell.Recalc_Attributes();
                    cell.Redraw_Objects(false);
                }
            }

            List_Copy(tmp_list, object->Overlap_List(false), ARRAY_SIZE(tmp_list));
            list_ptr = tmp_list;

            while (*list_ptr != LIST_END) {
                cell_t occupy_cell = cellnum + *list_ptr++;

                if (occupy_cell < MAP_MAX_AREA) {
                    CellClass &cell = Array[occupy_cell];
                    cell.Overlap_Up(object);
                    cell.Redraw_Objects(false);
                }
            }
        }
    }
}

/**
 * @brief Places down a unit from the cell after performing movement logic...  I think.
 *
 * 0x004FEB38
 */
void MapClass::Overlap_Down(cell_t cellnum, ObjectClass *object)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(object != nullptr);

    int16_t tmp_list[60];

    if (object != nullptr) {
        if (object->Class_Of().Get_Bit128() && object->In_Which_Layer() == LAYER_GROUND) {
            List_Copy(tmp_list, object->Overlap_List(), ARRAY_SIZE(tmp_list));
            int16_t *list_ptr = tmp_list;

            while (*list_ptr != LIST_END) {
                cell_t occupy_cell = cellnum + *list_ptr++;

                if (occupy_cell < MAP_MAX_AREA) {
                    CellClass &cell = Array[occupy_cell];
                    cell.Overlap_Down(object);
                    cell.Redraw_Objects(false);
                }
            }
        }
    }
}

/**
 * @brief Picks up a unit from the cell to perform movement logic...  I think.
 *
 * 0x004FEBD8
 */
void MapClass::Overlap_Up(cell_t cellnum, ObjectClass *object)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DEBUG_ASSERT(object != nullptr);

    int16_t tmp_list[60];

    if (object != nullptr) {
        if (object->Class_Of().Get_Bit128() && object->In_Which_Layer() == LAYER_GROUND) {
            List_Copy(tmp_list, object->Overlap_List(), ARRAY_SIZE(tmp_list));
            int16_t *list_ptr = tmp_list;

            while (*list_ptr != LIST_END) {
                cell_t occupy_cell = cellnum + *list_ptr++;

                if (occupy_cell < MAP_MAX_AREA) {
                    CellClass &cell = Array[occupy_cell];
                    cell.Overlap_Up(object);
                    cell.Redraw_Objects(false);
                }
            }
        }
    }
}

/**
 * @brief Performs randomized adjustments to ore across the playable map area.
 *
 * 0x004FEC78
 */
int MapClass::Overpass()
{
    int retval = 0;

    if (MapCellHeight > 0) {
        for (int y = 0; y < MapCellHeight; ++y) {
            for (int x = 0; x < MapCellWidth; ++x) {
                cell_t cellnum = Cell_From_XY(MapCellX + x, MapCellY + y);
                CellClass &cell = Array[cellnum];
                retval += cell.Ore_Adjust(true);
                cell.Recalc_Attributes();
            }
        }
    }

    return retval;
}

/**
 * @brief Some calculation relating to where within the area of the map a given is positioned.
 *
 * 0x004FF168
 */
int MapClass::Cell_Region(cell_t cellnum)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    return 34 * (Cell_Get_X(cellnum) / 4 + 1) + (Cell_Get_Y(cellnum) / 4 + 1);
}

/**
 * @brief Calculates how dangerous a given cells is?
 *
 * 0x004FF1BC
 */
int MapClass::Cell_Threat(cell_t cellnum, HousesType house)
{
    // TODO requires HouseClass
#ifndef CHRONOSHIFT_STANDALONE
    int(*func)(const MapClass*, cell_t, HousesType) = reinterpret_cast<int(*)(const MapClass*, cell_t, HousesType)>(0x004FF1BC);
    return func(this, cellnum, house);
#elif 0
    DEBUG_ASSERT(this != nullptr);

    RegionClass *house_regions = HouseClass::As_Pointer(house)->Regions;
    int region = Cell_Region(Array[cellnum].Cell_Number());

    if (house_regions != nullptr) {
        int retval = house_regions[region].field_0;

        if (retval == 0) {
            if (Array[cellnum].Get_Bit128()) {
                return 1;
            }
        }

        return retval;
    }

    return 0;
#else
    return 0;
#endif
}

/**
 * @brief Recalculates contiguous reachable zones for each movement type.
 *
 * 0x004FF690
 */
int MapClass::Zone_Reset(int zones)
{
    // Loop through and reset every cells movement zones to 0.
    for (int index = 0; index < Array.Length(); ++index) {
        CellClass &cell = Array[index];

        if (zones & (1 << MZONE_NORMAL)) {
            cell.Set_Zone(MZONE_NORMAL, 0);
        }

        if (zones & (1 << MZONE_CRUSHER)) {
            cell.Set_Zone(MZONE_CRUSHER, 0);
        }

        if (zones & (1 << MZONE_DESTROYER)) {
            cell.Set_Zone(MZONE_DESTROYER, 0);
        }

        if (zones & (1 << MZONE_AMPHIBIOUS_DESTROYER)) {
            cell.Set_Zone(MZONE_AMPHIBIOUS_DESTROYER, 0);
        }
    }

    int tmp;

    // Calculate zones on the map that are all suitable for a given movement zone. Zones are contiguous areas on the map that
    // can be reached using a given movement zone.
    if (zones & (1 << MZONE_NORMAL)) {
        tmp = 1;

        for (int cell = 0; cell < MAP_MAX_AREA; ++cell) {
            if (Zone_Span(cell, tmp, MZONE_NORMAL) != 0) {
                ++tmp;
            }
        }
    }

    if (zones & (1 << MZONE_CRUSHER)) {
        tmp = 1;

        for (int cell = 0; cell < MAP_MAX_AREA; ++cell) {

            if (Zone_Span(cell, tmp, MZONE_CRUSHER) != 0) {
                ++tmp;
            }
        }
    }

    if (zones & (1 << MZONE_DESTROYER)) {
        tmp = 1;

        for (int cell = 0; cell < MAP_MAX_AREA; ++cell) {
            if (Zone_Span(cell, tmp, MZONE_DESTROYER) != 0) {
                ++tmp;
            }
        }
    }

    if (zones & (1 << MZONE_AMPHIBIOUS_DESTROYER)) {
        tmp = 1;

        for (int cell = 0; cell < MAP_MAX_AREA; ++cell) {
            if (Zone_Span(cell, tmp, MZONE_AMPHIBIOUS_DESTROYER) != 0) {
                ++tmp;
            }
        }
    }

    return 0;
}

/**
 * @brief Calculates a contiguous reachable zones span within the map.
 *
 * 0x004FF7D8
 */
int MapClass::Zone_Span(cell_t cell, int zone, MZoneType mzone)
{
    int16_t cell_x = Cell_Get_X(cell);
    int16_t cell_y = Cell_Get_Y(cell);

    // Amphibious Destroyer is Vessels and other floaty things?
    SpeedType speed = (mzone == MZONE_AMPHIBIOUS_DESTROYER ? SPEED_FLOAT : SPEED_TRACK);

    // Check the cell coords are within the visible part of the map
    if (cell_y >= MapCellY && cell_y < (MapCellHeight + MapCellY) && cell_x >= MapCellX
        && cell_x < (MapCellWidth + MapCellX)) {
        int retval = 0;
        int left_pos;

        // Scan backwards from cell coord to edge of map until we find a cell that is already marked for this zone or can't be
        // moved to and record the pos.
        for (left_pos = cell_x; left_pos >= MapCellX; --left_pos) {
            CellClass &cell = Array[Cell_From_XY(left_pos, cell_y)];

            // Check if clear to move, ignoring units, buildings and infantry.
            if (cell.Get_Zone(mzone) != 0 || !cell.Is_Clear_To_Move(speed, true, true, -1, mzone)) {
                if (left_pos == cell_x) {
                    return 0;
                }

                ++left_pos;
                break;
            }

        }

        left_pos = Max(left_pos, MapCellX);

        int right_pos;

        // Scan forward to other edge of the map doing the same.
        for (right_pos = cell_x; right_pos < MapCellWidth + MapCellX; ++right_pos) {
            CellClass &cell = Array[Cell_From_XY(right_pos, cell_y)];

            // Check if clear to move, ignoring units, buildings and infantry.
            if (cell.Get_Zone(mzone) != 0 ||
                !cell.Is_Clear_To_Move(speed, true, true, -1, mzone)) {
                --right_pos;
                break;
            }
        }

        right_pos = Min(right_pos, MapCellWidth + MapCellX - 1);

        // Set all the cells between the two positions to our zone value for this movement zone.
        for (int i = left_pos; i <= right_pos; ++i) {
            Array[Cell_From_XY(i, cell_y)].Set_Zone(mzone, zone);
            ++retval;
        }

        // If we have at least one cell, repeat the process above and below, sweet sweet recursion...
        if ((left_pos - 1) <= right_pos) {
            for (int i = left_pos - 1; i <= right_pos; ++i) {
                retval += Zone_Span(Cell_From_XY(i, cell_y - 1), zone, mzone);
                retval += Zone_Span(Cell_From_XY(i, cell_y + 1), zone, mzone);
            }
        }

        // Return the number of cells that are in this zone.
        return retval;
    }

    return 0;
}

/**
 * @brief Something relating to near by reachable cells?
 *
 * 0x004FFAC4
 */
cell_t MapClass::Nearby_Location(cell_t cellnum, SpeedType speed, int zone, MZoneType mzone) const
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    cell_t near_cells[10];

    int near_cell_index = 0;
    int cell_x = Cell_Get_X(cellnum);
    int cell_y = Cell_Get_Y(cellnum);

    // Calculate the bounds of the area we want to check
    int left = cell_x - MapCellX;
    int right = MapCellWidth - left - 1;
    int top = cell_y - MapCellY;
    int bottom = MapCellHeight - top - 1;

    for (int i = 0; i < 64; ++i) {
        for (int j = -i; j <= i; ++j) {
            if (j >= -left && i <= top) {
                cell_t near_cellnum = Cell_From_XY(j + cell_x, cell_y - i);

                if (Map.In_Radar(near_cellnum)) {
                    if (Array[near_cellnum].Is_Clear_To_Move(speed, false, false, zone, mzone)) {
                        near_cells[near_cell_index++] = near_cellnum;
                    }
                }
            }

            if (near_cell_index == ARRAY_SIZE(near_cells)) {
                break;
            }

            if (j <= right && i <= bottom) {
                cell_t near_cellnum = Cell_From_XY(cell_x + j, i + cell_y);

                if (Map.In_Radar(near_cellnum)) {
                    if (Array[near_cellnum].Is_Clear_To_Move(speed, false, false, zone, mzone)) {
                        near_cells[near_cell_index++] = near_cellnum;
                    }
                }
            }

            if (near_cell_index == ARRAY_SIZE(near_cells)) {
                break;
            }
        }

        if (near_cell_index == ARRAY_SIZE(near_cells)) {
            break;
        }

        for (int k = -(i - 1); k <= i - 1; ++k) {
            if (k >= -top && i <= left) {
                cell_t near_cellnum = Cell_From_XY(cell_x - i, k + cell_y);

                if (Map.In_Radar(near_cellnum)) {
                    if (Array[near_cellnum].Is_Clear_To_Move(speed, false, false, zone, mzone)) {
                        near_cells[near_cell_index++] = near_cellnum;
                    }
                }
            }

            if (near_cell_index == ARRAY_SIZE(near_cells)) {
                break;
            }

            if (k <= bottom && i <= right) {
                cell_t near_cellnum = Cell_From_XY(cell_x + i, cell_y + k);

                if (Map.In_Radar(near_cellnum)) {
                    if (Array[near_cellnum].Is_Clear_To_Move(speed, false, false, zone, mzone)) {
                        near_cells[near_cell_index++] = near_cellnum;
                    }
                }
            }

            if (near_cell_index == ARRAY_SIZE(near_cells)) {
                break;
            }
        }

        if (near_cell_index > 0) {
            break;
        }
    }

    if (near_cell_index > 0) {
        return near_cells[g_frame % near_cell_index];
    }

    return 0;
}

/**
 * @brief Works out if a cell is within a houses zone and returns the zone type?
 *
 * 0x004FFEAC
 */
BOOL MapClass::Base_Region(cell_t cellnum, HousesType &house, ZoneType &zone) const
{
    // TODO requires HouseClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(const MapClass*, cell_t, HousesType&, ZoneType&) = reinterpret_cast<BOOL(*)(const MapClass*, cell_t, HousesType&, ZoneType&)>(0x004FFEAC);
    return func(this, cellnum, house, zone);
#elif 0
    if (cellnum < MAP_MAX_AREA && In_Radar(cellnum)) {
        for (house = HOUSES_FIRST; house < HOUSES_COUNT; ++house) {
            HouseClass *hptr = HouseClass::As_Pointer(house);
            DEBUG_ASSERT(hptr != nullptr);

            if (hptr != nullptr) {
                if (hptr->Is_Active() && !hptr->Defeated && Valid_Coord(hptr->field_2AE)) {
                    zone = hptr->Which_Zone(cellnum);

                    if (zone != ZONE_NONE) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
#else
    return false;
#endif
}

/**
 * @brief Destroys a bridge at a given location.
 *
 * 0x004FFF1C
 */
int MapClass::Destroy_Bridge_At(cell_t cellnum)
{
    // TODO Needs TemplateClass, AnimClass
#ifndef CHRONOSHIFT_STANDALONE
    int(*func)(const MapClass*, cell_t) = reinterpret_cast<int(*)(const MapClass*, cell_t)>(0x004FFF1C);
    return func(this, cellnum);
#else
    return 0;
#endif
}

/**
 * @brief Removes a trigger?
 *
 * 0x0050078C
 */
void MapClass::Detach(int32_t target, int a2)
{
    // TODO Requires TriggerClass
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const MapClass*, int32_t, int) = reinterpret_cast<void(*)(const MapClass*, int32_t, int)>(0x0050078C);
    func(this, target, a2);
#elif 0
    if (Target_Get_RTTI(target) == RTTI_TRIGGER) {
        for (int i = 0; i < MapTriggers.Count(); ++i) {
            TriggerClass *trig = MapTriggers[i];
            DEBUG_ASSERT(trig != nullptr);

            if (As_Trigger(target) == trig) {
                MapTriggers.Delete(trig);
                break;
            }
        }

        for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
            CellClass &cell = Array[cellnum];

            if (cell.CellTag == As_Trigger(target)) {
                cell.CellTag = nullptr;
            }
        }

    }
#endif
}

/**
 * @brief Gets a count of how many intact bridge cells there are on the map.
 *
 * 0x0050083C
 */
int MapClass::Intact_Bridge_Count() const
{
    int count = 0;

    // We loop through the cell array and only consider the 4 templates that have both bridge ends in the template (so not the long bridges)
    for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        const CellClass &cell = Array[cellnum];

        switch (cell.Get_Template()) {
            case TEMPLATE_BRIDGE1:    //Fall through on intact bridge templates
            case TEMPLATE_BRIDGE2:
            case TEMPLATE_BRIDGE1H:
            case TEMPLATE_BRIDGE2H:
                // If the Icon is number 6, then we have the intact bridge with an intact bridge cell, increment count.
                if (cell.Get_Icon() == 6) {
                    ++count;
                }

                break;

            default:
                break;
        }
    }

    return count;
}

/**
 * @brief Unsuprisingly, this shrouds the map.
 *
 * 0x00500908
 */
void MapClass::Shroud_The_Map()
{
    // TODO Requires DisplayClass, TechnoClass
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const MapClass*) = reinterpret_cast<void(*)(const MapClass*)>(0x00500908);
    func(this);
#elif 0
    for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        CellClass &cell = Array[cellnum];

        if (cell.Is_Visible() || cell.Is_Revealed()) {
            cell.Redraw_Objects();

            int xpos = Cell_Get_X(cellnum);
            int ypos = Cell_Get_Y(cellnum);

            if (xpos >= MapCellX && xpos < MapCellWidth + MapCellX &&
                ypos >= MapCellY && ypos < MapCellHeight + MapCellY) {

                cell.Set_Visible(false);
                cell.Set_Revealed(false);
            }
        }
    }

    for (int i = 0; i < DisplayClass::Layers[LAYER_GROUND].Count(); ++i) {    
        if (DisplayClass::Layers[LAYER_GROUND][i]->Is_Techno()) {
            TechnoClass *tptr = reinterpret_cast<TechnoClass *>(DisplayClass::Layers[LAYER_GROUND][i]);

            if (tptr->OwnerHouse == house) {
                tptr->Look();
            }
        }
    }

    Flag_To_Redraw(true);
#endif
}

/**
 * @brief Write the map data out to the provided pipe in NewINIFormat=3 format.
 *
 * 0x004FECFC
 */
int MapClass::Write_Binary(Pipe &pipe)
{
    LCWPipe lcw(PIPE_COMPRESS);
    int total = 0;

    lcw.Put_To(&pipe);

    // Writes for INIFormat = 3 only, Tile first, then the sub cell (Icon).
    for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        uint16_t temp = Array[cellnum].Get_Template();
        temp = htole16(temp);
        total += lcw.Put(&temp, sizeof(temp));
    }

    for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        uint8_t temp = Array[cellnum].Get_Icon();
        total += lcw.Put(&temp, sizeof(temp));
    }

    // Return the total bytes written to the pipe.
    return total;
}

/**
 * @brief Reads the map data out of the provided straw, handles all known C&C, RA and SS formats.
 *
 * 0x004FED90
 */
BOOL MapClass::Read_Binary(Straw &straw)
{
    LCWStraw lcw(STRAW_UNCOMPRESS);
    lcw.Get_From(&straw);

    switch (g_iniFormat) {
        case INIFORMAT_0:    //Covers both TD and SS maps, .BIN files
            // TD and SS both "blank" the map cell array first.
            for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
                CellClass &cell = Array[cellnum];

                cell.Set_Template(TEMPLATE_NONE);
                cell.Set_Icon(0);
                cell.Recalc_Attributes();
            }

            // Handle Sole Survivor maps. Only cells other than TEMPLATE_NONE are saved in the binary.
            if (g_mapBinaryVersion != 0) {
                // Format is 4 bytes per cell, X pos, Y pos, template type, icon
                uint8_t coord[2];
                uint8_t icon[2];

                while (straw.Get(&coord, sizeof(coord)) > 0 && straw.Get(icon, sizeof(icon)) > 0) {
                    DEBUG_ASSERT((coord[0] * coord[1]) < MAP_MAX_AREA);
                    CellClass &cell = Array[coord[0] * coord[1]];

                    if (icon[0] == 0xFF) {
                        cell.Set_Template(TEMPLATE_NONE);
                        cell.Set_Icon(0);
                    } else {
                        cell.Set_Template((TemplateType)icon[0]);
                        cell.Set_Icon(icon[1]);
                    }

                    // TD and SS also call a CRC function here, used to calc CRC in absence of SHA1 checksums for the map?
                    cell.Recalc_Attributes();
                }
            } else {
                // Handle TD maps being loaded into larger array than the original. Like blitting a region in a buffer.
                for (int h = 0; h < MAPTD_MAX_HEIGHT; ++h) {
                    for (int w = 0; w < MAPTD_MAX_WIDTH; ++w) {
                        CellClass &cell = Array[Cell_From_XY(w, h)];
                        uint8_t icon[2];
                        straw.Get(icon, sizeof(icon));

                        if (icon[0] == 0xFF) {
                            cell.Set_Template(TEMPLATE_NONE);
                            cell.Set_Icon(0);
                        } else {
                            cell.Set_Template((TemplateType)icon[0]);
                            cell.Set_Icon(icon[1]);
                        }

                        // TD and SS also call a CRC function here, used to calc CRC in absence of SHA1 checksums for the map?
                        cell.Recalc_Attributes();
                    }
                }
            }

            break;

        case INIFORMAT_1:    // Handles old MapPack data layout, similar to TD layout
        case INIFORMAT_2:    // Fall through
            for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
                CellClass &cell = Array[cellnum];

                uint16_t temp;
                uint8_t icon;
                lcw.Get(&temp, 2);
                lcw.Get(&icon, 1);
                temp = le16toh(temp);

                if (temp == 0xFFFF) {
                    cell.Set_Template(TEMPLATE_NONE);
                    cell.Set_Icon(0);
                } else {
                    cell.Set_Template((TemplateType)temp);
                    cell.Set_Icon(icon);
                }

                cell.Recalc_Attributes();
            }

            break;

        case INIFORMAT_3: // Handles newest MapPack layout, RA default.
            for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
                CellClass &cell = Array[cellnum];

                uint16_t temp;
                lcw.Get(&temp, 2);
                temp = le16toh(temp);
                cell.Set_Template(temp == 0xFFFF ? TEMPLATE_NONE : (TemplateType)temp);
            }

            for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
                CellClass &cell = Array[cellnum];

                uint8_t icon;
                lcw.Get(&icon, 1);
                cell.Set_Icon(icon);
                cell.Recalc_Attributes();
            }

            break;

        default: // Unknown format, future expansion?
            DEBUG_ASSERT_PRINT(false, "Invalid Map format\n");
            break;
    }

    return true;
}

/**
 * @brief Performs some validation on the map data.
 *
 * 0x004FF368
 */
BOOL MapClass::Validate()
{
    // Some static pointer in CellClass in the original, not clear what it is for... 
    // CellClass::BlubCell = &Array[797];

    for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        CellClass &cell = Array[cellnum];

        if (cell.Get_Template() < TEMPLATE_NONE && cell.Get_Template() >= TEMPLATE_COUNT) {
            return false;
        }

        if (cell.Get_Template() != TEMPLATE_NONE) {
            TemplateTypeClass &temptype = TemplateTypeClass::As_Reference(cell.Get_Template());
            uint8_t icon = cell.Get_Icon();
            int icon_count = temptype.Get_Width() * temptype.Get_Height();
            uint8_t *tile_map = Get_Icon_Set_Map(temptype.Get_Image_Data());
            if (icon > icon_count || tile_map[icon] == 0xFF) {
                return false;
            }
        }

        if (cell.Get_Overlay() < OVERLAY_NONE || cell.Get_Overlay() >= OVERLAY_COUNT) {
            return false;
        }

        if (cell.Get_Smudge() < SMUDGE_NONE || cell.Get_Smudge() >= SMUDGE_COUNT) {
            return false;
        }

        if (cell.Get_Land() < LAND_NONE || cell.Get_Land() >= LAND_COUNT) {
            return false;
        }

        ObjectClass *objptr = cell.Get_Occupier();

        if (objptr != nullptr) {
            // These two seem to be making assumptions about values a pointer can hold to check if they are currently Coded.
            // Not sure how valid these assumptions will be cross platform.
            if (uintptr_t(objptr) & 0xFF000000) {
                return false;
            }

            if (uintptr_t(objptr->Get_Next()) & 0xFF000000) {
                return false;
            }

            if (objptr->In_Limbo()) {
                return false;
            }

            if (Coord_To_Cell(objptr->Get_Coord()) >= MAP_MAX_AREA) {
                return false;
            }
        }

        for (int i = 0; i < OVERLAPPER_COUNT; ++i) {
            ObjectClass *objptr = cell.Get_Overlapper(i);

            if (objptr != nullptr) {
                if (uintptr_t(objptr) & 0xFF000000) {
                    return false;
                }

                if (uintptr_t(objptr->Get_Next()) & 0xFF000000) {
                    return false;
                }

                if (objptr->In_Limbo()) {
                    return false;
                }

                if (Coord_To_Cell(objptr->Get_Coord()) >= MAP_MAX_AREA) {
                    return false;
                }
            }
        }
    }

    return true;
}

/**
 * @brief Gets the closest object to a given coord.
 *
 * 0x004FF554
 */
ObjectClass *MapClass::Close_Object(coord_t coord) const
{
    // TODO Requires TechnoClass
#ifndef CHRONOSHIFT_STANDALONE
    ObjectClass *(*func)(const MapClass*, coord_t) = reinterpret_cast<ObjectClass *(*)(const MapClass*, coord_t)>(0x004FF554);
    return func(this, coord);
#elif 0
    static const int _offsets[9] = { 0, -1, 1, -128, 128, 127, 129, -127, -129 };

    ObjectClass *retval = nullptr;
    int nearest = 0;

    for (int i = 0; i < ARRAY_SIZE(_offsets); ++i) {
        cell_t offset_cell = _offsets[i] + Coord_To_Cell(coord);

        if (In_Radar(offset_cell)) {
            const CellClass &cell = Array[offset_cell];

            for (ObjectClass *obj = cell.Get_Occupier(); obj != nullptr; obj = obj->Next) {
                if (!obj->Is_Techno() || reinterpret_cast<TechnoClass *>(obj)->PlayerOwned || reinterpret_cast<TechnoClass *>(obj)->Is_Cloaked()) {
                    int distance;

                    if (obj->What_Am_I() == RTTI_BUILDING) {
                        distance = Distance(coord, Cell_To_Coord(offset_cell));

                        if (distance > 181) {
                            distance = -1;
                        }
                    } else {
                        distance = Distance(coord, obj->Center_Coord());
                    }

                    if (distance >= 0 && (retval == nullptr || distance < nearest)) {
                        nearest = distance;
                        retval = obj;
                    }
                }
            }
        }
    }

    return retval;
#else
    return nullptr;
#endif
}
