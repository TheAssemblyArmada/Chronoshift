/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on building objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "buildingtype.h"
#include "aircrafttype.h"
#include "coord.h"
#include "gamedebug.h"
#include "globals.h"
#include "lists.h"
#include "rules.h"
#include "special.h"
#include "unittype.h"
#include "gamefile.h"
#include <algorithm>

#ifndef GAME_DLL
TFixedIHeapClass<BuildingTypeClass> g_BuildingTypes;
void *BuildingTypeClass::g_WarFactoryOverlay = nullptr;
void *BuildingTypeClass::g_LightningShapes = nullptr;
#endif

/**
 * 0x00429CEC
 */
BuildingTypeClass::BuildingTypeClass(BuildingType type, int uiname, const char *name, FacingType facing, coord_t exit_coord,
    RemapType altremap, int primaryf, int primaryl, int primaryh, BOOL fake, BOOL normalized, BOOL nominal, BOOL wall, BOOL simple_damage,
    BOOL radar_invisible, BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL theater, BOOL turret, BOOL remapable,
    RTTIType factory, DirType dir, BSizeType size, const int16_t *exit_list, const int16_t *occupy_list,
    const int16_t *overlap_list) :
    TechnoTypeClass(RTTI_BUILDINGTYPE, type, uiname, name, altremap, primaryf, primaryl, primaryh, primaryl, primaryh,
        nominal, radar_invisible, selectable, legal_target, insignificant, 0, theater, turret, remapable, 1,
        turret ? FACING_COUNT_32 : FACING_COUNT_1, SPEED_NONE),
    m_BaseNormal(true),
    m_Fake(fake),
    m_Bib(false),
    m_Wall(wall),
    m_SimpleDamage(simple_damage),
    m_Capturable(false),
    m_Normalized(normalized),
    m_Powered(false),
    m_Unsellable(false),
    m_Facing(facing),
    m_Adjacency(1),
    m_FactoryType(factory),
    m_ExitCoord(exit_coord),
    m_ExitList(exit_list),
    m_Type(type),
    m_StartFacing(dir),
    m_Storage(0),
    m_Power(0),
    m_Drain(0),
    m_BuildingSize(size),
    m_OccupyList(occupy_list),
    m_OverlapList(overlap_list),
    m_BuildupData(nullptr)
{ 
    for (BStateType i = BSTATE_FIRST; i < BSTATE_COUNT; ++i) {
        m_Anims[i].m_Start = 0;
        m_Anims[i].m_FrameCount = 1;
        m_Anims[i].m_Rate = 0;
    }
}

/**
 * 0x00454070
 */
BuildingTypeClass::BuildingTypeClass(const BuildingTypeClass &that) :
    TechnoTypeClass(that),
    m_BaseNormal(that.m_BaseNormal),
    m_Fake(that.m_Fake),
    m_Bib(that.m_Bib),
    m_Wall(that.m_Wall),
    m_SimpleDamage(that.m_SimpleDamage),
    m_Capturable(that.m_Capturable),
    m_Normalized(that.m_Normalized),
    m_Powered(that.m_Powered),
    m_Unsellable(that.m_Unsellable),
    m_Facing(that.m_Facing),
    m_Adjacency(that.m_Adjacency),
    m_FactoryType(that.m_FactoryType),
    m_ExitCoord(that.m_ExitCoord),
    m_ExitList(that.m_ExitList),
    m_Type(that.m_Type),
    m_StartFacing(that.m_StartFacing),
    m_Storage(that.m_Storage),
    m_Power(that.m_Power),
    m_Drain(that.m_Drain),
    m_BuildingSize(that.m_BuildingSize),
    m_OccupyList(that.m_OccupyList),
    m_OverlapList(that.m_OverlapList),
    m_BuildupData(that.m_BuildupData)
{
    memcpy(m_Anims, that.m_Anims, sizeof(m_Anims));
}

/**
 * 0x00429F0C
 */
void *BuildingTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(BuildingTypeClass) && size == g_BuildingTypes.Heap_Size());
    return g_BuildingTypes.Allocate();
}

/**
 * 0x00429F20
 */
void BuildingTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_BuildingTypes.Free(ptr);
}

/**
 * Fixes up a coord.
 *
 * 0x00454004
 */
coord_t BuildingTypeClass::Coord_Fixup(coord_t coord) const
{
    return Coord_Top_Left(coord);
}

/**
 * Fetches the name of the object for display purposes.
 *
 * 0x0045401C
 */
int BuildingTypeClass::Full_Name() const
{
    if (g_inMapEditor || !IsNominal || Rule.Named_Civilians() || m_Type < BUILDING_V01 || m_Type > BUILDING_V37) {
        return AbstractTypeClass::Full_Name();
    }

    return TXT_CIVILIAN_BUILDING;
}

/**
 * Calculate the maximum number of pips at could be displayed over the object.
 *
 * 0x00453C24
 */
int BuildingTypeClass::Max_Pips() const
{
    return std::clamp((m_Storage / 100), 0, (24 * Width() / 4));
}

/**
 * Fetches the width and height of the object.
 *
 * 0x00453A18
 */
void BuildingTypeClass::Dimensions(int &w, int &h) const
{
    w = CELL_PIXELS * Width();
    w -= (CELL_PIXELS * Width() / 5);
    h = CELL_PIXELS * Height();
    h -= (CELL_PIXELS * Height() / 5);
}

/**
 * Creates a BuildingClass instance for the specified house and places it at the specified cell.
 *
 * 0x00453804
 */
BOOL BuildingTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
#ifdef GAME_DLL
    BOOL(*func)
    (const BuildingTypeClass *, cell_t, HousesType) =
        reinterpret_cast<BOOL (*)(const BuildingTypeClass *, cell_t, HousesType)>(0x00453804);
    return func(this, cellnum, house);
#else
    /*BuildingClass *bptr = new BuildingClass(Type, house);
    DEBUG_ASSERT(bptr != nullptr);
    if (bptr != nullptr) {
        return bptr->Unlimbo(Cell_To_Coord(cellnum));
    }*/

    return false;
#endif
}

/**
 * Fetches the cost of the object.
 *
 * 0x00453CF4
 */
int BuildingTypeClass::Cost_Of() const
{
    if (Rule.Separate_Aircraft() && m_Type == BUILDING_HELIPAD) {
        return Raw_Cost();
    }

    return TechnoTypeClass::Cost_Of();
}

/**
 * Creates a BuildingClass for the specified house.
 *
 * 0x00453868
 */
ObjectClass *BuildingTypeClass::Create_One_Of(HouseClass *house) const
{
#ifdef GAME_DLL
    ObjectClass *(*func)(const BuildingTypeClass *, HouseClass *) =
        reinterpret_cast<ObjectClass *(*)(const BuildingTypeClass *, HouseClass *)>(0x00453868);
    return func(this, house);
#else
    /*DEBUG_ASSERT(house != nullptr);
    return new BuildingClass(Type, house->What_Type());*/

    return nullptr;
#endif
}

/**
 * Returns a list of relative cell offsets this object occupies.
 *
 * 0x00453A84
 */
const int16_t *BuildingTypeClass::Occupy_List(BOOL recalc) const
{
    static const int16_t _templap[] = { LIST_END };
    static int16_t _occupy[32] = {0};

    SmudgeType bib_smudge = SMUDGE_NONE;
    cell_t bibcell = 0;

    if (recalc && Bib_And_Offset(bib_smudge, bibcell)) {
        if (bib_smudge != SMUDGE_NONE && bib_smudge < SMUDGE_COUNT) {
            int16_t *dst_list = _occupy;

            const int16_t *bib_list = SmudgeTypeClass::As_Pointer(bib_smudge)->Occupy_List(false);
            while (bib_list != nullptr && *bib_list != LIST_END) {
                *dst_list++ = bibcell + *bib_list;
                ++bib_list;
            }

            const int16_t *building_list = m_OccupyList;
            while (building_list != nullptr && *building_list != LIST_END) {
                *dst_list++ = *building_list;
                ++building_list;
            }

            *dst_list = LIST_END;

            // Return the occupy list for this BuildingType, including bib calculations.
            return _occupy;
        }
    }

    if (m_OccupyList == nullptr) {
        return _templap;
    }

    return m_OccupyList;
}

/**
 * Returns a list of relative cell offsets this object overlaps.
 *
 * 0x00453B4C
 */
const int16_t *BuildingTypeClass::Overlap_List() const
{
    static const int16_t _templap[] = { LIST_END };

    if (m_OverlapList == nullptr) {
        return _templap;
    }

    return m_OverlapList;
}

/**
 * Fetches the raw cost of the object, minus the cost of any other objects included in the build.
 * Specifically this affects refineries and helipads when their purchase includes a unit as well.
 *
 * 0x00453C70
 */
int BuildingTypeClass::Raw_Cost() const
{
#ifdef GAME_DLL
    int (*func)(const BuildingTypeClass *) = reinterpret_cast<int (*)(const BuildingTypeClass *)>(0x00453C70);
    return func(this);
#elif 0
    int cost = TechnoTypeClass::Raw_Cost();

    switch (m_Type) {
        case BUILDING_PROC:
            cost -= UnitTypeClass::As_Reference(UNIT_HARVESTER).Get_Cost();
            break;

        case BUILDING_HELIPAD:
            if (Rule.Separate_Aircraft()) {
                cost -= (AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Get_Cost()
                        + AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Get_Cost()) / 2;
            }

            break;

        default:
            break;
    };

    return cost;
#else 
    return 0;
#endif
}

/**
 * Reads an object of this type from an ini file.
 *
 * 0x00453E48
 */
BOOL BuildingTypeClass::Read_INI(GameINIClass &ini)
{
#ifdef GAME_DLL
    BOOL (*func)
    (const BuildingTypeClass *, GameINIClass &) =
        reinterpret_cast<BOOL (*)(const BuildingTypeClass *, GameINIClass &)>(0x00453E48);
    return func(this, ini);
#else
    return false;
#endif
}

/**
 * Returns the buildings width in cells.
 *
 * 0x00453B6C
 */
int BuildingTypeClass::Width() const
{
    static int _width[BSIZE_COUNT] = {
        1, // BSIZE_1BY1
        2, // BSIZE_2BY1
        1, // BSIZE_1BY2
        2, // BSIZE_2BY2
        2, // BSIZE_2BY3
        3, // BSIZE_3BY2
        3, // BSIZE_3BY3
        4, // BSIZE_4BY2
        5 // BSIZE_5BY5
    };

    // BuildingSize should never be greater than the size of the array.
    DEBUG_ASSERT(m_BuildingSize < BSIZE_COUNT);

    return _width[m_BuildingSize];
}

/**
 * Returns the buildings height in cells, can optionally request height including bib.
 *
 * 0x00453B84
 */
int BuildingTypeClass::Height(BOOL check_bib) const
{
    static int _height[BSIZE_COUNT] = {
        1, // BSIZE_1BY1
        1, // BSIZE_2BY1
        2, // BSIZE_1BY2
        2, // BSIZE_2BY2
        3, // BSIZE_2BY3
        2, // BSIZE_3BY2
        3, // BSIZE_3BY3
        2, // BSIZE_4BY2
        5 // BSIZE_5BY5
    };

    // BuildingSize should never be greater than the size of the array.
    DEBUG_ASSERT(m_BuildingSize < BSIZE_COUNT);

    // Adds 1 to the height if including bib cells.
    return _height[m_BuildingSize] + (check_bib && m_Bib);
}

/**
 * Fetches the smudge type for the bib and its offset on the building.
 *
 * 0x00453BB4
 */
BOOL BuildingTypeClass::Bib_And_Offset(SmudgeType &smudge, cell_t &cellnum) const
{
    smudge = SMUDGE_NONE;

    if (m_Bib) {
        int width = Width();

        switch (width) {
            case 2:
                smudge = SMUDGE_BIB3;
                break;

            case 3:
                smudge = SMUDGE_BIB2;
                break;

            case 4:
                smudge = SMUDGE_BIB1;
                break;

            default:
                break;
        };

        if (smudge != SMUDGE_NONE) {
            int offset = ((Height() - 1) * CELL_MAX);
            cellnum += offset;
        }
    }

    return (smudge != SMUDGE_NONE && smudge < SMUDGE_COUNT);
}

/**
 * Initialises theater specific data on theater change.
 *
 * 0x004538F4
 */
void BuildingTypeClass::Init(TheaterType theater)
{
    // TODO Doable, just wrapped to complete IOMap stack.
#ifdef GAME_DLL
    void (*func)(TheaterType) = reinterpret_cast<void (*)(TheaterType)>(0x004538F4);
    func(theater);
#endif
}

/**
* Initialises animation frame info.
*
* Inlined
*/
void BuildingTypeClass::Init_Anim(BStateType bstate, int start_frame, int frame_count, int delay)
{
    m_Anims[bstate].m_Start = start_frame;
    m_Anims[bstate].m_FrameCount = frame_count;
    m_Anims[bstate].m_Rate = delay;
}

void BuildingTypeClass::One_Time()
{
    struct BAnimStruct {
        BuildingType m_Building;
        BStateType m_State;
        int m_StartFrame;
        int m_FrameCount;
        int m_Delay;			// TODO: Needs confirming what this actually is, could be Rate or Speed?
    };

    static BAnimStruct _anims[] = {

        // paradox device (chronosphere)
        { BUILDING_PDOX, BSTATE_1, 0, 4, 6 },
        { BUILDING_PDOX, BSTATE_2, 4, 16, 3 },

        // missile silo
        { BUILDING_MSLO, BSTATE_1, 0, 0, 0 },	//idle?
        { BUILDING_MSLO, BSTATE_2, 0, 5, 2 },	//attacking?
        { BUILDING_MSLO, BSTATE_4, 4, 1, 0 },	//closing?
        { BUILDING_MSLO, BSTATE_5, 5, 3, 2 },

        // camouflaged pill box
        { BUILDING_HBOX, BSTATE_2, 0, 2, 1 },

        // gap generator
        { BUILDING_GAP_GENERATOR, BSTATE_1, 0, 32, 3 },

        // soviet airfield
        { BUILDING_AIRFIELD, BSTATE_1, 0, 0, 0 },
        { BUILDING_AIRFIELD, BSTATE_4, 0, 8, 3 },

        // soviet barracks
        { BUILDING_BARR, BSTATE_2, 0, 10, 3 },
        { BUILDING_BARR, BSTATE_1, 0, 10, 3 },

        // allied barracks
        { BUILDING_TENT, BSTATE_2, 0, 10, 3 },
        { BUILDING_TENT, BSTATE_1, 0, 10, 3 },

        // queen ant
        { BUILDING_QUEE, BSTATE_1, 0, 10, 3 },

        // construction yard
        { BUILDING_FACT, BSTATE_2, 0, 26, 3 },

        // fake construction yard
        { BUILDING_FACF, BSTATE_2, 0, 26, 3 },

        // helipad
        { BUILDING_HELIPAD, BSTATE_2, 0, 7, 4 }, //Active?
        { BUILDING_HELIPAD, BSTATE_1, 0, 0, 0 }, //Idle?

        // civillian hosptial
        { BUILDING_HOSPITAL, BSTATE_1, 0, 4, 3 }, //Idle?

        // 
        { BUILDING_V19, BSTATE_1, 0, 14, 4 }, //Idle?

        // service depot
        { BUILDING_FIX, BSTATE_2, 0, 7, 2 }, //Active?
        { BUILDING_FIX, BSTATE_1, 0, 1, 0 }, //Idle?

        // 
        { BUILDING_V20, BSTATE_1, 0, 3, 3 }, //Idle?
        { BUILDING_V21, BSTATE_1, 0, 3, 3 }, //Idle?
        { BUILDING_V22, BSTATE_1, 0, 3, 3 }, //Idle?
        { BUILDING_V23, BSTATE_1, 0, 3, 3 }, //Idle?

        // weapons factory
        { BUILDING_WEAP, BSTATE_2, 0, 1, 0 }, //Active?
        { BUILDING_WEAP, BSTATE_1, 0, 1, 0 }, //Idle?

        // fake weapons factory
        { BUILDING_WEAF, BSTATE_2, 0, 1, 0 },
        { BUILDING_WEAF, BSTATE_1, 0, 1, 0 },

        // iron curtain
        { BUILDING_IRON_CURTAIN, BSTATE_2, 0, 11, 3 },

        // tesla coil
        { BUILDING_TESLA_COIL, BSTATE_2, 0, 10, 2 }

    };

    char filename[512];

    for (BuildingType i = BUILDING_FIRST; i < BUILDING_COUNT; ++i) {
        BuildingTypeClass &building = As_Reference(i);

        char const *name = (building.ImageName[0] != '\0' ? building.ImageName : building.Get_Name());

        if (building.TechLevel != -1) {
            sprintf(filename, "%.4sicon.shp", name);
            building.CameoData = GameFileClass::Retrieve(filename);
        }

        sprintf(filename, "%s.shp", name);
        building.ImageData = GameFileClass::Retrieve(filename);

        sprintf(filename, "%.4smake.shp", name);
        building.m_BuildupData = GameFileClass::Retrieve(filename);

        if (building.m_BuildupData != nullptr) {
            int frame_count = Get_Build_Frame_Count(building.m_BuildupData);
            int delay = (frame_count > 0 ? ((Rule.Buildup_Time() * 900) / frame_count) : 1);
            building.Init_Anim(BSTATE_0, 0, frame_count, delay);
        }
    }

    if (g_WarFactoryOverlay == nullptr) {
        g_WarFactoryOverlay = GameFileClass::Retrieve("weap2.shp");
        DEBUG_ASSERT(g_WarFactoryOverlay != nullptr);
    }

    // TODO: Should be moved to TechnoTypeClass.
    if (g_LightningShapes == nullptr) {
        g_LightningShapes = GameFileClass::Retrieve("litning.shp");
        DEBUG_ASSERT(g_LightningShapes != nullptr);
    }

    for (int index = 0; index < ARRAY_SIZE(_anims); ++index) {
        BAnimStruct &bstate = _anims[index];
        As_Reference(bstate.m_Building).Init_Anim(bstate.m_State, bstate.m_StartFrame, bstate.m_FrameCount, bstate.m_Delay);
    }
}

/**
 * Fetches a reference to the actual object from a type enum value.
 *
 * 0x00453A6C
 * @warning Heap allocation order MUST match the enum order in Init_Heap for this to work.
 */
BuildingTypeClass &BuildingTypeClass::As_Reference(BuildingType type)
{
    DEBUG_ASSERT(type != BUILDING_NONE);
    DEBUG_ASSERT(type < BUILDING_COUNT);

    return g_BuildingTypes[type];
}

/**
 * Fetches the type enum value from a name string.
 *
 * 0x004537B4
 */
BuildingType BuildingTypeClass::From_Name(const char *name)
{
    if (name != nullptr) {
        for (BuildingType building = BUILDING_FIRST; building < BUILDING_COUNT; ++building) {
            if (strcasecmp(name, As_Reference(building).m_Name) == 0) {
                return building;
            }
        }
    }

    return BUILDING_NONE;
}

/**
 * @brief Initialises the memory heap for BuildingTypeClass objects.
 * @warning Order of initialisation is important so enum matches position in the heap.
 *
 * @address 0x00429F38
 */
void BuildingTypeClass::Init_Heap()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x00429F38);
    func();
#else
    // The order of heap initialisation MUST match the BuildingType enum in buildingtype.h
#endif
}
