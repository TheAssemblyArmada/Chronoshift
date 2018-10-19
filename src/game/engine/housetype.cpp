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
#include "ccini.h"
#include "language.h"
#include "stringex.h"

// These global objects are used to initialise the heap
HouseTypeClass const HouseEngland(HOUSES_ENGLAND, "England", TXT_ENGLAND, "ENG", 0, PLAYER_COLOR_GREEN, 'E');
HouseTypeClass const HouseGermany(HOUSES_GERMANY, "Germany", TXT_GERMANY, "GER", 0, PLAYER_COLOR_GREY, 'G');
HouseTypeClass const HouseFrance(HOUSES_FRANCE, "France", TXT_FRANCE, "FRA", 0, PLAYER_COLOR_BLUE, 'F');
HouseTypeClass const HouseUkraine(HOUSES_UKRAINE, "Ukraine", TXT_UKRAINE, "UKA", 0, PLAYER_COLOR_ORANGE, 'K');
HouseTypeClass const HouseUSSR(HOUSES_USSR, "USSR", TXT_RUSSIA, "RED", 0, PLAYER_COLOR_RED, 'U');
HouseTypeClass const HouseGreece(HOUSES_GREECE, "Greece", TXT_GREECE, "GRE", 0, PLAYER_COLOR_LIGHT_BLUE, 'G');
HouseTypeClass const HouseTurkey(HOUSES_TURKEY, "Turkey", TXT_TURKEY, "TRK", 0, PLAYER_COLOR_BROWN, 'T');
HouseTypeClass const HouseSpain(HOUSES_SPAIN, "Spain", TXT_SPAIN, "SPN", 0, PLAYER_COLOR_YELLOW, 'S');
HouseTypeClass const HouseGood(HOUSES_GOODGUY, "GoodGuy", TXT_FRIENDLY, "GDI", 0, PLAYER_COLOR_LIGHT_BLUE, 'G');
HouseTypeClass const HouseBad(HOUSES_BADGUY, "BadGuy", TXT_ENEMY, "NOD", 0, PLAYER_COLOR_RED, 'B');
HouseTypeClass const HouseCivilian(HOUSES_NEUTRAL, "Neutral", TXT_CIVILIAN, "CIV", 0, PLAYER_COLOR_YELLOW, 'C');
HouseTypeClass const HouseJP(HOUSES_SPECIAL, "Special", TXT_JP, "JP", 0, PLAYER_COLOR_YELLOW, 'J');
HouseTypeClass const HouseMulti1(HOUSES_MULTI_1, "Multi1", TXT_CIVILIAN, "MP1", 0, PLAYER_COLOR_YELLOW, 'M');
HouseTypeClass const HouseMulti2(HOUSES_MULTI_2, "Multi2", TXT_CIVILIAN, "MP2", 0, PLAYER_COLOR_LIGHT_BLUE, 'M');
HouseTypeClass const HouseMulti3(HOUSES_MULTI_3, "Multi3", TXT_CIVILIAN, "MP3", 0, PLAYER_COLOR_RED, 'M');
HouseTypeClass const HouseMulti4(HOUSES_MULTI_4, "Multi4", TXT_CIVILIAN, "MP4", 0, PLAYER_COLOR_GREEN, 'M');
HouseTypeClass const HouseMulti5(HOUSES_MULTI_5, "Multi5", TXT_CIVILIAN, "MP5", 0, PLAYER_COLOR_ORANGE, 'M');
HouseTypeClass const HouseMulti6(HOUSES_MULTI_6, "Multi6", TXT_CIVILIAN, "MP6", 0, PLAYER_COLOR_GREY, 'M');
HouseTypeClass const HouseMulti7(HOUSES_MULTI_7, "Multi7", TXT_CIVILIAN, "MP7", 0, PLAYER_COLOR_BLUE, 'M');
HouseTypeClass const HouseMulti8(HOUSES_MULTI_8, "Multi8", TXT_CIVILIAN, "MP8", 0, PLAYER_COLOR_BROWN, 'M');

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<HouseTypeClass> &HouseTypes = Make_Global<TFixedIHeapClass<HouseTypeClass> >(0x0065DD24);
#else
TFixedIHeapClass<HouseTypeClass> HouseTypes;
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

BOOL HouseTypeClass::Read_INI(CCINIClass &ini)
{
    if (ini.Find_Section(Name) == nullptr) {
        return false;
    }

    Firepower = ini.Get_Fixed(Name, "Firepower", Firepower);
    Groundspeed = ini.Get_Fixed(Name, "Groundspeed", Groundspeed);
    Airspeed = ini.Get_Fixed(Name, "Airspeed", Airspeed);
    Armor = ini.Get_Fixed(Name, "Armor", Armor);
    ROF = ini.Get_Fixed(Name, "ROF", ROF);
    Cost = ini.Get_Fixed(Name, "Cost", Cost);
    BuildTime = ini.Get_Fixed(Name, "BuildTime", BuildTime);

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
        return As_Reference(type).Name;
    }

    return "None";
}

HousesType HouseTypeClass::From_Name(const char *name)
{
    for (HousesType type = HOUSES_FIRST; type < HOUSES_COUNT; ++type) {
        if (strcasecmp(As_Reference(type).Name, name) == 0) {
            return type;
        }
    }

    return HOUSES_NONE;
}

HouseTypeClass &HouseTypeClass::As_Reference(HousesType type)
{
    return HouseTypes[type];
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

void *HouseTypeClass::operator new(size_t size)
{
    return HouseTypes.Allocate();
}

void HouseTypeClass::operator delete(void *ptr)
{
    HouseTypes.Free(ptr);
}
