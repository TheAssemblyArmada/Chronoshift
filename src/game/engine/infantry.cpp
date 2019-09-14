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
#include "infantry.h"

#ifndef GAME_DLL
TFixedIHeapClass<InfantryClass> g_Infantry;
#endif

InfantryClass::InfantryClass(RTTIType type, int id, HousesType house) :
    FootClass(type, id, house)
{
}

InfantryClass::InfantryClass(const InfantryClass &that) :
    FootClass(that)
{
}

InfantryClass::InfantryClass(const NoInitClass &noinit) :
    FootClass(noinit)
{
}

InfantryClass::~InfantryClass()
{
}
