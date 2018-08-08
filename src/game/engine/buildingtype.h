/**
* @file
*
* @author CCHyper
* @author OmniBlade
*
* @brief <todo>
*
* @copyright RedAlert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef BUILDINGTYPE_H
#define BUILDINGTYPE_H

#include "always.h"
#include "technotype.h"
#include "theater.h"

enum BuildingType
{
    BUILDING_NONE = -1,

    BUILDING_FIRST = 0,

    // The follow right hand comments are the bit index (uint32) for storing
    // which buildings are set, ie, HouseClas storing the buildings the house
    // has constructed, or CCINIClass writing an entry.
    BUILDING_ATEK = 0,				// 1 - 1
    BUILDING_IRON_CURTAIN = 1,		// 1 - 2
    BUILDING_WEAP = 2,				// 1 - 4
    BUILDING_PDOX = 3,				// 1 - 8
    BUILDING_PBOX = 4,				// 1 - 16
    BUILDING_HBOX = 5,				// 1 - 32
    BUILDING_DOME = 6,				// 1 - 64
    BUILDING_GAP_GENERATOR = 7,		// 1 - 128
    BUILDING_GUN = 8,				// 2 - 1
    BUILDING_AGUN = 9,				// 2 - 2
    BUILDING_FTUR = 10,				// 2 - 4
    BUILDING_FACT = 11,				// 2 - 8
    BUILDING_PROC = 12,				// 2 - 16
    BUILDING_SILO = 13,				// 2 - 32
    BUILDING_HELIPAD = 14,		    // 2 - 64
    BUILDING_SAM = 15,				// 2 - 128
    BUILDING_AIRFIELD = 16,			// 3 - 1
    BUILDING_POWR = 17,				// 3 - 2
    BUILDING_APWR = 18,				// 3 - 4
    BUILDING_STEK = 19,				// 3 - 8
    BUILDING_HOSPITAL = 20,			// 3 - 16
    BUILDING_BARR = 21,				// 3 - 32
    BUILDING_TENT = 22,				// 3 - 64
    BUILDING_KENN = 23,				// 3 - 128
    BUILDING_FIX = 24,				// 4 - 1
    BUILDING_BIO = 25,				// 4 - 2
    BUILDING_MISS = 26,				// 4 - 4
    BUILDING_SYRD = 27,				// 4 - 8
    BUILDING_SPEN = 28,				// 4 - 16
    BUILDING_MSLO = 29,				// 4 - 32
    BUILDING_FCOM = 30,				// 4 - 64
    BUILDING_TESLA_COIL = 31,		// 4 - 128

    // Only the first 32 entries of BuildingType are valid within a 32 integer.
    BUILDING_VALID_COUNT = 32,      // TOOD: Rename this.

    // Fakes
    BUILDING_WEAF = 32,
    BUILDING_FACF = 33,
    BUILDING_SYRF = 34,
    BUILDING_SPEF = 35,
    BUILDING_DOMF = 36,

    // Walls
    BUILDING_SBAG = 37,
    BUILDING_CYCL = 38,
    BUILDING_BRIK = 39,
    BUILDING_BARB = 40,
    BUILDING_WOOD = 41,
    BUILDING_FENC = 42,

    // Mines
    BUILDING_MINV = 43,
    BUILDING_MINP = 44,

    // Villages
    BUILDING_V01 = 45,
    BUILDING_V02 = 46,
    BUILDING_V03 = 47,
    BUILDING_V04 = 48,
    BUILDING_V05 = 49,
    BUILDING_V06 = 50,
    BUILDING_V07 = 51,
    BUILDING_V08 = 52,
    BUILDING_V09 = 53,
    BUILDING_V10 = 54,
    BUILDING_V11 = 55,
    BUILDING_V12 = 56,
    BUILDING_V13 = 57,
    BUILDING_V14 = 58,
    BUILDING_V15 = 59,
    BUILDING_V16 = 60,
    BUILDING_V17 = 61,
    BUILDING_V18 = 62,
    BUILDING_V19 = 63,
    BUILDING_V20 = 64,
    BUILDING_V21 = 65,
    BUILDING_V22 = 66,
    BUILDING_V23 = 67,
    BUILDING_V24 = 68,
    BUILDING_V25 = 69,
    BUILDING_V26 = 70,
    BUILDING_V27 = 71,
    BUILDING_V28 = 72,
    BUILDING_V29 = 73,
    BUILDING_V30 = 74,
    BUILDING_V31 = 75,
    BUILDING_V32 = 76,
    BUILDING_V33 = 77,
    BUILDING_V34 = 78,
    BUILDING_V35 = 79,
    BUILDING_V36 = 80,
    BUILDING_V37 = 81,

    // Barrels
    BUILDING_BARREL = 82,
    BUILDING_BRL3 = 83,

    // Ants
    BUILDING_QUEE = 84,
    BUILDING_LAR1 = 85,
    BUILDING_LAR2 = 86,

    BUILDING_LAST = 86,

    BUILDING_COUNT = 87
};

DEFINE_ENUMERATION_OPERATORS(BuildingType);
DEFINE_ENUMERATION_BITWISE_OPERATORS(BuildingType);

class BuildingTypeClass : public TechnoTypeClass
{
public:
    static void Init(TheaterType theater);
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
//extern TFixedIHeapClass<BuildingTypeClass> &BuildingTypes;
#else
//extern TFixedIHeapClass<BuildingTypeClass> BuildingTypes;
#endif

#endif // BUILDINGTYPE_H
