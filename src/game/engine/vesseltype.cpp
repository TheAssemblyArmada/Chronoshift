/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on vessel objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "vesseltype.h"
#include "vesseldata.h"
#include "gamefile.h"
#include "coord.h"
#include "facing.h"
#include "lists.h"
#include "mixfile.h"
#include "scenario.h"
#include <cstdio>

using std::snprintf;

#ifndef GAME_DLL
TFixedIHeapClass<VesselTypeClass> g_VesselTypes;
#endif

/**
 * 0x00581F0C
 */
VesselTypeClass::VesselTypeClass(VesselType type, int uiname, const char *name, AnimType death_anim, int def_fire_coord, int pri_fire_coord_a, int pri_fire_coord_b,
    int sec_fire_coord_a, int sec_fire_coord_b, BOOL a10, BOOL nominal, BOOL has_turret, int facings, MissionType mission):
    TechnoTypeClass(RTTI_VESSELTYPE, type, uiname, name, REMAP_1, def_fire_coord, pri_fire_coord_a, pri_fire_coord_b, sec_fire_coord_a, sec_fire_coord_b, nominal, false, true, true, false,
        false, false, has_turret, true, true, facings, SPEED_FLOAT),
    m_UnkBool(a10),
    m_Type(type),
    m_UnkMissionA(mission),
    m_UnkMissionB(MISSION_GUARD),
    m_ExplosionAnim(death_anim),
    m_UnkInt(0)
{
    IsScanner = true;
    IsCrewed = false;
}

/**
 * 0x00584B90
 */
VesselTypeClass::VesselTypeClass(const VesselTypeClass &that) :
    TechnoTypeClass(that),
    m_UnkBool(that.m_UnkBool),
    m_Type(that.m_Type),
    m_UnkMissionA(that.m_UnkMissionA),
    m_UnkMissionB(that.m_UnkMissionB),
    m_ExplosionAnim(that.m_ExplosionAnim),
    m_UnkInt(that.m_UnkInt)
{
}

/**
 * 0x00581FEC
 */
void *VesselTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(VesselTypeClass) && size == g_VesselTypes.Heap_Size());
    return g_VesselTypes.Allocate();
}

/**
 * 0x00582000
 */
void VesselTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_VesselTypes.Free(ptr);
}

/**
 * Calculate the maximum number of pips at could be displayed over the object.
 *
 * 0x00584B7C
 */
int VesselTypeClass::Max_Pips() const
{
    return Max_Passengers();
}

/**
 * Fetches the width and height of the object.
 *
 * 0x00584954
 */
void VesselTypeClass::Dimensions(int &w, int &h) const
{
    w = 48;
    h = 48;
}

/**
 * Creates a VesselClass instance for the specified house and places it at the specified cell.
 *
 * 0x005848C4
 */
BOOL VesselTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
#ifdef GAME_DLL
    BOOL (*func)(const VesselTypeClass *, cell_t, HousesType) =
        reinterpret_cast<BOOL (*)(const VesselTypeClass *, cell_t, HousesType)>(0x005848C4);
    return func(this, cellnum, house);
#else
    /*VesselClass *vptr = new VesselClass(m_Type, HOUSES_NONE);

    if (vptr != nullptr) {
        DirType dir = (DirType)Scen.Get_Random_Value(DIR_FIRST, DIR_LAST);
        return vptr->Unlimbo(Cell_To_Coord(cellnum), dir);
    }*/

    return false;
#endif
}

/**
 * Creates a VesselClass for the specified house.
 *
 * 0x00584870
 */
ObjectClass *VesselTypeClass::Create_One_Of(HouseClass *house) const
{
#ifdef GAME_DLL
    ObjectClass *(*func)(const VesselTypeClass *, HouseClass *) =
        reinterpret_cast<ObjectClass *(*)(const VesselTypeClass *, HouseClass *)>(0x00584870);
    return func(this, house);
#else
    /*DEBUG_ASSERT(house != nullptr);

    return new VesselClass(m_Type, house->What_Type());*/
    return nullptr;
#endif
}

/**
 * Returns a list of relative cell offsets this object overlaps.
 *
 * 0x00584B20
 */
const int16_t *VesselTypeClass::Overlap_List() const
{
    static const int16_t _list[] = { -3, -2, -1, 1, 2, 3, -128, -129, -127, -130, -126, 128, 129, 127, 130, 126, LIST_END };

    return _list;
}

/**
 * Fetches a reference to the actual object from a type enum value.
 *
 * 0x00584858
 * @warning Heap allocation order MUST match the enum order in Init_Heap for this to work.
 */
VesselTypeClass &VesselTypeClass::As_Reference(VesselType type)
{
    DEBUG_ASSERT(type != VESSEL_NONE);
    DEBUG_ASSERT(type < VESSEL_COUNT);

    return g_VesselTypes[type];
}

/**
 * Fetches the type enum value from a name string.
 *
 * 0x00584B2C
 */
VesselType VesselTypeClass::From_Name(const char *name)
{
    if (name != nullptr) {
        for (VesselType vessel = VESSEL_FIRST; vessel < VESSEL_COUNT; ++vessel) {
            if (strcasecmp(name, As_Reference(vessel).m_Name) == 0) {
                return vessel;
            }
        }
    }

    return VESSEL_NONE;
}

/**
 * Initialises data that requires a one time load.
 *
 * 0x00584968
 */
void VesselTypeClass::One_Time()
{
    char filename[512];

    for (VesselType i = VESSEL_FIRST; i < VESSEL_COUNT; ++i) {
        VesselTypeClass &vessel = As_Reference(i);
        const char *name = vessel.ImageName[0] != '\0' ? vessel.ImageName : vessel.m_Name;

        // Original has a special case where icon is always loaded for helicarrier?
        // Possibly intended for the "Secret Units" enabling code that was never in any released build.
        if (vessel.TechLevel != -1 /*|| i == VESSEL_CARRIER*/) {
            snprintf(filename, sizeof(filename), "%.4sicon.shp", name);
            vessel.CameoData = GameFileClass::Retrieve(filename);
        }

        snprintf(filename, sizeof(filename), "%s.shp", name);
        vessel.ImageData = GameFileClass::Retrieve(filename);
        vessel.m_UnkInt = 26;
    }
}

/**
 * @brief Initialises the memory heap for VesselTypeClass objects.
 * @warning Order of initialisation is important so enum matches position in the heap.
 *
 * @address 0x00582018
 */
void VesselTypeClass::Init_Heap()
{
    // The order of heap initialisation MUST match the VesselType enum in vesseltype.h
    new VesselTypeClass(VesselSubmarine);
    new VesselTypeClass(VesselDestroyer);
    new VesselTypeClass(VesselCruiser);
    new VesselTypeClass(VesselTransport);
    new VesselTypeClass(VesselPTBoat);
    new VesselTypeClass(VesselMissileSubmarine);
    new VesselTypeClass(VesselHeliCarrier);
}
