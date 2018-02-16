/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information pertaining to a single map cell.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cell.h"
#include "coord.h"
#include "iomap.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "theater.h"

CellClass::CellClass() :
    CellNumber(Map.Cell_Number(this)),
    Bit1(false),
    Bit2(false),
    Bit4(false),
    Bit8(false),
    Bit16(false),
    Bit32(false),
    HasFlag(false),
    Bit128(false),
    field_A(HOUSES_COUNT),
    CellTag(),
    Template(TEMPLATE_NONE),
    Icon(0),
    Overlay(OVERLAY_NONE),
    OverlayFrame(-1),
    Smudge(SMUDGE_NONE),
    SmudgeFrame(-1),
    OwnerHouse(HOUSES_NONE),
    field_18(HOUSES_NONE),
    OccupierPtr(nullptr),
    OccupantBit(0),
    Land(LAND_CLEAR)
{
    for (int i = 0; i < MZONE_COUNT; ++i) {
        field_6[i] = 0;
    }

    for (int i = 0; i < ARRAY_SIZE(Overlapper); ++i) {
        Overlapper[i] = nullptr;
    }
}

/**
 * @brief Gets the color of the cell based on what it contains and the type of terrain.
 *
 * 0x0049EEF8
 */
int CellClass::Cell_Color(BOOL none) const
{
#ifndef RAPP_STANDALONE
    int(*func)(const CellClass *, BOOL) = reinterpret_cast<int(*)(const CellClass *, BOOL)>(0x0049EEF8);
    return func(this, none);
#elif 0
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);

    static const int _ground_color[9] = { 141, 141, 172, 21, 21, 158, 141, 141, 174 };
    static const int _snow_color[9] = { 141, 141, 172, 21, 21, 158, 141, 141, 174 };

    int color;
    BuildingClass *bptr = Cell_Building();

    if (bptr == nullptr || bptr->Class_Of().IsInvisible) {

        if (none) {
            color = 0;
        } else {
            switch (g_lastTheater) {
                case THEATER_SNOW:
                    color = _snow_color[Land];
                    break;

                default: // All others use this one.
                    color = _ground_color[Land];
                    break;

            }
        }

        return color;
    }

    return ColorRemaps[HouseClass::As_Reference(bptr->Owner()).Color].WindowPalette[7];
#else
    return 0;
#endif
}

/**
 * @brief Returns a pointer to a TechnoClass object in this cell that can be found closest the given pixel coordinates.
 *
 * 0x0049EFBC
 */
TechnoClass *CellClass::Cell_Techno(int x, int y) const
{
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);

    uint32_t coord = Coord_From_Pixel_XY(x, y);
    ObjectClass *object = nullptr;

    for (ObjectClass *obj = OccupierPtr; obj != nullptr; obj = obj->Get_Next()) {
        uint32_t sdistance = 0;

        if (obj->Is_Techno()) {
            uint32_t obj_coord = obj->Center_Coord();
            uint32_t distance = Distance(obj_coord &= 0x00FF00FF, coord);   //TODO what is going on with obj_coord?!  is it fetching sub cell or cell xy?

            if (object == nullptr || (signed int)distance < (signed int)sdistance) {
                sdistance = distance;
                object = obj;
            }
        }
    }

    return reinterpret_cast<TechnoClass *>(object);
}

/**
 * @brief Recalculates the LandType of this cell for movement and pathfind calculations based on current terrain and overlay.
 *
 * 0x0049F314
 */
void CellClass::Recalc_Attributes()
{
    if (g_lastTheater != THEATER_INTERIOR || (Template != TEMPLATE_NONE && Template >= TEMPLATE_FIRST)) {
        if (Overlay == OVERLAY_NONE || (Land = OverlayTypeClass::As_Reference(Overlay).Get_Land(), Land == LAND_CLEAR)) {
            // TODO Bug TEMPLATE_ARRO0003 can only be land type LAND_CLEAR because it is 0xFF, the value that was
            // TEMPLATE_NONE in the TD and SS map formats due to lower number of templates and using uint8_t to hold the
            // value.
            if (Template == TEMPLATE_NONE || Template == 0xFF) {
                Land = LAND_CLEAR;
            } else {
                Land = TemplateTypeClass::As_Reference(Template).Land_Type(Icon);
            }
        }

    } else {
        Land = LAND_ROCK;
    }
}

/**
 * @brief Checks if Ore can grow in this cell. 
 *
 * 0x004A1C4C
 */
BOOL CellClass::Can_Ore_Grow() const
{
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);

    if (Rule.Ore_Grows()) {
        if (Session.Game_To_Play() == GAME_CAMPAIGN || Session.MPlayer_Ore_Growth()) {
            if (Land == LAND_ORE && OverlayFrame < ORESTAGE_FULLGROWN) {	//see OreStageEnum?
                return Contains_Ore();
            }
        }
    }

    return false;
}

/**
 * @brief Checks if Ore can spread from this cell to neighbours.
 *
 * 0x004A1CCC
 */
BOOL CellClass::Can_Ore_Spread() const
{
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);

    if (Rule.Ore_Grows()) {
        if (Session.Game_To_Play() == GAME_CAMPAIGN || Session.MPlayer_Ore_Growth()) {
            if (Land == LAND_ORE && OverlayFrame < ORESTAGE_SPREADING) {	//see OreStageEnum?
                return Contains_Ore();
            }
        }
    }

    return false;
}

/**
 * @brief Checks if Ore can spawn in this cell.
 *
 * 0x004A1E40
 */
BOOL CellClass::Can_Ore_Germinate() const
{
    // TODO Needs BuildingClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)(const CellClass *) = reinterpret_cast<BOOL(*)(const CellClass *)>(0x004A1E40);
    return func(this);
#elif 0
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);

    if (Map.In_Radar(CellNumber)) {
        if (Is_Bridge_Here()) {
            return false;
        }

        BuildingClass *bptr = Cell_Building();

        // IsInvisible needs double checking
        // TS has this checking for "InvisibleInGame", that internally toggles;
        // IsInvisible to true;
        // IsRadarVisible to false;
        // in Read_INI() (TS of course).

        if (bptr == nullptr || bptr->Class_Of().IsInvisible) {
            if (GroundClass::Ground[Land].Is_Buildable()) {
                return Overlay == OVERLAY_NONE;
            }
        }
    }

    return false;
#else
    return false;
#endif
}

/**
 * @brief Performs Ore growth logic in this cell.
 *
 * 0x004A1D4C
 */
BOOL CellClass::Grow_Ore()
{
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);

    if (Can_Ore_Grow()) {
        ++OverlayFrame;
        Redraw_Objects();

        return true;
    }

    return false;
}

/**
 * @brief Performs Ore spread logic to neighbouring cells.
 *
 * 0x004A1D7C
 */
BOOL CellClass::Spread_Ore(BOOL force)
{
    // TODO Requires OverlayClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)(const CellClass *, BOOL) = reinterpret_cast<BOOL(*)(const CellClass *, BOOL)>(0x004A1D7C);
    return func(this, force);
#elif 0
    if (force || Can_Ore_Spread()) {
        // Pick a random direction to start in.
        FacingType start_dir = (FacingType)Scen.Get_Random_Value(FACING_FIRST, FACING_COUNT - 1);

        for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; ++facing) {
            // Check each facing starting with our random one to see if Ore can be spawned there.
            const CellClass &cell = Adjacent_Cell((FacingType)(unsigned(start_dir + facing) % FACING_COUNT));

            if (cell.Can_Ore_Germinate()) {
                OverlayType overlay = (OverlayType)Scen.Get_Random_Value(OVERLAY_GOLD_01, OVERLAY_GOLD_04);
                OverlayClass *optr = new OverlayClass(overlay, cell.CellNumber, OwnerHouse);
                DEBUG_ASSERT(optr != nullptr);
                OverlayFrame = 0;

                return true;
            }
        }
    }

    return false;
#else
    return false;
#endif
}

/**
 * @brief Gets a reference to the cell that is adjacent on the given facing.
 *
 * 0x004A01EC
 */
CellClass &CellClass::Adjacent_Cell(FacingType facing)
{
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);
    DEBUG_ASSERT(facing < FACING_COUNT);

    static const int16_t AdjacentCell[FACING_COUNT] = {
        (-MAP_MAX_WIDTH), // NORTH
        (-MAP_MAX_WIDTH + 1), // NORTH EAST
        (1), // EAST
        (MAP_MAX_WIDTH + 1), // SOUTH EAST
        (MAP_MAX_WIDTH), // SOUTH
        (MAP_MAX_WIDTH - 1), // SOUTH WEST
        (-1), // WEST
        (-MAP_MAX_WIDTH - 1) // NORTH WEST
    };

    if (facing != FACING_NONE && facing < FACING_COUNT) {
        DEBUG_ASSERT_PRINT(uint16_t(CellNumber + AdjacentCell[facing]) <= MAP_MAX_AREA,
            "Attempting to get adjacent cell outside valid map.\n");

        // This logic relies on map cells being held in a contiguous area of memory, which in MapClass they are.
        if (uint16_t(CellNumber + AdjacentCell[facing]) <= MAP_MAX_AREA) {
            return this[AdjacentCell[facing]];
        }
    }

    return *this;
}

/**
 * @brief Checks if this cell is part of a bridge.
 *
 * 0x004A1BE8
 */
BOOL CellClass::Is_Bridge_Here() const
{
    switch (Template) {
        case TEMPLATE_BRIDGE1: //Intentional fall through
        case TEMPLATE_BRIDGE1D:
        case TEMPLATE_BRIDGE2:
        case TEMPLATE_BRIDGE2D:
        case TEMPLATE_BRIDGE1A:
        case TEMPLATE_BRIDGE1B:
        case TEMPLATE_BRIDGE1C:
        case TEMPLATE_BRIDGE2A:
        case TEMPLATE_BRIDGE2B:
        case TEMPLATE_BRIDGE2C:
        case TEMPLATE_BRIDGE3A:
        case TEMPLATE_BRIDGE3B:
        case TEMPLATE_BRIDGE3C:
        case TEMPLATE_BRIDGE3D:
        case TEMPLATE_BRIDGE3E:
        case TEMPLATE_BRIDGE3F:
            return true;

        default:
            break;
    }

    return false;
}

/**
 * @brief Marks any objects in this cell to be redrawn at the next call to redraw the map.
 *
 * 0x0049F10C
 */
void CellClass::Redraw_Objects(BOOL force)
{
    // TODO Requires DisplayClass and RadarClass layers of IOMap hierachy.
#ifndef RAPP_STANDALONE
    void(*func)(const CellClass *, BOOL) = reinterpret_cast<void(*)(const CellClass *, BOOL)>(0x0049F10C);
    func(this, force);
#elif 0
    DEBUG_ASSERT(CellNumber < MAP_MAX_AREA);

    // Check if we are even visible first, then check if already flagged for a redraw.
    if (Map.In_View(CellNumber)) {
        if (force || !Map.Is_Cell_Flagged(CellNumber)) {
            Map.Flag_Cell(CellNumber);

            // Loop through occupiers and if active mark them for redraw.
            for (ObjectClass *optr = OccupierPtr; optr != nullptr; optr = optr->Get_Next()) {
                DEBUG_ASSERT(optr->Is_Active());
                optr->Mark(MARK_REDRAW);
            }

            // Loop through overlappers and if active mark them for redraw.
            for (int j = 0; j < ARRAY_SIZE(Overlapper); ++j) {
                if (Overlapper[j] != nullptr) {
                    DEBUG_ASSERT(Overlapper[j]->Is_Active());
                    Overlapper[j]->Mark(MARK_REDRAW);
                }
            }
        }
    }
#endif
}
