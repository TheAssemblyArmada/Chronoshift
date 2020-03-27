/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Class holding information for scripted events in scenarios.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TRIGGERTYPE_H
#define TRIGGERTYPE_H

#include "always.h"
#include "abstracttype.h"
#include "housetype.h"
#include "taction.h"
#include "target.h"
#include "tevent.h"

class GameINIClass;
class TriggerClass;

// This enum is just for casting the TriggerType heap indexes.
enum TriggerType
{
    TRIGGER_NONE = 0xFFFFFFFF,
};

DEFINE_ENUMERATION_OPERATORS(TriggerType);

enum PersistanceType
{
    PERSIST_NONE = -1,
    PERSIST_VOLATILE, // Will only ever be activated once.
    PERSIST_SEMI_PERSISTANT, // Will only trigger once the trigger event has all conditions have been met.
    PERSIST_PERSISTANT, // Will continue to repeat itself whenever its trigger event is true.
    PERSIST_COUNT,
    PERSIST_FIRST = 0,
};

DEFINE_ENUMERATION_OPERATORS(PersistanceType);

enum EventLinkType
{
    /*
    When 0, only the first trigger event (part 5) must be true for the trigger to be activated.
    When 1, the first (part 5) and second (part 8) must both be true for the trigger to be activated.
    When 2, either the first trigger event or the second trigger event must be true, whichever comes first.
    When 3, either the first trigger event or the second trigger event must be true.
            First event triggers first action, second triggers second action.
            Ignores action link.
    */
    EVLINK_SINGLE,
    EVLINK_AND,
    EVLINK_OR,
    EVLINK_LINKED,
};

enum ActionLinkType
{
    /*
    When 0, only one trigger action is activated when the event is triggered. See the summary (next) for which trigger action
            is activated, and when.
    When 1, both trigger actions are activated when the event is triggered.
    */
    ACTLINK_SINGLE,
    ACTLINK_AND,
};

class TriggerTypeClass : public AbstractTypeClass
{
public:
    TriggerTypeClass();
    TriggerTypeClass(const NoInitClass &noinit) : AbstractTypeClass(noinit) {}
    ~TriggerTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif
    void Code_Pointers();
    void Decode_Pointers();
    TriggerClass *Find_Or_Make();
    void Detach(target_t target, int unk);
    AttachType Attaches_To();
    BOOL Edit();
    void Draw_It(int index, int x, int y, int x_max, int y_max, BOOL selected, TextPrintType style);
    const TEventClass &Get_Event_One() const { return m_EventOne; }
    const TEventClass &Get_Event_Two() const { return m_EventTwo; }
    const TActionClass &Get_Action_One() const { return m_ActionOne; }
    const TActionClass &Get_Action_Two() const { return m_ActionTwo; }
    HousesType Get_House() const { return m_House; }
    target_t As_Target() const { return Make_Target(RTTI_TRIGGERTYPE, m_HeapID); }
    
    static void Init();
    static void Read_INI(GameINIClass &ini);
    static void Write_INI(GameINIClass &ini);
    static TriggerTypeClass *From_Name(const char *name);
    static const char *Name_From(TriggerType trigger);
    static const char *Name_From(TriggerTypeClass *trigger);
    static TriggerTypeClass &As_Reference(TriggerType trigger);
    static TriggerTypeClass *As_Pointer(TriggerType trigger);

private:
    void Fill_In(const char *name, char *options);
    void Build_INI_Entry(char *buffer);
    const char *Description();

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1; // 1
#else
    bool m_IsActive;
#endif

    PersistanceType m_Existence;
    HousesType m_House;
    TEventClass m_EventOne;
    TEventClass m_EventTwo;
    EventLinkType m_EventLinkage;
    TActionClass m_ActionOne;
    TActionClass m_ActionTwo;
    ActionLinkType m_ActionLinkage;
};

#ifdef GAME_DLL
#include "hooker.h"
extern TFixedIHeapClass<TriggerTypeClass> &g_TriggerTypes;
#else
extern TFixedIHeapClass<TriggerTypeClass> g_TriggerTypes;
#endif

#endif // TRIGGERTYPE_H
