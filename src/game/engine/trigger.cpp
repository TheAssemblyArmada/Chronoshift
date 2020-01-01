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
#include "trigger.h"
#include "target.h"
#include "iomap.h"
#include "cell.h"
#include "unit.h"
#include "building.h"
#include "infantry.h"
#include "aircraft.h"
#include "vessel.h"
#include "logic.h"

#ifndef GAME_DLL
TFixedIHeapClass<TriggerClass> g_Triggers;
#endif

TriggerClass::TriggerClass(TriggerTypeClass *trigger_type) :
    m_RTTI(RTTI_TRIGGER),
    m_HeapID(g_Triggers.ID(this)),
    m_Class(trigger_type),
    m_IsActive(false),
    m_TDEvent1(),
    m_TDEvent2(),
    m_RefCount(0),
    m_AttachedCell(0)
{
    //m_Class->Get_Event_One().Reset();
    //m_Class->Get_Event_Two().Reset();
}

TriggerClass::TriggerClass(const TriggerClass &that) :
    m_RTTI(that.m_RTTI),
    m_HeapID(that.m_HeapID),
    m_Class(that.m_Class),
    m_IsActive(that.m_IsActive),
    m_TDEvent1(that.m_TDEvent1),
    m_TDEvent2(that.m_TDEvent2),
    m_RefCount(that.m_RefCount),
    m_AttachedCell(that.m_AttachedCell)
{
}

TriggerClass::TriggerClass(const NoInitClass &noinit) :
    m_Class(noinit),
    m_TDEvent1(noinit),
    m_TDEvent2(noinit)
{
}

TriggerClass::~TriggerClass()
{
}

void *TriggerClass::operator new(size_t size)
{
    TriggerClass *this_ptr = g_Triggers.Alloc();
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void TriggerClass::operator delete(void *ptr)
{
    TriggerClass *this_ptr = static_cast<TriggerClass *>(ptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Triggers.Free(this_ptr);
}

target_t TriggerClass::As_Target() const
{
    return Make_Target(m_RTTI, m_HeapID);
}

void TriggerClass::Detach(target_t target, BOOL a2)
{
}

AttachType TriggerClass::Attaches_To() const
{
    // TODO: Requires TEventClass.
    //return TEventClass::Attaches_To();
    return ATTACH_NONE;
}

BOOL TriggerClass::Spring(TEventType tevent, ObjectClass *object, cell_t cellnum, BOOL a4)
{
#ifdef GAME_DLL
    BOOL (*func)(TriggerClass *, ObjectClass *, cell_t, BOOL) = reinterpret_cast<BOOL (*)(TriggerClass *, ObjectClass *, cell_t, BOOL)>(0x0056CC54);
    return func(this, object, cellnum, a4);
#else
    return false;
#endif
}

BOOL TriggerClass::Spring(TEventType tevent, HousesType house, target_t target)
{
    return false;
}

BOOL TriggerClass::Spring(TEventType tevent, cell_t cellnum)
{
    return false;
}

BOOL TriggerClass::Remove()
{
    /*for (int index = 0; index < MAP_MAX_AREA; ++index) {
        CellClass &cell = Map[index];
        if (cell.Get_Bit16()) {
            if (this == cell.Get_Trigger()) {
                cell.Set_Bit16(false);
                cell.CellTag = nullptr;
            }
        }
    }*/

    /*for (int index = 0; index < g_Units.Count(); ++index) {
        UnitClass *uptr = g_Units.Ptr(index);
        captainslog_assert(uptr != nullptr);
        if (this == uptr->Get_Attached_Trigger()) {
            uptr->Invalidate_Attached_Trigger();
        }
    }*/

    /*for (int index = 0; index < g_Vessels.Count(); ++index) {
        VesselClass *vptr = g_Vessels.Ptr(index);
        captainslog_assert(vptr != nullptr);
        if (this == vptr->Get_Attached_Trigger()) {
            vptr->Invalidate_Attached_Trigger();
        }
    }*/

    /*for (int index = 0; index < g_Infantry.Count(); ++index) {
        InfantryClass *iptr = g_Infantry.Ptr(index);
        captainslog_assert(iptr != nullptr);
        if (this == iptr->Get_Attached_Trigger()) {
            iptr->Invalidate_Attached_Trigger();
        }
    }*/

    /*for (int index = 0; index < g_Aircraft.Count(); ++index) {
        AircraftClass *aptr = g_Aircraft.Ptr(index);
        captainslog_assert(aptr != nullptr);
        if (this == aptr->Get_Attached_Trigger()) {
            aptr->Invalidate_Attached_Trigger();
        }
    }*/

    /*for (int index = 0; index < g_Buildings.Count(); ++index) {
        BuildingClass *bptr = g_Buildings.Ptr(index);
        captainslog_assert(bptr != nullptr);
        if (this == bptr->Get_Attached_Trigger()) {
            bptr->Invalidate_Attached_Trigger();
        }
    }*/

    HousesType house = m_Class->Get_House();
    for (int index = 0; index < g_HouseTriggers[house].Count(); ++index) {
        TriggerClass *tptr = g_HouseTriggers[house][index];
        captainslog_assert(tptr != nullptr);
        if (tptr != nullptr) {
            g_HouseTriggers[index].Delete(this);
        }
    }

    // TODO: Might have to check LogicTriggers and MapTriggers?

    // Now we have removed all linked instances, delete ourself.
    delete this;

    return true;
}

void TriggerClass::Init()
{
    g_Triggers.Free_All();
}
