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
#include "minmax.h"
#include "rules.h"
#include "special.h"
#include "unittype.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<BuildingTypeClass> &g_BuildingTypes = Make_Global<TFixedIHeapClass<BuildingTypeClass> >(0x0065DD70);
#else
TFixedIHeapClass<BuildingTypeClass> g_BuildingTypes;
#endif

/**
 * 0x00429CEC
 */
BuildingTypeClass::BuildingTypeClass(BuildingType type, int uiname, const char *name, FacingType facing, uint32_t exit_coord,
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
    m_Anims[BSTATE_0].m_Start = 0;
    m_Anims[BSTATE_0].m_FrameCount = 1;
    m_Anims[BSTATE_0].m_Rate = 0;
    m_Anims[BSTATE_1].m_Start = 0;
    m_Anims[BSTATE_1].m_FrameCount = 1;
    m_Anims[BSTATE_1].m_Rate = 0;
    m_Anims[BSTATE_2].m_Start = 0;
    m_Anims[BSTATE_2].m_FrameCount = 1;
    m_Anims[BSTATE_2].m_Rate = 0;
    m_Anims[BSTATE_3].m_Start = 0;
    m_Anims[BSTATE_3].m_FrameCount = 1;
    m_Anims[BSTATE_3].m_Rate = 0;
    m_Anims[BSTATE_4].m_Start = 0;
    m_Anims[BSTATE_4].m_FrameCount = 1;
    m_Anims[BSTATE_4].m_Rate = 0;
    m_Anims[BSTATE_5].m_Start = 0;
    m_Anims[BSTATE_5].m_FrameCount = 1;
    m_Anims[BSTATE_5].m_Rate = 0;
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
uint32_t BuildingTypeClass::Coord_Fixup(uint32_t coord) const
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
    return Clamp((m_Storage / 100), 0, (24 * Width() / 4));
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
BOOL BuildingTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)
    (const BuildingTypeClass *, int16_t, HousesType) =
        reinterpret_cast<BOOL (*)(const BuildingTypeClass *, int16_t, HousesType)>(0x00453804);
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
#ifndef CHRONOSHIFT_STANDALONE
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
    int16_t bibcell = 0;

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
#ifndef CHRONOSHIFT_STANDALONE
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
                            + AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Get_Cost())
                    / 2;
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
#ifndef CHRONOSHIFT_STANDALONE
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
BOOL BuildingTypeClass::Bib_And_Offset(SmudgeType &smudge, int16_t &cellnum) const
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
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(TheaterType) = reinterpret_cast<void (*)(TheaterType)>(0x004538F4);
    func(theater);
#endif
}

/**
 * Fetches a reference to the actual object from a type enum value.
 *
 * 0x00453A6C
 * @warning Heap allocation order MUST match the enum order in Init_Heap for this to work.
 */
BuildingTypeClass &BuildingTypeClass::As_Reference(BuildingType building)
{
    DEBUG_ASSERT(building != BUILDING_NONE);
    DEBUG_ASSERT(building < BUILDING_COUNT);

    return g_BuildingTypes[building];
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
            if (strcasecmp(name, As_Reference(building).Name) == 0) {
                return building;
            }
        }
    }

    return BUILDING_NONE;
}
