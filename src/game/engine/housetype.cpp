/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding information on house types.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "housetype.h"
#include "housedata.h"
#include "gameini.h"
#include "language.h"
#include "stringex.h"

#ifndef GAME_DLL
TFixedIHeapClass<HouseTypeClass> g_HouseTypes;
#endif

/**
 *
 *
 * 0x004CB774
 */
HouseTypeClass::HouseTypeClass(HousesType type, const char *name, int uiname, const char *suffix, int lemon_factor,
    PlayerColorType color, char prefix) :
    AbstractTypeClass(RTTI_HOUSETYPE, type, uiname, name),
    m_Type(type),
    m_LemonFactor(lemon_factor),
    m_Color(color),
    m_Prefix(prefix),
    m_Firepower(1, 1),
    m_Groundspeed(1, 1),
    m_Airspeed(1, 1),
    m_Armor(1, 1),
    m_ROF(1, 1),
    m_Cost(1, 1),
    m_BuildTime(1, 1)
{
    strlcpy(m_HouseName, suffix, 3);
}

/**
 *
 *
 * 0x004CD380
 */
HouseTypeClass::HouseTypeClass(HouseTypeClass const &that) :
    AbstractTypeClass(that),
    m_Type(that.m_Type),
    m_LemonFactor(that.m_LemonFactor),
    m_Color(that.m_Color),
    m_Prefix(that.m_Prefix),
    m_Firepower(that.m_Firepower),
    m_Groundspeed(that.m_Groundspeed),
    m_Airspeed(that.m_Airspeed),
    m_Armor(that.m_Armor),
    m_ROF(that.m_ROF),
    m_Cost(that.m_Cost),
    m_BuildTime(that.m_BuildTime)
{
    memcpy(m_HouseName, that.m_HouseName, sizeof(m_HouseName));
}

/**
 *
 *
 * 0x004CB820
 */
void *HouseTypeClass::operator new(size_t size)
{
    return g_HouseTypes.Allocate();
}

/**
 *
 *
 * 0x004CB834
 */
void HouseTypeClass::operator delete(void *ptr)
{
    g_HouseTypes.Free(ptr);
}

/**
 *
 *
 * 0x004CD174
 */
BOOL HouseTypeClass::Read_INI(GameINIClass &ini)
{
    if (!ini.Is_Present(m_Name)) {
        return false;
    }

    m_Firepower = ini.Get_Fixed(m_Name, "Firepower", m_Firepower);
    m_Groundspeed = ini.Get_Fixed(m_Name, "Groundspeed", m_Groundspeed);
    m_Airspeed = ini.Get_Fixed(m_Name, "Airspeed", m_Airspeed);
    m_Armor = ini.Get_Fixed(m_Name, "Armor", m_Armor);
    m_ROF = ini.Get_Fixed(m_Name, "ROF", m_ROF);
    m_Cost = ini.Get_Fixed(m_Name, "Cost", m_Cost);
    m_BuildTime = ini.Get_Fixed(m_Name, "BuildTime", m_BuildTime);

    return true;
}

/**
 *
 *
 * 0x004CD154
 */
uint8_t *HouseTypeClass::Remap_Table()
{
    return g_ColorRemaps[m_Color].RemapPalette;
}

/**
 *
 *
 * 0x004CB84C
 */
void HouseTypeClass::Init_Heap()
{
    // The order of allocation must follow the order of HouseType enum.
    new HouseTypeClass(HouseSpain);
    new HouseTypeClass(HouseGreece);
    new HouseTypeClass(HouseUSSR);
    new HouseTypeClass(HouseEngland);
    new HouseTypeClass(HouseUkraine);
    new HouseTypeClass(HouseGermany);
    new HouseTypeClass(HouseFrance);
    new HouseTypeClass(HouseTurkey);

    new HouseTypeClass(HouseGood);
    new HouseTypeClass(HouseBad);

    new HouseTypeClass(HouseCivilian);
    new HouseTypeClass(HouseJP);

    new HouseTypeClass(HouseMulti1);
    new HouseTypeClass(HouseMulti2);
    new HouseTypeClass(HouseMulti3);
    new HouseTypeClass(HouseMulti4);
    new HouseTypeClass(HouseMulti5);
    new HouseTypeClass(HouseMulti6);
    new HouseTypeClass(HouseMulti7);
    new HouseTypeClass(HouseMulti8);
}
const char *HouseTypeClass::Name_From(HousesType type)
{
    if (type >= HOUSES_FIRST && type < HOUSES_COUNT) {
        return As_Reference(type).m_Name;
    }

    return "None";
}

/**
 *
 *
 * 0x004CD0E4
 */
HousesType HouseTypeClass::From_Name(const char *name)
{
    for (HousesType type = HOUSES_FIRST; type < HOUSES_COUNT; ++type) {
        if (strcasecmp(As_Reference(type).m_Name, name) == 0) {
            return type;
        }
    }

    return HOUSES_NONE;
}

/**
 *
 *
 * 0x004CD13C
 */
HouseTypeClass &HouseTypeClass::As_Reference(HousesType type)
{
    DEBUG_ASSERT(type != HOUSES_NONE);
    DEBUG_ASSERT(type < HOUSES_COUNT);

    return g_HouseTypes[type];
}

const char *HouseTypeClass::Name_From_Owner(int owner)
{
    //TODO: Inlined function in INI code that converts from OwnerType to house name(s).
    return nullptr;
}

/**
 *
 *
 * 0x004AB5FC
 */
int HouseTypeClass::Owner_From_Name(const char *name)
{
    if (strcasecmp(name, "soviet") == 0) {
        return SIDE_SOVIET;
    }

    if (strcasecmp(name, "allies") == 0 || strcasecmp(name, "allied") == 0) {
        return SIDE_ALLIES;
    }

    HousesType type = From_Name(name);
    if (type != HOUSES_NONE && type < HOUSES_COUNT) {
        return 1 << type;
    }

    return 0;
}

BOOL HouseTypeClass::Is_Multiplayer_House(HousesType type)
{
    return (type >= HOUSES_MULTI_FIRST && type <= HOUSES_MULTI_LAST);
}
