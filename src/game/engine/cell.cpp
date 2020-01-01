/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information pertaining to a single map cell.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cell.h"
#include "aircraft.h"
#include "building.h"
#include "bench.h"
#include "coord.h"
#include "drawshape.h"
#include "gamefile.h"
#include "gbuffer.h"
#include "globals.h"
#include "ground.h"
#include "house.h"
#include "iomap.h"
#include "mixfile.h"
#include "objecttype.h"
#include "pipe.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "straw.h"
#include "target.h"
#include "theater.h"
#include "tileset.h"
#include "tracker.h"
#include <algorithm>
#include <captainslog.h>

// clang-format off
const coord_t CellClass::s_StoppingCoordAbs[CELL_SPOT_COUNT] = {
    0x00800080,     // abs center of cell                       //INFANTRY_SPOT_CENTER
    0x00400040,     // center of top left quadrant of cell      //INFANTRY_SPOT_TOP_LEFT
    0x004000C0,     // center of top right quadrant of cell     //INFANTRY_SPOT_TOP_RIGHT
    0x00C00040,     // center of bottom left quadrant of cell   //INFANTRY_SPOT_BOTTOM_LEFT
    0x00C000C0      // center of bottom right quadrant of cell  //INFANTRY_SPOT_BOTTOM_RIGHT
};
// clang-format on

// Current selected cell when in map editor mode.
// @address 006940AC (beta)
cell_t CellClass::CurrentSelectedCell = -1;

CellClass::CellClass() :
    m_CellNumber(g_Map.Cell_Number(this)),
    m_Bit1(false),
    m_PlacementCheck(false),
    m_Visible(false),
    m_Revealed(false),
    m_Bit16(false),
    m_Bit32(false),
    m_HasFlag(false),
    m_Bit128(false),
    m_field_A(0),
    m_CellTag(-1), // TODO, should be default GamePtr Ctor.
    m_Template(TEMPLATE_NONE),
    m_Icon(0),
    m_Overlay(OVERLAY_NONE),
    m_OverlayFrame(0),
    m_Smudge(SMUDGE_NONE),
    m_SmudgeFrame(0),
    m_OwnerHouse(HOUSES_NONE),
    m_field_18(HOUSES_NONE),
    m_OccupierPtr(nullptr),
    m_OccupantBit(OCCUPANT_NONE),
    m_Land(LAND_CLEAR)
{
    for (int i = 0; i < MZONE_COUNT; ++i) {
        m_Zones[i] = 0;
    }

    for (int i = 0; i < OVERLAPPER_COUNT; ++i) {
        m_Overlapper[i] = nullptr;
    }
}

/**
 * @brief Gets the color of the cell based on what it contains and the type of terrain.
 *
 * 0x0049EEF8
 */
int CellClass::Cell_Color(BOOL none) const
{
    // TODO Needs BuildingClass
#ifdef GAME_DLL
    int (*func)(const CellClass *, BOOL) = reinterpret_cast<int (*)(const CellClass *, BOOL)>(0x0049EEF8);
    return func(this, none);
#elif 0
    captainslog_assert(CellNumber < MAP_MAX_AREA);

    static const int _ground_color[9] = { 141, 141, 172, 21, 21, 158, 141, 141, 174 };
    static const int _snow_color[9] = { 141, 141, 172, 21, 21, 158, 141, 141, 174 };

    int color;
    BuildingClass *bptr = Cell_Building();

    if (bptr == nullptr || bptr->Class_Of().IsInvisible) {
        if (none) {
            color = 0;
        } else {
            switch (g_LastTheater) {
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

    return g_ColorRemaps[HouseClass::As_Reference(bptr->Owner()).Color].WindowPalette[7];
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
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    coord_t coord = Coord_From_Pixel_XY(x, y);
    ObjectClass *object = nullptr;

    for (ObjectClass *obj = m_OccupierPtr; obj != nullptr; obj = obj->Get_Next()) {
        uint32_t sdistance = 0;

        if (obj->Is_Techno()) {
            coord_t obj_coord = obj->Center_Coord();
            uint32_t distance = Coord_Distance(obj_coord &= 0x00FF00FF, coord); // TODO what is going on with obj_coord?! is it fetching sub cell or cell xy?

            if (object == nullptr || (signed int)distance < (signed int)sdistance) {
                sdistance = distance;
                object = obj;
            }
        }
    }

    return reinterpret_cast<TechnoClass *>(object);
}

/**
 * @brief Checks occupiers to see if any are the requested type and returns a pointer to the first one found.
 *
 * 0x0049F084
 */
ObjectClass *CellClass::Cell_Find_Object(RTTIType type) const
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    for (ObjectClass *object = m_OccupierPtr; object != nullptr; object = object->Get_Next()) {
        if (object->What_Am_I() == type) {
            return object;
        }
    }

    return nullptr;
}

/**
 * @brief Checks occupiers to see if any are a building and returns a pointer to the first one found.
 *
 * 0x0049F0B0
 */
BuildingClass *CellClass::Cell_Building() const
{
    return reinterpret_cast<BuildingClass *>(Cell_Find_Object(RTTI_BUILDING));
}

/**
 * @brief Checks occupiers to see if any are infantry and returns a pointer to the first one found.
 *
 * 0x0049F5E4
 */
InfantryClass *CellClass::Cell_Infantry() const
{
    return reinterpret_cast<InfantryClass *>(Cell_Find_Object(RTTI_INFANTRY));
}

/**
 * @brief Checks occupiers to see if any are units and returns a pointer to the first one found.
 *
 * 0x0049F5BC
 */
UnitClass *CellClass::Cell_Unit() const
{
    return reinterpret_cast<UnitClass *>(Cell_Find_Object(RTTI_UNIT));
}

/**
 * @brief Checks occupiers to see if any are vessels and returns a pointer to the first one found.
 *
 * 0x0049F5D0
 */
VesselClass *CellClass::Cell_Vessel() const
{
    return reinterpret_cast<VesselClass *>(Cell_Find_Object(RTTI_VESSEL));
}

/**
 * @brief Checks occupiers to see if any are aircraft and returns a pointer to the first one found.
 */
AircraftClass *CellClass::Cell_Aircraft() const
{
    return reinterpret_cast<AircraftClass *>(Cell_Find_Object(RTTI_AIRCRAFT));
}

/**
 * @brief Checks occupiers to see if any are terrain objects and returns a pointer to the first one found.
 *
 * 0x0049F0C4
 */
TerrainClass *CellClass::Cell_Terrain() const
{
    return reinterpret_cast<TerrainClass *>(Cell_Find_Object(RTTI_TERRAIN));
}

/**
 * @brief Checks occupiers to see if any are techno or terrain objects and returns a pointer to the first one found.
 *
 * 0x0049F0D8
 */
ObjectClass *CellClass::Cell_Object(int x, int y) const
{
    ObjectClass *object = Cell_Aircraft();

    if (object == nullptr) {
        object = Cell_Techno(x, y);

        if (object == nullptr) {
            object = (ObjectClass *)Cell_Terrain();
        }
    }

    return object;
}

/**
 * @brief Recalculates the LandType of this cell for movement and pathfind calculations based on current terrain and overlay.
 *
 * 0x0049F314
 */
void CellClass::Recalc_Attributes()
{
    if (g_LastTheater != THEATER_INTERIOR || (m_Template != TEMPLATE_NONE && m_Template >= TEMPLATE_FIRST)) {
        if (m_Overlay == OVERLAY_NONE || (m_Land = OverlayTypeClass::As_Reference(m_Overlay).Get_Land(), m_Land == LAND_CLEAR)) {
            // TODO Bug TEMPLATE_ARRO0003 can only be land type LAND_CLEAR because it is 0xFF, the value that was
            // TEMPLATE_NONE in the TD and SS map formats due to lower number of templates and using uint8_t to hold the
            // value.
            if (m_Template == TEMPLATE_NONE || m_Template == 0xFF) {
                m_Land = LAND_CLEAR;
            } else {
                m_Land = TemplateTypeClass::As_Reference(m_Template).Land_Type(m_Icon);
            }
        }

    } else {
        m_Land = LAND_ROCK;
    }
}

/**
 * @brief Checks if Ore can grow in this cell.
 *
 * 0x004A1C4C
 */
BOOL CellClass::Can_Ore_Grow() const
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (g_Rule.Ore_Grows()) {
        if (g_Session.Game_To_Play() == GAME_CAMPAIGN || g_Session.MPlayer_Ore_Growth()) {
            if (m_Land == LAND_ORE && m_OverlayFrame < ORESTAGE_FULLGROWN) { // see OreStageEnum?
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
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (g_Rule.Ore_Spreads()) {
        if (g_Session.Game_To_Play() == GAME_CAMPAIGN || g_Session.MPlayer_Ore_Growth()) {
            if (m_Land == LAND_ORE && m_OverlayFrame > ORESTAGE_SPREADING) { // see OreStageEnum?
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
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (!g_Map.In_Radar(m_CellNumber)) {
        return false;
    }
    if (Is_Bridge_Here()) {
        return false;
    }

    BuildingClass *bptr = Cell_Building();

    // IsInvisible needs double checking
    // TS has this checking for "InvisibleInGame", that internally toggles;
    // IsInvisible to true;
    // IsRadarVisible to false;
    // in Read_INI() (TS of course).
    if (bptr != nullptr && reinterpret_cast<const BuildingTypeClass &>(bptr->Class_Of()).Is_Invisible()) {
        return false;
    }
    if (g_Ground[m_Land].Is_Buildable()) {
        return m_Overlay == OVERLAY_NONE;
    }
    return false;
}

/**
 * @brief Performs Ore growth logic in this cell.
 *
 * 0x004A1D4C
 */
BOOL CellClass::Grow_Ore()
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (Can_Ore_Grow()) {
        ++m_OverlayFrame;
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
#ifdef GAME_DLL
    BOOL (*func)(const CellClass *, BOOL) = reinterpret_cast<BOOL (*)(const CellClass *, BOOL)>(0x004A1D7C);
    return func(this, force);
#elif 0
    if (force || Can_Ore_Spread()) {
        // Pick a random direction to start in.
        FacingType start_dir = (FacingType)g_Scen.Get_Random_Value(FACING_FIRST, FACING_COUNT - 1);

        for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; ++facing) {
            // Check each facing starting with our random one to see if Ore can be spawned there.
            const CellClass &cell = Adjacent_Cell((FacingType)(unsigned(start_dir + facing) % FACING_COUNT));

            if (cell.Can_Ore_Germinate()) {
                OverlayType overlay = (OverlayType)g_Scen.Get_Random_Value(OVERLAY_GOLD_01, OVERLAY_GOLD_04);
                OverlayClass *optr = new OverlayClass(overlay, cell.m_CellNumber, m_OwnerHouse);
                captainslog_assert(optr != nullptr);
                m_OverlayFrame = 0;

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
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);
    // captainslog_assert(facing < FACING_COUNT);

    static const cell_t AdjacentCell[FACING_COUNT] = {
        (-MAP_MAX_WIDTH), // NORTH
        (-MAP_MAX_WIDTH + 1), // NORTH EAST
        (1), // EAST
        (MAP_MAX_WIDTH + 1), // SOUTH EAST
        (MAP_MAX_WIDTH), // SOUTH
        (MAP_MAX_WIDTH - 1), // SOUTH WEST
        (-1), // WEST
        (-MAP_MAX_WIDTH - 1) // NORTH WEST
    };

    if (facing < FACING_COUNT) {
        captainslog_dbgassert(uint16_t(m_CellNumber + AdjacentCell[facing]) <= MAP_MAX_AREA,
            "Attempting to get adjacent cell outside valid map.\n");

        // This logic relies on map cells being held in a contiguous area of memory, which in MapClass they are.
        if (uint16_t(m_CellNumber + AdjacentCell[facing]) <= MAP_MAX_AREA) {
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
    switch (m_Template) {
        case TEMPLATE_BRIDGE1: // Intentional fall through
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
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    // Check if we are even visible first, then check if already flagged for a redraw.
    if (g_Map.In_View(m_CellNumber)) {
        if (force || !g_Map.Is_Cell_Flagged(m_CellNumber)) {
            g_Map.Flag_Cell(m_CellNumber);

            // Loop through occupiers and if active mark them for redraw.
            for (ObjectClass *optr = m_OccupierPtr; optr != nullptr; optr = optr->Get_Next()) {
                captainslog_assert(optr->Is_Active());
                if (!optr->Is_Active()) {
                    break;
                }
                optr->Mark(MARK_REDRAW);
                ObjectClass *next = optr->Get_Next();
                if (next != nullptr && !next->Is_Active()) {
                    optr->Set_Next(nullptr);
                }
            }

            // Loop through overlappers and if active mark them for redraw.
            for (int i = 0; i < ARRAY_SIZE(m_Overlapper); ++i) {
                if (m_Overlapper[i] != nullptr) {
                    captainslog_assert(m_Overlapper[i]->Is_Active());
                    if (m_Overlapper[i]->Is_Active()) {
                        m_Overlapper[i]->Mark(MARK_REDRAW);
                    } else {
                        m_Overlapper[i] = nullptr;
                    }
                }
            }
        }
    }
}

/**
 * @brief Check if this cell can be built upon.
 *
 * 0x0049F1FC
 */
BOOL CellClass::Is_Clear_To_Build(SpeedType speed) const
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (g_ScenarioInit) {
        return true;
    }

    if (Cell_Object(1, 0) != nullptr) {
        return false;
    }

    if (m_HasFlag) {
        return false;
    }

    if ((m_Overlay != OVERLAY_NONE && m_OverlayFrame != -1)
        && (!g_InMapEditor || m_Overlay == OVERLAY_FPLS || OverlayTypeClass::As_Reference(m_Overlay).Is_Wall())) {
        return false;
    }

    if ((m_Smudge != SMUDGE_NONE && m_SmudgeFrame != -1) && SmudgeTypeClass::As_Reference(m_Smudge).Is_Bib()) {
        return false;
    }

    if (speed != SPEED_NONE) {
        if (g_Ground[m_Land].Get_Speed(speed) == fixed_t::_0_1) {
            return false;
        }

        return true;
    }

    if (Is_Bridge_Here()) {
        return false;
    }

    return g_Ground[m_Land].Is_Buildable();
}

/**
 * @brief Move an object down the occupy list for this cell.
 *
 * 0x0049F394
 */
void CellClass::Occupy_Down(ObjectClass *object)
{
    // TODO Requires RadarClass layer of IOMap hierachy.
#ifdef GAME_DLL
    void (*func)(const CellClass *, ObjectClass *) =
        reinterpret_cast<void (*)(const CellClass *, ObjectClass *)>(0x0049F394);
    func(this, object);
#elif 0
    captainslog_assert(CellNumber < MAP_MAX_AREA);
    captainslog_assert(object->Is_Active());

    if (object != nullptr) {
        // If object is a building, and already have a cell occupier, find end of occupier chanin and stick object on the
        // end. Else stick object on the front.
        if (object->What_Am_I() == RTTI_BUILDING && OccupierPtr != nullptr) {
            SmartPtr<ObjectClass> *next_ptr;

            for (ObjectClass *next = OccupierPtr; next != nullptr; next = next->Get_Next()) {
                next_ptr = &next->Get_Next();
            }

            *next_ptr = object;
            object->Get_Next() = nullptr;
        } else {
            object->Get_Next() = OccupierPtr;
            OccupierPtr = object;
        }

        // Dunno what this does yet.
        g_Map.Radar_Pixel(CellNumber);

        if (Visible && g_Session.Game_To_Play() != GAME_CAMPAIGN) {
            object->Revealed(PlayerPtr);
        }

        // Set OccupantBit based on type of object.
        switch (object->What_Am_I()) {
            case RTTI_BUILDING:
                OccupantBit |= OCCUPANT_BUILDING;
                break;

            case RTTI_AIRCRAFT: // Fall through
            case RTTI_UNIT: // Fall through
            case RTTI_VESSEL:
                OccupantBit |= OCCUPANT_UNIT;
                break;

            case RTTI_TERRAIN:
                OccupantBit |= OCCUPANT_TERRAIN;
                break;

            default:
                break;
        }
    }
#endif
}

/**
 * @brief Move an object up the occupy list for this cell.
 *
 * 0x0049F450
 */
void CellClass::Occupy_Up(ObjectClass *object)
{
    // TODO Requires RadarClass layer of IOMap hierachy.
#ifdef GAME_DLL
    void (*func)(const CellClass *, ObjectClass *) =
        reinterpret_cast<void (*)(const CellClass *, ObjectClass *)>(0x0049F450);
    func(this, object);
#elif 0
    captainslog_assert(CellNumber < MAP_MAX_AREA);
    captainslog_assert(object->Is_Active());

    if (object != nullptr) {
        if (OccupierPtr == object) {
            OccupierPtr = object->Get_Next();
            object->Next = nullptr;
        } else {
            bool found = false;

            for (ObjectClass *next = OccupierPtr; next != nullptr; next = next->Get_Next()) {
                SmartPtr<ObjectClass> &next_ptr = next->Get_Next();

                if (object == next) {
                    next_ptr = object->Get_Next();
                    object->Get_Next() = nullptr;
                    found = true;
                    break;
                }
            }
        }

        // Dunno what this does yet. Updates radar pixel color for this cell?
        g_Map.Radar_Pixel(CellNumber);

        // Clear OccupantBit based on type of object.
        switch (object->What_Am_I()) {
            case RTTI_BUILDING:
                OccupantBit &= ~OCCUPANT_BUILDING;
                break;

            case RTTI_AIRCRAFT: // Fall through
            case RTTI_UNIT: // Fall through
            case RTTI_VESSEL:
                OccupantBit &= ~OCCUPANT_UNIT;
                break;

            case RTTI_TERRAIN:
                OccupantBit &= ~OCCUPANT_TERRAIN;
                break;

            default:
                break;
        }
    }
#endif
}

/**
 * @brief Move an object down the overlap list for this cell.
 *
 * 0x0049F4E4
 */
void CellClass::Overlap_Down(ObjectClass *object)
{
    // TODO Requires HouseClass.
#ifdef GAME_DLL
    void (*func)(const CellClass *, ObjectClass *) =
        reinterpret_cast<void (*)(const CellClass *, ObjectClass *)>(0x0049F4E4);
    func(this, object);
#elif 0
    captainslog_assert(CellNumber < MAP_MAX_AREA);
    captainslog_assert(object->Is_Active());

    if (object == nullptr) {
        return;
    }

    ObjectClass **overlap_slot = nullptr;

    // Go through our list over overlappers, return if object is already in otherwise get pointer to last free index.
    for (int i = 0; i < ARRAY_SIZE(Overlapper); ++i) {
        if (Overlapper[i] == object) {
            return;
        }

        if (Overlapper[i] == nullptr) {
            overlap_slot = &Overlapper[i];
        }
    }

    // If object is a building and it didn't get a free slot, loop through and try and find a slot that isn't already a
    // building or terrain and use that one instead.
    if (overlap_slot == nullptr && object->What_Am_I() == RTTI_BUILDING) {
        for (int i = 0; i < ARRAY_SIZE(Overlapper); ++i) {
            if (Overlapper[i]->What_Am_I() != RTTI_BUILDING && Overlapper[i]->What_Am_I() != RTTI_TERRAIN) {
                Overlapper[i] = object;
                break;
            }
        }
    }

    // If we got a slot, just use it.
    if (overlap_slot != nullptr) {
        *overlap_slot = object;
    }

    // Reveal object to the player?
    if (Visible) {
        object->Revealed(PlayerPtr);
    }
#endif
}

/**
 * @brief Move an object up the overlap list for this cell.
 *
 * 0x0049F590
 */
void CellClass::Overlap_Up(ObjectClass *object)
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);
    captainslog_assert(object->Is_Active());

    for (int i = 0; i < ARRAY_SIZE(m_Overlapper); ++i) {
        if (m_Overlapper[i] == object) {
            m_Overlapper[i] = nullptr;
            break;
        }
    }

    if (g_InMapEditor) {
        g_Map.Radar_Pixel(m_CellNumber);
    }
}

/**
 * @brief Returns the tile in a clear set to use for this cell number to give impression of randomness.
 *
 * 0x004A010C
 */
int CellClass::Clear_Icon() const
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);
    return ((m_CellNumber >> 5) & 0xC) | (m_CellNumber & 0x3);
}

/**
 * @brief Draws the cell and some of its contents.
 *
 * 0x0049F5F8
 */
void CellClass::Draw_It(int x, int y, BOOL flag) const
{
    /*#ifdef GAME_DLL
        void (*func)(const CellClass *, int, int, BOOL) =
            reinterpret_cast<void (*)(const CellClass *, int, int, BOOL)>(0x0049F5F8);
        func(this, x, y, unk_bool);
    #elif 0*/
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    int icon_num = 0;
    void *fading_table = nullptr;
    ObjectClass *obj = nullptr;
    TemplateTypeClass *tt = nullptr;

    GraphicViewPortClass dbgcell(g_LogicPage->Get_Graphic_Buffer(),
        g_LogicPage->Get_XPos() + g_WindowList[WINDOW_TACTICAL].X,
        g_LogicPage->Get_YPos() + g_WindowList[WINDOW_TACTICAL].Y,
        g_WindowList[WINDOW_TACTICAL].W,
        g_WindowList[WINDOW_TACTICAL].H);

    if (!flag) {

        BENCHMARK_START(BENCH_CELL);

        //++CellCount; // Original has a global variable here. Not sure what this is used for.

        // TODO
        // TEMPLATE_ARRO0003 (interior tileset) is tile 255, but that value is treated as clear in many vanilla maps, even
        // interior ones so it cannot be used. Possible solution is to handle this at map load and if no 0xFFFF values
        // found, convert any 0xFF to 0xFFFF and only handle 0xFFFF here?
        if (m_Template != TEMPLATE_NONE && m_Template != 255 && m_Template != TEMPLATE_CLEAR && m_Template < TEMPLATE_COUNT) {
            tt = &TemplateTypeClass::As_Reference(m_Template);
            icon_num = m_Icon;
        } else {
            tt = &TemplateTypeClass::As_Reference(TEMPLATE_CLEAR);
            icon_num = Clear_Icon();
        }

#ifdef CHRONOSHIFT_DEBUG
        if (g_Debug_Passable) {
            // editor version from edwin
            if (g_InMapEditor) {
                // impassable
                if (g_Ground[m_Land].Get_Speed(SPEED_FOOT) == fixed_t(0, 0) || m_OccupierPtr != nullptr) {
                    fading_table = DisplayClass::s_FadingRed;
                }
                // is occupied by multiple objects, could had been for finding overlapping objects?
                if (m_OccupierPtr != nullptr && m_OccupierPtr->Get_Next() != nullptr) {
                    fading_table = DisplayClass::s_FadingYellow;
                }
            // ingame version from RA beta
            } else {
                // impassable
                if (g_Ground[m_Land].Get_Speed(SPEED_FOOT) == fixed_t(0, 0)
                    || m_OccupierPtr != nullptr && m_OccupierPtr->What_Am_I() != RTTI_INFANTRY) {
                    fading_table = DisplayClass::s_FadingRed;
                // unit will be very slow on this cell, in a unmodified RA this won't ever be true
                } else if (g_Ground[m_Land].Get_Speed(SPEED_FOOT) <= fixed_t(1, 3)) {
                    fading_table = DisplayClass::s_FadingYellow;
                // fully passable
                } else {
                    fading_table = DisplayClass::s_FadingGreen;
                }
            }
        }
#endif

        if (tt->Get_Image_Data() != nullptr) {
            g_LogicPage->Draw_Stamp(tt->Get_Image_Data(),
                icon_num,
                x,
                y,
                nullptr,
                g_WindowList[WINDOW_TACTICAL].X,
                g_WindowList[WINDOW_TACTICAL].Y,
                g_WindowList[WINDOW_TACTICAL].W,
                g_WindowList[WINDOW_TACTICAL].H);
#ifdef CHRONOSHIFT_DEBUG
            if (g_Debug_Passable) {
                if (fading_table != nullptr) {
                    GraphicViewPortClass remap(g_LogicPage->Get_Graphic_Buffer(),
                        g_LogicPage->Get_XPos() + g_WindowList[WINDOW_TACTICAL].X,
                        g_LogicPage->Get_YPos() + g_WindowList[WINDOW_TACTICAL].Y,
                        g_WindowList[WINDOW_TACTICAL].W,
                        g_WindowList[WINDOW_TACTICAL].H);
                    // original line, i think this could result in out of bounds draws so using what edwin does instead
                    // g_LogicPage->Remap(x + g_Map.Tac_Offset_X(), y + g_Map.Tac_Offset_Y(), CELL_PIXELS, CELL_PIXELS, (unsigned char *)fading_table);
                    remap.Remap(x, y, CELL_PIXELS, CELL_PIXELS, (unsigned char *)fading_table);
                }
            }
#endif
        }
#ifdef CHRONOSHIFT_DEBUG
        if (g_InMapEditor) {
            if (CurrentSelectedCell == m_CellNumber) {
                // TODO!
            }
        }
#endif
        if (m_Smudge != SMUDGE_NONE && m_SmudgeFrame != -1) {
            SmudgeTypeClass::As_Reference(m_Smudge).Draw_It(x, y, m_SmudgeFrame);
        }

        if (m_Overlay != OVERLAY_NONE && m_OverlayFrame != -1) {
            // TODO this looks like a hack in the original code, since OverlayTypeClass::Draw_It doesn't
            // clip to the tactical view as it should.
            // OverlayTypeClass::As_Reference(m_Overlay).Draw_It(x, y, m_OverlayFrame);
            OverlayTypeClass &otc = OverlayTypeClass::As_Reference(m_Overlay);
            g_IsTheaterShape = otc.Is_Theater();
            CC_Draw_Shape(otc.Get_Image_Data(),
                m_OverlayFrame,
                x + 12,
                y + 12,
                WINDOW_TACTICAL,
                SHAPE_GHOST | SHAPE_WIN_REL | SHAPE_CENTER,
                nullptr,
                DisplayClass::s_UnitShadow);
            g_IsTheaterShape = false;
        }

        if (m_PlacementCheck) {
            SpeedType speed = SPEED_NONE;

            if (g_Map.Pending_ObjectType() != nullptr && g_Map.Pending_ObjectType()->What_Am_I() == RTTI_BUILDING) {
                speed = reinterpret_cast<TechnoTypeClass *>(g_Map.Pending_ObjectType())->Get_Speed();
            }

            int placement_icon = PLACEMENT_CLEAR;

            // TODO this is an enhancement to make unit occupied cells yellow rather than red.
            /*if (!g_Map.Passed_Proximity_Check() && Get_Occupier() != nullptr && Get_Occupier()->What_Am_I() != RTTI_BUILDING)
            { placement_icon = PLACEMENT_YELLOW; } else if (!g_Map.Passed_Proximity_Check() || !Is_Clear_To_Build(speed)) {
                placement_icon = PLACEMENT_RED;
            }*/

            if (!g_Map.Passed_Proximity_Check() || !Is_Clear_To_Build(speed)) {
                placement_icon = PLACEMENT_RED;
            }

            g_LogicPage->Draw_Stamp(DisplayClass::s_TransIconset,
                placement_icon,
                x,
                y,
                nullptr,
                g_WindowList[WINDOW_TACTICAL].X,
                g_WindowList[WINDOW_TACTICAL].Y,
                g_WindowList[WINDOW_TACTICAL].W,
                g_WindowList[WINDOW_TACTICAL].H);

#if 0 // TODO Edwin map stuff.
            if (g_Debug_Placement_Ghosts) {
                if (g_Map.Pending_ObjectType() != nullptr) {
                    switch (g_Map.Pending_ObjectType()->What_Am_I()) {
                        case RTTI_VESSEL:
                            // TODO

                        case RTTI_BUILDING:
                            // TODO

                        case RTTI_UNIT:
                            // TODO

                        case RTTI_INFANTRY:
                            // TODO

                        case RTTI_AIRCRAFT:
                            // TODO
                        case RTTI_OVERLAYTYPE:
                            break;
                        case RTTI_SMUDGETYPE:
                            break;
                        case RTTI_TEMPLATETYPE:
                            break;

                        default:
                            break;
                    }
                }
            }
#endif
        }

        if (m_HasFlag) {
            void *flag_shape = GameFileClass::Retrieve("flagfly.shp");

            // 'flag_frame' will be the number of frames in the shape sequence, so it draws it based on what frame the
            // game is on, wrapped so it goes 0 to 13. So when game frame is 14 it will return 0, 15 will return 1 and so on.
            int flag_frame = g_GameFrame % Get_Build_Frame_Count(flag_shape); // shape has 14 frames

            // 12 is what is set, but the shape is 23, so im gonna take it as / 2. or is this centered to a icon w/h? (24 sq)
            int flag_x = x + (Get_Build_Frame_Width(flag_shape) / 2) + 1;
            int flag_y = y + (Get_Build_Frame_Height(flag_shape) / 2) + 1;

            CC_Draw_Shape(flag_shape,
                flag_frame,
                flag_x,
                flag_y,
                WINDOW_TACTICAL,
                SHAPE_SHADOW | SHAPE_FADING | SHAPE_CENTER,
                (void *)HouseClass::As_Pointer(m_OwnerHouse)->Remap_Table(false, REMAP_1),
                DisplayClass::s_UnitShadow);
        }

        BENCHMARK_END(BENCH_CELL);

    }
    //#endif
}

/**
 * @brief Unknown, empty function in RA.
 *
 * 0x0049FC50
 */
void CellClass::Concrete_Calc()
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    static FacingType _even[] = {
        FACING_NORTH, FACING_SOUTH, FACING_SOUTH_WEST, FACING_WEST, FACING_NORTH_WEST, FACING_NORTH
    };

    static FacingType _odd[] = {
        FACING_NORTH, FACING_NORTH_EAST, FACING_EAST, FACING_SOUTH_EAST, FACING_SOUTH, FACING_NORTH
    };

    // TODO Function exists in RA, but just returns, C&C has full code though. Is logic unique to C&C or just elsewhere in
    // RA? Could be left over from concrete placement in Dune2 which was also in early versions of C&C.
}

/**
 * @brief Updates any wall logic overlay in this cell.
 *
 * 0x0049FC58
 */
void CellClass::Wall_Update()
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    static FacingType _offsets[] = { FACING_NORTH, FACING_EAST, FACING_SOUTH, FACING_WEST, FACING_NONE };

    for (int index = 0; index < ARRAY_SIZE(_offsets); ++index) {
        CellClass &adjcell = Adjacent_Cell(_offsets[index]);

        if (adjcell.m_Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(adjcell.m_Overlay).Is_Wall()) {
            int neighbour_mask = 0;

            // Identify neighbour cells that are the same overlay.
            for (int j = 0; j < ARRAY_SIZE(_offsets) - 1; ++j) {
                if (adjcell.Adjacent_Cell(_offsets[j]).m_Overlay == adjcell.m_Overlay) {
                    neighbour_mask |= (1 << j);
                }
            }

            // What is going on here? Okay, the & 0xF0 masks it to be either 0, 16 or 32 in practice which determines if its
            // a damaged state or not, then the | adds the frame offset based on the adjacent cells as determined by the loop
            // above.
            adjcell.m_OverlayFrame = (adjcell.m_OverlayFrame & 0xF0) | neighbour_mask;

            // m_OverlayFrame checked against here are blanks in the art
            // Frame 48 for BRIK is after the first damage state set
            if (adjcell.m_Overlay == OVERLAY_BRICK_WALL && adjcell.m_OverlayFrame == 48) {
                adjcell.m_OverlayFrame = 0;
                adjcell.m_Overlay = OVERLAY_NONE;
                adjcell.m_OwnerHouse = HOUSES_NONE; // C&C DOS sets this, bug fix perhaps?
                Detach_This_From_All(As_Target(adjcell.m_CellNumber));
            }

            // Frame 16 for SBAG is after the nondamaged set
            if (adjcell.m_Overlay == OVERLAY_SANDBAG && adjcell.m_OverlayFrame == 16) {
                adjcell.m_OverlayFrame = 0;
                adjcell.m_Overlay = OVERLAY_NONE;
                adjcell.m_OwnerHouse = HOUSES_NONE;
                Detach_This_From_All(As_Target(adjcell.m_CellNumber));
            }

            // Frame 32 for CYCL is after the first damage state set
            if (adjcell.m_Overlay == OVERLAY_CYCLONE_FENCE && adjcell.m_OverlayFrame == 32) {
                adjcell.m_OverlayFrame = 0;
                adjcell.m_Overlay = OVERLAY_NONE;
                adjcell.m_OwnerHouse = HOUSES_NONE;
                Detach_This_From_All(As_Target(adjcell.m_CellNumber));
            }

            // Frame 16 for FENC is after the nondamaged set, on a 0 basis frame 32 doesn't exist, some hack for older art?
            if (adjcell.m_Overlay == OVERLAY_FENCE && (adjcell.m_OverlayFrame == 16 || adjcell.m_OverlayFrame == 32)) {
                adjcell.m_OverlayFrame = 0;
                adjcell.m_Overlay = OVERLAY_NONE;
                adjcell.m_OwnerHouse = HOUSES_NONE;
                Detach_This_From_All(As_Target(adjcell.m_CellNumber));
            }

            // Frame 16 for SBAG is after the nondamaged set
            if (adjcell.m_Overlay == OVERLAY_BARB_WIRE && adjcell.m_OverlayFrame == 16) {
                adjcell.m_OverlayFrame = 0;
                adjcell.m_Overlay = OVERLAY_NONE;
                adjcell.m_OwnerHouse = HOUSES_NONE;
                Detach_This_From_All(As_Target(adjcell.m_CellNumber));
            }

            adjcell.Recalc_Attributes();
            adjcell.Redraw_Objects();
        }
    }
}

/**
 * @brief Gets this cells position as a Coord.
 *
 * 0x0049FDE0
 */
coord_t CellClass::Cell_Coord() const
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    return Cell_To_Coord(m_CellNumber);
}

/**
 * @brief Reduces the amount of ore in a cell and returns the amount it was reduced by.
 *
 * 0x0049FE18
 */
int CellClass::Reduce_Ore(int reduction)
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (reduction > 0 && m_Land == LAND_ORE) {
        // If we have a greater reduction than we have frames, set to -1 and return the frame we were on as the reduction,
        // else reduce by reduction
        if (m_OverlayFrame + 1 <= reduction) {
            int oldframe = m_OverlayFrame;
            m_Overlay = OVERLAY_NONE;
            m_OverlayFrame = -1;
            Recalc_Attributes();

            return oldframe;
        } else {
            m_OverlayFrame -= reduction;

            return reduction;
        }
    }

    return 0;
}

/**
 * @brief Perform wall reduction logic for a given damage.
 *
 * 0x0049FE58
 */
BOOL CellClass::Reduce_Wall(int damage)
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (m_Overlay != OVERLAY_NONE) {
        OverlayTypeClass &overlay = OverlayTypeClass::As_Reference(m_Overlay);
        bool is_reduced = false;

        // This looks like it tests if an overlay was reduced by the value in damage. -1 skips the check and auto destroys
        // the wall. As far as I can tell anyhow.
        if (overlay.Is_Wall()) {
            if (damage != -1 && overlay.Get_Overlay_Strength() > damage) {
                is_reduced = overlay.Get_Overlay_Strength() > damage ? g_Scen.Get_Random_Value(0, damage) < damage : true;
            } else {
                is_reduced = true;
            }
        }

        if (is_reduced) {
            m_OverlayFrame += 16;

            if ((damage == -1) || (m_OverlayFrame / 16) >= overlay.Get_Damage_Levels()
                || ((m_OverlayFrame / 16) == (overlay.Get_Damage_Levels() - 1) && (m_OverlayFrame % 16) == 0)) {
                m_OwnerHouse = HOUSES_NONE;
                m_Overlay = OVERLAY_NONE;
                m_OverlayFrame = -1;
                Recalc_Attributes();
                Redraw_Objects(0);
                Adjacent_Cell(FACING_NORTH).Wall_Update();
                Adjacent_Cell(FACING_WEST).Wall_Update();
                Adjacent_Cell(FACING_SOUTH).Wall_Update();
                Adjacent_Cell(FACING_EAST).Wall_Update();
                Detach_This_From_All(As_Target(Cell_Number()));

                if (overlay.Is_Crushable()) {
                    g_Map.Zone_Reset(1 << MZONE_NORMAL);
                } else {
                    g_Map.Zone_Reset((1 << MZONE_NORMAL) | (1 << MZONE_CRUSHER));
                }

                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Check if the cell is clear for movement.
 *
 * @param ignore_crushable ignores any infantry in the cell when checking if occupied.
 * @param ignore_destructable ignores any units or buildings when checking if occupied.
 *
 * 0x004A1B0C
 */
BOOL CellClass::Is_Clear_To_Move(
    SpeedType speed, BOOL ignore_crushable, BOOL ignore_destructable, int zone, MZoneType mzone) const
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (speed == SPEED_WINGED) {
        return true;
    }

    if (zone != -1 && m_Zones[mzone] != zone) {
        return false;
    }

    uint32_t occupier = m_OccupantBit;

    // Mask out infantry so they are ignored.
    if (ignore_crushable) {
        occupier &= OCCUPANT_UNIT | OCCUPANT_TERRAIN | OCCUPANT_BUILDING;
    }

    // Mask out units/buildings so they are ignored.
    if (ignore_destructable) {
        occupier &= OCCUPANT_INFANTRY | OCCUPANT_TERRAIN;
    }

    // Check if occupied.
    if (occupier != OCCUPANT_NONE) {
        return false;
    }

    LandType land = m_Land;

    if (m_Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(m_Overlay).Is_Wall()) {
        if (mzone != MZONE_DESTROYER
            && (mzone != MZONE_CRUSHER || !OverlayTypeClass::As_Reference(m_Overlay).Is_Crushable())) {
            return false;
        }

        land = LAND_CLEAR;
    }

    if (land == LAND_NONE) {
        return false;
    }

    // Check to make sure our speed type can actually move on this ground.
    return (g_Ground[land].Get_Speed(speed) != fixed_t::_0_1);
}

/**
 * @brief Adjusts the amount of Ore in a cell.
 *
 * 0x004A02BC
 */
int CellClass::Ore_Adjust(BOOL randomize)
{
    static int _adj[] = { 0, 1, 3, 4, 6, 7, 8, 10, 11 };
    static int _adjgem[] = { 0, 0, 0, 1, 1, 1, 2, 2, 2 };

    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    if (m_Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(m_Overlay).Get_Land() == LAND_ORE) {
        int value = 0;
        int adjustment = 0;
        bool is_gems = false;

        if (randomize) {
            switch (m_Overlay) {
                case OVERLAY_GOLD_01:
                case OVERLAY_GOLD_02:
                case OVERLAY_GOLD_03:
                case OVERLAY_GOLD_04: // Fallthrough
                    is_gems = false;
                    value = g_Rule.Get_Gold_Value();
                    m_Overlay = OverlayType(g_Scen.Get_Random_Value(OVERLAY_GOLD_01, OVERLAY_GOLD_04));
                    break;

                case OVERLAY_GEM_01:
                case OVERLAY_GEM_02:
                case OVERLAY_GEM_03:
                case OVERLAY_GEM_04: // Fallthrough
                    is_gems = true;
                    value = g_Rule.Get_Gem_Value() * 4;
                    m_Overlay = OverlayType(g_Scen.Get_Random_Value(OVERLAY_GEM_01, OVERLAY_GEM_04));
                    break;

                default:
                    break;
            }
        }

        int frame = m_OverlayFrame;

        for (FacingType facing = FACING_NORTH; facing < FACING_COUNT; ++facing) {
            CellClass const &cell = Adjacent_Cell(facing);

            if (cell.m_Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(cell.m_Overlay).Get_Land() == LAND_ORE) {
                ++adjustment;
            }
        }

        captainslog_assert(adjustment < ARRAY_SIZE(_adj));
        captainslog_assert(adjustment < ARRAY_SIZE(_adjgem));

        if (is_gems) {
            m_OverlayFrame = std::max(2, _adjgem[adjustment]);
        } else {
            m_OverlayFrame = _adj[adjustment];
        }

        // This is done in edwin but not final game?
        // if (m_OverlayFrame != frame) {
        //    Redraw_Objects();
        //}

        return value * (m_OverlayFrame + 1);
    }

    return 0;
}

/**
 *
 *
 */
coord_t CellClass::Closest_Free_Spot(coord_t coord, BOOL skip_occupied) const
{
    captainslog_assert(m_CellNumber < MAP_MAX_AREA);

    // clang-format off
    static char _sequence[][4] = {
        { 1, 2, 3, 4 },
        { 0, 2, 3, 4 },
        { 0, 1, 4, 3 },
        { 0, 1, 4, 2 },
        { 0, 2, 3, 1 }
    };

    static char _alternate[][4] = {
        { 1, 2, 3, 4 },
        { 2, 3, 4, 1 },
        { 3, 4, 1, 2 },
        { 4, 1, 2, 3 }
    };
    // clang-format on

    int spotindex = Spot_Index(coord);
    coord_t top_left = Coord_Top_Left(coord);

    // If we have a unit or terrain object, return is 0;
    if (!skip_occupied && (m_OccupantBit & OCCUPANT_UNIT || m_OccupantBit & OCCUPANT_TERRAIN)) {
        return 0;
    }

    // If our intended spot is free or we are skipping occupied, return it.
    if (skip_occupied || Is_Spot_Free(spotindex)) {
        return Coord_Add(top_left, s_StoppingCoordAbs[spotindex]);
    }

    // If we already have an occupier on our intended spot, recalculate next best.
    char *spots = nullptr;
    if (spotindex == 0) {
        spots = _alternate[g_Scen.Get_Random_Value((m_OccupantBit & INFANTRY_SPOT_TOP_LEFT) == 0, 3)];
    } else {
        spots = _sequence[spotindex];
    }

    // From our possible spots list, find a free one, if not, return 0.
    for (int i = 0; i < 4; ++i) {
        if (Is_Spot_Free(spots[i])) {
            return Coord_Add(top_left, s_StoppingCoordAbs[spots[i]]);
        }
    }

    return 0;
}

/**
 * 0x0049FF98
 */
int CellClass::Spot_Index(coord_t coord)
{
    coord_t spot = Coord_Sub_Cell(coord);

    // Looks like it checks the lepton distance and then does some math on the X and Y lepton dimensions to decide which spot
    // the passed packed coords are in.
    if (Coord_Distance(spot, 0x00800080) >= 60) {
        int spot_index = 0;

        if (Coord_Lepton_X(spot) > 128) {
            spot_index = 1;
        }

        if (Coord_Lepton_Y(spot) > 128) {
            spot_index |= 2;
        }

        return spot_index + 1;
    }

    return 0;
}

/**
 * @brief Performs a check for the result of a crate collection from the look of it.
 *
 * 0x004A0460
 */
BOOL CellClass::Goodie_Check(FootClass *foot)
{
#ifdef GAME_DLL
    BOOL (*func)(const CellClass *, FootClass *) = reinterpret_cast<BOOL (*)(const CellClass *, FootClass *)>(0x004A0460);
    return func(this, foot);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

/**
 * @brief Checks if a cells state is not the default state.
 *
 * 0x004F8E08
 */
BOOL CellClass::Should_Save() const
{
    // Default constructed cell to compare other cells against.
    // TODO, this test will always return true as each cell has its ID encoded so will never fully match. Bug in original.
    static const CellClass _identity_cell;
    // static const uintptr_t _null_array[OVERLAPPER_COUNT] = { 0 };
    return memcmp(&_identity_cell, this, sizeof(*this)) != 0;

    /* return Bit1 != false || PlacementCheck != false
        || Visible != false || Revealed != false || Bit16 != false
        || Bit32 != false || HasFlag != false || Bit128 != false
        || field_A != 0 || CellTag != -1 || Template != TEMPLATE_NONE
        || Icon != 0 || Overlay != OVERLAY_NONE || OverlayFrame != 0
        || Smudge != SMUDGE_NONE || SmudgeFrame != 0
        || OwnerHouse != HOUSES_NONE || field_18 != HOUSES_NONE
        || OccupierPtr != nullptr || OccupantBit != 0
        || Land != LAND_CLEAR || memcmp(Zones, _null_array, sizeof(Zones)) != 0
        || memcmp(Overlapper, _null_array, sizeof(Overlapper)) != 0; */
}

/**
 * @brief Load data for this class from a save file.
 *
 * 0x004F8E64
 */
BOOL CellClass::Load(Straw &straw)
{
    // TODO save/load members individually using fixed size data types for fully defined save format.
    return straw.Get(this, sizeof(*this)) == sizeof(*this);
}

/**
 * @brief Save data for this class to a save file.
 *
 * 0x004F8E88
 */
BOOL CellClass::Save(Pipe &pipe) const
{
    return pipe.Put(this, sizeof(*this)) == sizeof(*this);
}

/**
 * @brief
 *
 * 0x004A0230
 */
void CellClass::Adjust_Threat(HousesType house, int threat)
{
    int region = g_Map.Cell_Region(m_CellNumber);
    for (HousesType i = HOUSES_FIRST; i < HOUSES_COUNT; ++i) {
        if (i != house) {
            HouseClass *hptr = HouseClass::As_Pointer(i);
            if (hptr != nullptr) {
                if (!hptr->Is_Human() || !hptr->Is_Ally(house)) {
                    hptr->Adjust_Threat(region, threat);
                }
            }
        }
    }
    if (g_Debug_Threat) {
        g_Map.Flag_To_Redraw(true);
    }
}

/**
 * @brief
 *
 * 0x004A1AE4
 */
void CellClass::Shimmer()
{
    for (ObjectClass *optr = m_OccupierPtr; optr != nullptr; optr = optr->Get_Next()) {
        optr->Do_Shimmer();
    }
}

/**
 * @brief
 *
 * 0x004A013C
 */
void CellClass::Incoming(coord_t coord, BOOL a2, BOOL a3)
{
    for (ObjectClass *optr = m_OccupierPtr; optr != nullptr; optr = optr->Get_Next()) {
        if (a3 || g_Rule.Player_Scatter()
            || (optr->Is_Techno()
                && g_Rule.IQ_Controls().m_Scatter
                    <= reinterpret_cast<TechnoClass *>(optr)->Get_Owner_House()->Get_Current_IQ())) {
            optr->Scatter(coord, a2, a3);
        }
    }
}

/**
 * @brief
 *
 * @address 0x004F8EAC
 */
void CellClass::Code_Pointers()
{
    if (m_OccupierPtr != nullptr) {
        m_OccupierPtr = reinterpret_cast<ObjectClass *>(m_OccupierPtr->As_Target());
    }
    for (int i = 0; i < OVERLAPPER_COUNT; ++i) {
        if (m_Overlapper[i] != nullptr && m_Overlapper[i]->Is_Active()) {
            m_Overlapper[i] = reinterpret_cast<ObjectClass *>(m_Overlapper[i]->As_Target());
        } else {
            m_Overlapper[i] = nullptr;
        }
    }
}

/**
 * @brief
 *
 * @address 0x004F8F2C
 */
void CellClass::Decode_Pointers()
{
    if (m_OccupierPtr != nullptr) {
        m_OccupierPtr = As_Object((uintptr_t)m_OccupierPtr);
        captainslog_assert(m_OccupierPtr != nullptr);
    }
    for (int i = 0; i < OVERLAPPER_COUNT; ++i) {
        if (m_Overlapper[i] != nullptr) {
            m_Overlapper[i] = As_Object((uintptr_t)m_Overlapper[i]);
            captainslog_assert(m_Overlapper[i] != nullptr);
        }
    }
}

/**
 *
 *
 */
BOOL CellClass::Flag_Place(HousesType house)
{
    if (!m_HasFlag && Is_Clear_To_Move(SPEED_TRACK)) {
        m_OwnerHouse = house;
        m_HasFlag = true;
        Redraw_Objects();
        return true;
    }

    return false;
}

/**
 *
 *
 */
BOOL CellClass::Flag_Remove()
{
    if (m_HasFlag) {
        m_HasFlag = false;
        m_OwnerHouse = HOUSES_NONE;
        Redraw_Objects();
        return true;
    }

    return false;
}
