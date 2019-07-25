/**
* @file
*
* @author CCHyper
* @author OmniBlade
*
* @brief Static instances of house type objects.
*
* @copyright Chronoshift is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#include "housedata.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (HouseType)
 * Name
 * UI Name
 * Suffix
 * Lemon Factor
 * Color
 * Prefix
 *
 */

const HouseTypeClass HouseEngland(HOUSES_ENGLAND, "England", TXT_ENGLAND, "ENG", 0, PLAYER_COLOR_GREEN, 'E');

const HouseTypeClass HouseGermany(HOUSES_GERMANY, "Germany", TXT_GERMANY, "GER", 0, PLAYER_COLOR_GREY, 'G');

const HouseTypeClass HouseFrance(HOUSES_FRANCE, "France", TXT_FRANCE, "FRA", 0, PLAYER_COLOR_BLUE, 'F');

const HouseTypeClass HouseUkraine(HOUSES_UKRAINE, "Ukraine", TXT_UKRAINE, "UKA", 0, PLAYER_COLOR_ORANGE, 'K');

const HouseTypeClass HouseUSSR(HOUSES_USSR, "USSR", TXT_RUSSIA, "RED", 0, PLAYER_COLOR_RED, 'U');

const HouseTypeClass HouseGreece(HOUSES_GREECE, "Greece", TXT_GREECE, "GRE", 0, PLAYER_COLOR_LIGHT_BLUE, 'G');

const HouseTypeClass HouseTurkey(HOUSES_TURKEY, "Turkey", TXT_TURKEY, "TRK", 0, PLAYER_COLOR_BROWN, 'T');

const HouseTypeClass HouseSpain(HOUSES_SPAIN, "Spain", TXT_SPAIN, "SPN", 0, PLAYER_COLOR_YELLOW, 'S');

const HouseTypeClass HouseGood(HOUSES_GOODGUY, "GoodGuy", TXT_FRIENDLY, "GDI", 0, PLAYER_COLOR_LIGHT_BLUE, 'G');

const HouseTypeClass HouseBad(HOUSES_BADGUY, "BadGuy", TXT_ENEMY, "NOD", 0, PLAYER_COLOR_RED, 'B');

const HouseTypeClass HouseCivilian(HOUSES_NEUTRAL, "Neutral", TXT_CIVILIAN, "CIV", 0, PLAYER_COLOR_YELLOW, 'C');

const HouseTypeClass HouseJP(HOUSES_SPECIAL, "Special", TXT_JP, "JP", 0, PLAYER_COLOR_YELLOW, 'J');

// Multiplayer Houses
const HouseTypeClass HouseMulti1(HOUSES_MULTI_1, "Multi1", TXT_CIVILIAN, "MP1", 0, PLAYER_COLOR_YELLOW, 'M');

const HouseTypeClass HouseMulti2(HOUSES_MULTI_2, "Multi2", TXT_CIVILIAN, "MP2", 0, PLAYER_COLOR_LIGHT_BLUE, 'M');

const HouseTypeClass HouseMulti3(HOUSES_MULTI_3, "Multi3", TXT_CIVILIAN, "MP3", 0, PLAYER_COLOR_RED, 'M');

const HouseTypeClass HouseMulti4(HOUSES_MULTI_4, "Multi4", TXT_CIVILIAN, "MP4", 0, PLAYER_COLOR_GREEN, 'M');

const HouseTypeClass HouseMulti5(HOUSES_MULTI_5, "Multi5", TXT_CIVILIAN, "MP5", 0, PLAYER_COLOR_ORANGE, 'M');

const HouseTypeClass HouseMulti6(HOUSES_MULTI_6, "Multi6", TXT_CIVILIAN, "MP6", 0, PLAYER_COLOR_GREY, 'M');

const HouseTypeClass HouseMulti7(HOUSES_MULTI_7, "Multi7", TXT_CIVILIAN, "MP7", 0, PLAYER_COLOR_BLUE, 'M');

const HouseTypeClass HouseMulti8(HOUSES_MULTI_8, "Multi8", TXT_CIVILIAN, "MP8", 0, PLAYER_COLOR_BROWN, 'M');
