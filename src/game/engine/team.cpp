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
#include "team.h"
#include "gamedebug.h"
#include "foot.h"
#include "target.h"

#ifndef GAME_DLL
TFixedIHeapClass<TeamClass> g_Teams;
#endif

TeamClass::TeamClass()
{
}

TeamClass::TeamClass(const TeamClass &that)
{
}

TeamClass::TeamClass(const NoInitClass &noinit)
{
}

TeamClass::~TeamClass()
{
}

void *TeamClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(TeamClass) && size == g_Teams.Heap_Size());
    TeamClass *this_ptr = g_Teams.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void TeamClass::operator delete(void *ptr)
{
    TeamClass *this_ptr = static_cast<TeamClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Teams.Free(this_ptr);
}

BOOL TeamClass::Remove(FootClass *object, int a2)
{
#ifdef GAME_DLL
    BOOL (*func)(TeamClass *, FootClass *, int) = reinterpret_cast<BOOL (*)(TeamClass *, FootClass *, int)>(0x004C3794);
    return func(this, object, a2);
#else
    //DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

/**
 * @brief
 *
 * @address 0x004F93DC
 */
void TeamClass::Code_Pointers()
{
    if (m_field_54 != nullptr) {
        m_field_54 = reinterpret_cast<FootClass *>(m_field_54->As_Target());
    }
}

/**
 * @brief
 *
 * @address 0x004F9420
 */
void TeamClass::Decode_Pointers()
{
    if (m_field_54 != nullptr) {
        m_field_54 = reinterpret_cast<FootClass *>(As_Techno((uintptr_t)m_field_54));
    }
}
