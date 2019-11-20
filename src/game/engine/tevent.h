/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Classes holding event information for the trigger system.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TEVENT_H
#define TEVENT_H

#include "always.h"
#include "aircrafttype.h"
#include "buildingtype.h"
#include "dialog.h"
#include "gameptr.h"
#include "infantrytype.h"
#include "teamtype.h"
#include "ttimer.h"
#include "unittype.h"
#include "vesseltype.h"

class TeamTypeClass;

enum TEventType
{
    TEVENT_NO_EVENT, // "-No Event-"
    TEVENT_ENTERED_BY, // "Entered by..."
    TEVENT_SPIED_BY, // "Spied by..."
    TEVENT_THIEVED_BY, // "Thieved by..."
    TEVENT_DISCOVERED_BY_PLAYER, // "Discovered by player"
    TEVENT_HOUSE_DISCOVERED, // "House Discovered..."
    TEVENT_ATTACKED, // "Attacked by anybody"
    TEVENT_DESTROYED, // "Destroyed by anybody"
    TEVENT_ANY, // "Any Event"
    TEVENT_DESTROYED_ALL_UNITS, // "Destroyed, Units, All..."
    TEVENT_DESTROYED_ALL_BUILDINGS, // "Destroyed, Buildings, All..."
    TEVENT_DESTROYED_ALL, // "Destroyed, All..."
    TEVENT_CREDIT_EXCEED, // "Credits exceed (x100)..."
    TEVENT_ELAPSED_TIME, // "Elapsed Time (1/10th min)..."
    TEVENT_TIMER_EXPIRED, // "Mission Timer Expired"
    TEVENT_DESTROYED_BUILDINGS, // "Destroyed, Buildings, #..."
    TEVENT_DESTROYED_UNITS, // "Destroyed, Units, #..."
    TEVENT_NO_FACTORIES, // "No Factories left"
    TEVENT_CIVS_EVACUATED, // "Civilians Evacuated"
    TEVENT_BUILD_BUILDING, // "Build Building Type..."
    TEVENT_BUILD_UNIT, // "Build Unit Type..."
    TEVENT_BUILD_INFANTRY, // "Build Infantry Type..."
    TEVENT_BUILD_AIRCRAFT, // "Build Aircraft Type..."
    TEVENT_LEAVES_MAP, // "Leaves map (team)..."
    TEVENT_ZONE_ENTERED, // "Zone Entry by..."
    TEVENT_CROSSED_HORIZ_LINE, // "Crosses Horizontal Line..."
    TEVENT_CROSSED_VERT_LINE, // "Crosses Vertical Line..."
    TEVENT_GLOBAL_SET, // "Global is set..."
    TEVENT_GLOBAL_CLEAR, // "Global is clear..."
    TEVENT_DESTROYED_ALL_FAKES, // "Destroyed, Fakes, All..."
    TEVENT_LOW_POWER, // "Low Power..."
    TEVENT_ATTACHED_BRIDGE_DESTROYED, // "All bridges destroyed"
    TEVENT_BUILDING_EXISTS, // "Building exists..."
    TEVENT_COUNT,
    TEVENT_FIRST = TEVENT_NO_EVENT,
};

DEFINE_ENUMERATION_OPERATORS(TEventType);

class ObjectClass;

class TDelayEventClass
{
public:
    TDelayEventClass() : m_Bit1(false), m_DelayTimer() {}
    TDelayEventClass(const TDelayEventClass &that) : m_Bit1(that.m_Bit1), m_DelayTimer(that.m_DelayTimer) {}
    TDelayEventClass(const NoInitClass &noinit) {}
    ~TDelayEventClass() {}

    BOOL Get_Bit1() const { return m_Bit1; }
    void Set_Bit1(BOOL value) { m_Bit1 = value; }
    TCountDownTimerClass<FrameTimerClass> &Get_Delay_Timer() { return m_DelayTimer; }
    BOOL Timer_Expired() { return m_DelayTimer.Expired(); }
    void Set_Delay_Timer(int value) { m_DelayTimer = value; }

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Bit1 : 1; // 1
#else
    bool m_Bit1;
#endif
    TCountDownTimerClass<FrameTimerClass> m_DelayTimer;
};

class TEventClass
{
    friend TriggerTypeClass;

public:
    TEventClass() : m_Type(TEVENT_NO_EVENT), m_TeamType(nullptr), m_Value(0) {}
    ~TEventClass() { m_TeamType = nullptr; }

    BOOL operator()(TDelayEventClass &tdevent, TEventType tevent, HousesType house, ObjectClass *object, BOOL a5);

    void Reset(TDelayEventClass &tdevent) const;
    void Build_INI_Entry(char *entry_buffer) const;
    void Read_INI();
    AttachType Attaches_To();
    NeedType Event_Needs() { return Event_Needs(m_Type); };
    void Code_Pointers() {}
    void Decode_Pointers() {}

    TEventType What_Type() const { return m_Type; }

    static TEventType Event_From_Name(const char *name);
    static const char *Name_From_Event(TEventType tevent);
    static NeedType Event_Needs(TEventType tevent);

protected:
    TEventType m_Type;
    GamePtr<TeamTypeClass> m_TeamType;
    int m_Value;

private:
    struct EventTextStruct
    {
        const char *m_Name;
        const char *m_Description;
    };

    static const EventTextStruct s_EventText[TEVENT_COUNT];
};

class EventChoiceClass
{
public:
    EventChoiceClass(TEventType event) : m_Event(event) {}

    void Draw_It(int index, int x, int y, int x_max, int y_max, BOOL selected, TextPrintType style) const;
    const char *Get_Name() const { return TEventClass::Name_From_Event(m_Event); }
    TEventType Get_Event() const { return m_Event; }

    static int Comp(const void *a, const void *b);

private:
    TEventType m_Event;

public:
    static EventChoiceClass s_EventChoices[TEVENT_COUNT];
};

#endif // TEVENT_H
