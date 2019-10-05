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

#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include "always.h"
#include "gametypes.h"
#include "rtti.h"
#include "special.h"
#include "target.h"

class GameEventClass
{
public:
    enum GameEventType
    {
        EVENT_NONE = -1,
        EVENT_FIRST = 0,
        EVENT_EMPTY = 0,
        EVENT_ALLY,
        EVENT_MEGAMISSION,
        EVENT_MEGAMISSION_F,
        EVENT_IDLE,
        EVENT_SCATTER,
        EVENT_DESTRUCT,
        EVENT_DEPLOY,
        EVENT_PLACE,
        EVENT_OPTIONS,
        EVENT_GAMESPEED,
        EVENT_PRODUCE,
        EVENT_SUSPEND,
        EVENT_ABANDON,
        EVENT_PRIMARY,
        EVENT_SPECIAL_PLACE,
        EVENT_EXIT,
        EVENT_ANIMATION,
        EVENT_REPAIR,
        EVENT_SELL,
        EVENT_SELL_CELL,
        EVENT_SPECIAL,
        EVENT_FRAME_SYNC,
        EVENT_MESSAGE,
        EVENT_RESPONSE_TIME,
        EVENT_FRAME_INFO,
        EVENT_SAVE_GAME,
        EVENT_ARCHIVE,
        EVENT_ADD_PLAYER,
        EVENT_TIMING,
        EVENT_PROCESS_TIME,
        EVENT_PROPOSE_DRAW,
        EVENT_RETRACT_DRAW,
        EVENT_COUNT
    };

public:
    // Default constructor.
    GameEventClass();
    GameEventClass(const GameEventClass &that);
    ~GameEventClass() {}

    GameEventClass &operator=(const GameEventClass &that);

    // General constructors.
    GameEventClass(GameEventType type); // For events: (without special data) - DESTRUCT, SAVEGAME, RETRACT_DRAW, PROPOSE_DRAW, OPTIONS, EXIT
    GameEventClass(GameEventType type, cell_t cell); // For events: SELLCELL
    GameEventClass(GameEventType type, unsigned int gamespeed); // For events: GAMESPEED
    GameEventClass(GameEventType type, HousesType house); // For events: ALLY
    GameEventClass(GameEventType type, RTTIType rtti); // For events: SUSPEND, ABANDON
    GameEventClass(GameEventType type, RTTIType rtti, unsigned int heap_id); // For events: PLACE, PRODUCE
    GameEventClass(GameEventType type, RTTIType rtti, cell_t cell); // For events:
    GameEventClass(GameEventType type, SpecialWeaponType special, cell_t cell); // For events: SPECIAL_PLACE
    GameEventClass(GameEventType type, void *a2, unsigned long a3); // For events:
    GameEventClass(GameEventType type, TargetClass whom); // For events: PRIMARY, SELL, IDLE, SCATTER, REPAIR
    GameEventClass(GameEventType type, TargetClass whom, TargetClass target); // For events: ARCHIVE
    GameEventClass(GameEventType type, AnimType anim, HousesType owner, coord_t coord); // For events: ANIMATION
    GameEventClass(GameEventType type, unsigned int crc, unsigned short cmd_count, unsigned char delay); // For events: FRAME_SYNC
    GameEventClass(GameEventType type, unsigned short desired_frame_rate, unsigned short max_ahead); // For events: TIMING

    // TODO: Once GameEventClass is complete, and code for event below is under control, move
    //        these above and use the constructors above that explicitly take the event type.

    // Mega mission constructors.
    GameEventClass(TargetClass whom, MissionType mission, TargetClass target, TargetClass dest); // For events: MEGAMISSION
    GameEventClass(TargetClass whom, MissionType mission, TargetClass target, TargetClass dest, SpeedType speed, MPHType max_speed); // For events: MEGAMISSION_F

    // Specialised constructors.
    GameEventClass(SpecialClass special); // For events: Hardcoded to use SPECIAL
    GameEventClass(AnimType anim, HousesType owner, coord_t coord); // For events: Hardcoded to use ANIMATION

    void Execute();

    int Get_Event_Frame() const { return m_EventFrame; }
    void Set_Event_Frame(int frame) { m_EventFrame = frame; }
    int Get_House_ID() const { return m_HouseID; }
    void Set_House_ID(int id) { m_HouseID = id; }
    BOOL Is_Executed() const { return m_IsExecuted; }
    void Set_Executed() { m_IsExecuted = true; }

    const char *Name() const { return Name_From(m_Type); }

private:
    struct MegaMissionEventStruct
    {
        target_t m_Whom;
        MissionType m_Mission;
        target_t m_Target;
        target_t m_Dest;
    };

    struct MegaMissionFEventStruct
    {
        target_t m_Whom;
        MissionType m_Mission;
        target_t m_Target;
        target_t m_Dest;
        SpeedType m_FormSpeed;
        MPHType m_FormMaxSpeed;
    };

    struct WhomEventStruct
    {
        target_t m_Whom;
    };

    struct WhomTargetEventStruct
    {
        target_t m_Whom;
        target_t m_Target;
    };

    struct WhomTargetDestEventStruct
    {
        target_t m_Whom;
        target_t m_Target;
        target_t m_Dest;
    };

    struct HouseEventStruct
    {
        int /*HousesType*/ m_Whom;
    };

    struct AnimEventStruct
    {
        AnimType m_Anim;
        HousesType m_Owner;
        coord_t m_Coord;
    };

    struct CellEventStruct
    {
        cell_t m_Cell;
    };

    struct RTTIEventStruct
    {
        RTTIType m_RTTI;
    };

    struct RTTICellEventStruct
    {
        RTTIType m_RTTI;
        cell_t m_Cell;
    };

    struct RTTIHeapIDEventStruct
    {
        RTTIType m_RTTI;
        int m_HeapID;
    };

    struct SpecialPlaceEventStruct
    {
        int /*SpecialWeaponType*/ m_Special;
        cell_t m_Cell;
    };

    struct SpecialEventStruct
    {
        void operator=(const SpecialClass &that) { m_Special = that.Pack(); }
        uint32_t m_Special;
    };

    struct FrameEventStruct
    {
        unsigned int m_CRC;
        unsigned short m_CommandCount;
        unsigned char m_Delay;
    };

    struct TimingEventStruct
    {
        unsigned int m_DesiredFrameRate : 16; // 1
        unsigned int m_MaxAhead : 16; // 16
    };

    struct TimeEventStruct
    {
        unsigned char m_MaxAhead;
    };

    struct GameSpeedEventStruct
    {
        unsigned int m_GameSpeed;
    };

private:
    GameEventType m_Type; // The type for this object.

#ifndef CHRONOSHIFT_NO_BITFIELDS
    unsigned int m_EventFrame : 26; //
    unsigned int m_HouseID : 5; //
    BOOL m_IsExecuted : 1; //
#else
    unsigned int m_EventFrame; // The frame this event was created on.
    unsigned int m_HouseID; // The id of the house that triggered this event.
    bool m_IsExecuted; // Has this event been executed? This use to make sure the event is
                       // not executed twice and also used for cleaning up the queue.
#endif

    union
    {
        MegaMissionEventStruct u_MegaMission;
        MegaMissionFEventStruct u_MegaMissionF;
        WhomEventStruct u_Whom;
        WhomTargetEventStruct u_WhomTarget;
        RTTICellEventStruct u_RTTICell;
        RTTIHeapIDEventStruct u_RTTIHeapID;
        RTTIEventStruct u_RTTI;
        HouseEventStruct u_House;
        CellEventStruct u_Cell;
        AnimEventStruct u_Anim;
        SpecialPlaceEventStruct u_SpecialPlace;
        SpecialEventStruct u_Special;
        FrameEventStruct u_Frame;
        TimingEventStruct u_Timing;
        TimeEventStruct u_ResponseTime;
        GameSpeedEventStruct u_GameSpeed;
    } m_EventData;

public:
    struct EventInfoStruct
    {
        char const *Name;
        int RawSize;
    };

    static EventInfoStruct EventTypeList[EVENT_COUNT];
    static char const *Name_From(GameEventType type);
    static GameEventType From_Name(char const *name);
};

DEFINE_ENUMERATION_OPERATORS(GameEventClass::GameEventType);

#endif // GAMEEVENT_H
