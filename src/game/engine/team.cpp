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
#include "scenario.h"
#include "target.h"
#include "trigger.h"

#ifndef GAME_DLL
TFixedIHeapClass<TeamClass> g_Teams;
#endif

TeamClass::TeamClass(TeamTypeClass *teamtype, HouseClass *house) :
    AbstractClass(RTTI_TEAM, g_Teams.ID(this)),
    m_Class(teamtype),
    m_Owner(house),
    m_ForcedActive(false),
    m_HasBeen(false),
    m_FullStrength(false),
    m_Understrength(true),
    m_Reforming(false),
    m_Bit1_32(false),
    m_Bit1_64(true),
    m_Bit1_128(false),
    m_Moving(false),
    m_Bit2_2(true),
    m_Bit2_4(false),
    m_Bit2_8(false),
    m_Bit2_16(false),
    m_Bit2_32(false),
    m_Bit2_64(false),
    m_Bit2_128(false),
    m_Center(0),
    m_field_25(),
    m_field_29(),
    m_Target(),
    m_Members(0),
    m_field_35(0),
    m_Formation(FORMATION_NONE),
    m_field_3A(),
    m_Trigger(nullptr),
    m_Mission(-1),
    m_field_4B(),
    m_field_54(nullptr)
{
    if (house == nullptr) {
        m_Owner = HouseClass::As_Pointer(m_Class->Get_Owner());
    }

    memset(m_Quantity, 0, sizeof(m_Quantity));

    if (m_Class->Get_Location() != -1) {
        cell_t cellnum = g_Scen.Get_Waypoint(m_Class->Get_Location());
        m_Center = ::As_Target(cellnum);
    }

    m_Class->Add_Instance();

    if (m_Class->Get_TriggerType() != nullptr) {
        m_Trigger = new TriggerClass(m_Class->Get_TriggerType());
    }
}

TeamClass::~TeamClass()
{
    if (g_GameActive) {
        if (m_Class != nullptr) {
            for (FootClass **ptr = &m_field_54; *ptr != nullptr; Remove(*ptr));
            m_Class->Remove_Instance();

            if (m_Trigger != nullptr) {
                if (!m_Trigger->Has_Refs()) {
                    delete static_cast<TriggerClass *>(m_Trigger);
                    m_Trigger = nullptr;
                }
            }
        }

        m_field_54 = nullptr;
    }
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

void TeamClass::AI()
{
#ifdef GAME_DLL 
    DEFINE_CALL(func, 0x0055C1DC, void, TeamClass *);
    func(this);
#endif 
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
