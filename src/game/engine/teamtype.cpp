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
#include "objecttype.h"
#include "target.h"
#include "triggertype.h"
#include <cstring>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifndef GAME_DLL
TFixedIHeapClass<TeamTypeClass> g_TeamTypes;
#endif

TeamTypeClass::TeamTypeClass() :
    AbstractTypeClass(RTTI_TEAMTYPE, g_TeamTypes.ID(this), 0, "")
{
}

void *TeamTypeClass::operator new(size_t size)
{
    return g_TeamTypes.Allocate();
}

void TeamTypeClass::operator delete(void *ptr)
{
    g_TeamTypes.Free(ptr);
}

/**
 * @brief
 *
 * @address 0x004F9340
 */
void TeamTypeClass::Code_Pointers()
{
    for (int i = 0; i < m_MemberCount; ++i) {
        m_Content[i].Object = reinterpret_cast<ObjectTypeClass *>(m_Content[i].Object->As_Target());
    }
}

/**
 * @brief
 *
 * @address 0x004F939C
 */
void TeamTypeClass::Decode_Pointers()
{
    for (int i = 0; i < m_MemberCount; ++i) {
        m_Content[i].Object = reinterpret_cast<ObjectTypeClass *>(As_TechnoType((uintptr_t)m_Content[i].Object));
    }
}

TeamTypeClass *TeamTypeClass::From_Name(const char *name)
{
    for (int i = 0; i < g_TeamTypes.Count(); ++i) {
        if (strcasecmp(g_TeamTypes[i].Get_Name(), name) == 0) {
            return &g_TeamTypes[i];
        }
    }

    return nullptr;
}

const char *TeamTypeClass::Name_From(TeamTypeClass *team)
{
    return team->Get_Name();
}
