/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief 
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "aircraft.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<AircraftClass> &g_Aircraft = Make_Global<TFixedIHeapClass<AircraftClass> >(0x0065D818);
#else
TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

AircraftClass::AircraftClass(RTTIType type, int id, HousesType house) :
    FootClass(type, id, house)
{
}

AircraftClass::AircraftClass(const AircraftClass &that) :
    FootClass(that)
{
}

AircraftClass::AircraftClass(const NoInitClass &noinit) :
    FootClass(noinit)
{
}

AircraftClass::~AircraftClass()
{
}
