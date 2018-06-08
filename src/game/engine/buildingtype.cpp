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
#include "buildingtype.h"

#ifndef RAPP_STANDALONE
//TFixedIHeapClass<BuildingTypeClass> &BuildingTypes = *reinterpret_cast<TFixedIHeapClass<BuildingTypeClass> *>(0x);
#else
//TFixedIHeapClass<BuildingTypeClass> BuildingTypes;
#endif

void BuildingTypeClass::Init(TheaterType theater)
{
    // TODO Doable, just wrapped to complete IOMap stack.
#ifndef RAPP_STANDALONE
    void (*func)(TheaterType) = reinterpret_cast<void (*)(TheaterType)>(0x004538F4);
    func(theater);
#endif
}
