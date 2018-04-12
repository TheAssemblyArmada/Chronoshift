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

#ifndef INFANTRYTYPE_H
#define INFANTRYTYPE_H

#include "always.h"
#include "technotype.h"

enum InfantryType
{
    INFANTRY_NONE				= -1,		// Invalid InfantryType

    INFANTRY_FIRST				= 0,

    INFANTRY_E1					= 0,		// Rifle Infantry
    INFANTRY_E2					= 1,		// Grenadier
    INFANTRY_E3					= 2,		// Rocket Soldier
    INFANTRY_FLAME_TROOPER		= 3,		// Flamethrower
    INFANTRY_ENGINEER			= 4,		// Engineer
    INFANTRY_TANYA			    = 5,		// Tanya
    INFANTRY_SPY				= 6,		// Spy
    INFANTRY_THIEF				= 7,		// Theif
    INFANTRY_MEDIC				= 8,		// Medic
    INFANTRY_GENERAL			= 9,		// General
    INFANTRY_DOG				= 10,		// Attack Dog
    INFANTRY_C1					= 11,		//
    INFANTRY_C2					= 12,		//
    INFANTRY_C3					= 13,		//
    INFANTRY_C4					= 14,		//
    INFANTRY_C5					= 15,		//
    INFANTRY_C6					= 16,		//
    INFANTRY_C7					= 17,		//
    INFANTRY_C8					= 18,		//
    INFANTRY_C9					= 19,		//
    INFANTRY_C10				= 20,		//
    INFANTRY_EINSTEIN			= 21,		// Einstein
    INFANTRY_DELPHI				= 22,		// Delphi
    INFANTRY_CHAN				= 23,		//
    INFANTRY_SHOCK_TROOPER		= 24,		// Shock Trooper
    INFANTRY_MECHANIC			= 25,		// Mechanic

    INFANTRY_LAST				= 25,		//

    INFANTRY_COUNT				= 26		//
};

DEFINE_ENUMERATION_OPERATORS(InfantryType);
DEFINE_ENUMERATION_BITWISE_OPERATORS(InfantryType);

enum DoType
{
    DO_NONE = -1,

    DO_FIRST = 0,

    DO_READY = 0,
    DO_GUARD = 1,
    DO_PRONE = 2,
    DO_WALK = 3,
    DO_FIRE_UP = 4,
    DO_DOWN = 5,
    DO_CRAWL = 6,
    DO_UP = 7,
    DO_FIRE_PRONE = 8,

    DO_IDLE1 = 9,
    DO_IDLE2 = 10,

    DO_DIE1 = 11,
    DO_DIE2 = 12,
    DO_DIE3 = 13,
    DO_DIE4 = 14,
    DO_DIE5 = 15,

    DO_16 = 16,
    DO_17 = 17,
    DO_18 = 18,
    DO_19 = 19,
    DO_20 = 20,

    DO_LAST = 20,

    DO_COUNT = 21
};

DEFINE_ENUMERATION_OPERATORS(DoType);

#ifndef RAPP_STANDALONE
#include "hooker.h"
#else
#endif

#endif // INFANTRYTYPE_H
