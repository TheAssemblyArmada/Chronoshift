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

#ifndef AIRCRAFTTYPE_H
#define AIRCRAFTTYPE_H

#include "always.h"
#include "technotype.h"

enum AircraftType
{
    AIRCRAFT_NONE = -1,
    AIRCRAFT_FIRST = 0,
    AIRCRAFT_TRANSPORT = 0,
    AIRCRAFT_BADGER = 1,
    AIRCRAFT_U2 = 2,
    AIRCRAFT_MIG = 3,
    AIRCRAFT_YAK = 4,
    AIRCRAFT_HELI = 5,
    AIRCRAFT_HIND = 6,
    AIRCRAFT_LAST = 6,
    AIRCRAFT_COUNT = 7
};

DEFINE_ENUMERATION_OPERATORS(AircraftType);
DEFINE_ENUMERATION_BITWISE_OPERATORS(AircraftType);

class AircraftTypeClass : public TechnoTypeClass
{
};

#ifndef RAPP_STANDALONE
#include "hooker.h"
//extern TFixedIHeapClass<AircraftTypeClass> &AircraftTypes;
#else
//extern TFixedIHeapClass<AircraftTypeClass> AircraftTypes;
#endif

#endif // AIRCRAFTTYPE_H
