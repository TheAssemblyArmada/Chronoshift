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
#include "unit.h"

#ifdef GAME_DLL
TFixedIHeapClass<UnitClass> &g_Units = Make_Global<TFixedIHeapClass<UnitClass> >(0x0065DC40);
#else
TFixedIHeapClass<UnitClass> g_Units;
#endif

UnitClass::UnitClass(RTTIType type, int id, HousesType house) :
    DriveClass(type, id, house)
{
}

UnitClass::UnitClass(const UnitClass &that) :
    DriveClass(that)
{
}

UnitClass::UnitClass(const NoInitClass &noinit) :
    DriveClass(noinit)
{
}

UnitClass::~UnitClass()
{
}
