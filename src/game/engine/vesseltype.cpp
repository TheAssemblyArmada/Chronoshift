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
#include "coord.h"
#include "facing.h"
#include "lists.h"
#include "scenario.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<VesselTypeClass> &g_VesselTypes = Make_Global<TFixedIHeapClass<VesselTypeClass> >(0x0065DF38);
#else
TFixedIHeapClass<VesselTypeClass> g_VesselTypes;
#endif

VesselTypeClass::VesselTypeClass(VesselType type, int uiname, const char *name, AnimType death_anim, int a5, int a6, int a7,
    int a8, int a9, BOOL a10, BOOL nominal, BOOL has_turret, BOOL twin_turrets, int facings, MissionType mission):
    TechnoTypeClass(RTTI_VESSELTYPE, type, uiname, name, REMAP_1, a5, a6, a7, a8, a9, nominal, false, true, true, false,
        false, false, has_turret, true, true, facings, SPEED_FLOAT),
    m_Type(type),
    m_UnkBool(a10),
    m_UnkMissionA(mission),
    m_UnkMissionB(MISSION_GUARD),
    m_DeathAnim(death_anim),
    m_UnkInt(0)
{
    IsScanner = true;
    IsCrewed = false;
}

VesselTypeClass::VesselTypeClass(const VesselTypeClass &that) :
    TechnoTypeClass(that),
    m_Type(that.m_Type),
    m_UnkBool(that.m_UnkBool),
    m_UnkMissionA(that.m_UnkMissionA),
    m_UnkMissionB(that.m_UnkMissionB),
    m_DeathAnim(that.m_DeathAnim),
    m_UnkInt(that.m_UnkInt)
{
}

void *VesselTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(VesselTypeClass) && size == g_VesselTypes.Heap_Size());
    return g_VesselTypes.Allocate();
}

void VesselTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_VesselTypes.Free(ptr);
}

int VesselTypeClass::Max_Pips() const
{
    return Max_Passengers();
}

void VesselTypeClass::Dimensions(int &w, int &h) const
{
    w = 48;
    h = 48;
}

BOOL VesselTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const VesselTypeClass *, int16_t, HousesType) =
        reinterpret_cast<BOOL (*)(const VesselTypeClass *, int16_t, HousesType)>(0x005848C4);
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

ObjectClass *VesselTypeClass::Create_One_Of(HouseClass *house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    ObjectClass *(*func)(const VesselTypeClass *, HouseClass *) =
        reinterpret_cast<ObjectClass *(*)(const VesselTypeClass *, HouseClass *)>(0x00584870);
    return func(this, house);
#else
    /*DEBUG_ASSERT(house != nullptr);

    return new VesselClass(m_Type, house->What_Type());*/
    return nullptr;
#endif
}

const int16_t *VesselTypeClass::Overlap_List() const
{
    static const int16_t _list[] = { -3, -2, -1, 1, 2, 3, -128, -129, -127, -130, -126, 128, 129, 127, 130, 126, LIST_END };

    return _list;
}
