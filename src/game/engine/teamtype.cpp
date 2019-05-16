/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief TeamTypes used to hold collections of units for reinforcements and such.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "teamtype.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<TeamTypeClass> &g_TeamTypes = Make_Global<TFixedIHeapClass<TeamTypeClass> >(0x0065DB10);
#else
TFixedIHeapClass<TeamTypeClass> g_TeamTypes;
#endif

TeamTypeClass::TeamTypeClass() : AbstractTypeClass(RTTI_TEAMTYPE, g_TeamTypes.ID(this), 0, "") {}

void *TeamTypeClass::operator new(size_t size)
{
    return g_TeamTypes.Allocate();
}

void TeamTypeClass::operator delete(void *ptr)
{
    g_TeamTypes.Free(ptr);
}

void TeamTypeClass::Code_Pointers() {}

void TeamTypeClass::Decode_Pointers() {}
