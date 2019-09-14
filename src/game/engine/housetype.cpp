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

HouseTypeClass::HouseTypeClass(HousesType type, const char *name, int uiname, const char *suffix, int lemon_factor,
    PlayerColorType color, char prefix) :
    AbstractTypeClass(RTTI_HOUSETYPE, type, uiname, name),
    Type(type),
    LemonFactor(lemon_factor),
    Color(color),
    Prefix(prefix),
    Firepower(1, 1),
    Groundspeed(1, 1),
    Airspeed(1, 1),
    Armor(1, 1),
    ROF(1, 1),
    Cost(1, 1),
    BuildTime(1, 1)
{
    strlcpy(HouseName, suffix, 3);
}

HouseTypeClass::HouseTypeClass(HouseTypeClass const &that) :
    AbstractTypeClass(that),
    Type(that.Type),
    LemonFactor(that.LemonFactor),
    Color(that.Color),
    Prefix(that.Prefix),
    Firepower(that.Firepower),
    Groundspeed(that.Groundspeed),
    Airspeed(that.Airspeed),
    Armor(that.Armor),
    ROF(that.ROF),
    Cost(that.Cost),
    BuildTime(that.BuildTime)
{
}

void *HouseTypeClass::operator new(size_t size)
{
    return g_HouseTypes.Allocate();
}

void HouseTypeClass::operator delete(void *ptr)
{
    g_HouseTypes.Free(ptr);
}

BOOL HouseTypeClass::Read_INI(GameINIClass &ini)
{
    if (ini.Find_Section(m_Name) == nullptr) {
        return false;
    }

    Firepower = ini.Get_Fixed(m_Name, "Firepower", Firepower);
    Groundspeed = ini.Get_Fixed(m_Name, "Groundspeed", Groundspeed);
    Airspeed = ini.Get_Fixed(m_Name, "Airspeed", Airspeed);
    Armor = ini.Get_Fixed(m_Name, "Armor", Armor);
    ROF = ini.Get_Fixed(m_Name, "ROF", ROF);
    Cost = ini.Get_Fixed(m_Name, "Cost", Cost);
    BuildTime = ini.Get_Fixed(m_Name, "BuildTime", BuildTime);

    return true;
}

void HouseTypeClass::Init_Heap()
{
    // The order of allocation must follow the order of HouseType enum
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

HousesType HouseTypeClass::From_Name(const char *name)
{
    for (HousesType type = HOUSES_FIRST; type < HOUSES_COUNT; ++type) {
        if (strcasecmp(As_Reference(type).m_Name, name) == 0) {
            return type;
        }
    }

    return HOUSES_NONE;
}

HouseTypeClass &HouseTypeClass::As_Reference(HousesType type)
{
    return g_HouseTypes[type];
}

const char * HouseTypeClass::Name_From_Owner(int owner)
{
    //TODO: OmniBlade, please implement.
    return nullptr;
}

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
