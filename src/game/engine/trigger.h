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
#pragma once

#ifndef TRIGGER_H
#define TRIGGER_H

#include "always.h"
#include "triggertype.h"
#include "gameptr.h"
#include "ttimer.h"
#include "taction.h"
#include "tevent.h"

class ObjectClass;

class TriggerClass
{
public:
    TriggerClass(TriggerTypeClass *trigger_type);
    TriggerClass(const TriggerClass &that);
    TriggerClass(const NoInitClass &noinit);
    ~TriggerClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    int Get_Heap_ID() const { return m_HeapID; }
    BOOL Is_Active() const { return m_IsActive; }

    target_t As_Target() const;
    void Detach(target_t target, int a2);
    AttachType Attaches_To() const;
    BOOL Remove();

    BOOL Spring(TEventType tevent, ObjectClass *object = nullptr, cell_t cellnum = -1, BOOL a4 = false);
    BOOL Spring(TEventType tevent, HousesType house, target_t target = 0);
    BOOL Spring(TEventType tevent, cell_t cellnum);

    TEventType EventOne_What_Type() const { return m_Class->Get_Event_One().What_Type(); }
    TEventType EventTwo_What_Type() const { return m_Class->Get_Event_Two().What_Type(); }
    TActionType ActionOne_What_Type() const { return m_Class->Get_Action_One().What_Type(); }
    TActionType ActionTwo_What_Type() const { return m_Class->Get_Action_Two().What_Type(); }

    const TriggerTypeClass &Class_Of() const { return *m_Class; }

    void Add_Ref() { ++m_RefCount; }
    void Release_Ref() { --m_RefCount; }
    BOOL Has_Refs() const { return m_RefCount != 0; }

    void Code_Pointers() {}
    void Decode_Pointers() {}

    static void Init();

private:
    RTTIType m_RTTI;
    int m_HeapID;
    GamePtr<TriggerTypeClass> m_Class;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1; // 1
#else
    bool m_IsActive;
#endif
    TDelayEventClass m_TDEvent1; // 0x0D
    TDelayEventClass m_TDEvent2; // 0x01A
    int m_RefCount; // could be attach count, or instance count?"
    cell_t m_AttachedCell; // 0x2B // TODO: Name guessed.
};

#ifdef GAME_DLL
extern TFixedIHeapClass<TriggerClass> &g_Triggers;
#else
extern TFixedIHeapClass<TriggerClass> g_Triggers;
#endif

#endif // TRIGGER_H
