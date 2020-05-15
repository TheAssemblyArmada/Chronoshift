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
#include "gameevent.h"
#include "animtype.h"
#include "coord.h"
#include "globals.h"
#include "house.h"
#include "mission.h"
#include <captainslog.h>

#define EVENT_DEBUG_LOG(x, ...) \
    /*if ( g_Debug_Print_Events )*/ \
    { \
        captainslog_debug(x, ##__VA_ARGS__); \
    }

GameEventClass::EventInfoStruct GameEventClass::s_EventTypeList[EVENT_COUNT] = {
    { "EMPTY", 0 },// No extra data.
    { "ALLY", sizeof(GameEventClass::HouseEventStruct) },
    { "MEGAMISSION", sizeof(GameEventClass::MegaMissionEventStruct) },
    { "MEGAMISSION_F", sizeof(GameEventClass::MegaMissionFEventStruct) },
    { "IDLE", sizeof(GameEventClass::WhomEventStruct) },
    { "SCATTER", sizeof(GameEventClass::WhomEventStruct) },
    { "DESTRUCT", 0 }, // No extra data.
    { "DEPLOY", 0 }, // No extra data.
    { "PLACE", 3 },
    { "OPTIONS", 0 }, // No extra data.
    { "GAMESPEED", sizeof(GameEventClass::GameSpeedEventStruct) },
    { "PRODUCE", sizeof(GameEventClass::RTTIHeapIDEventStruct) },
    { "SUSPEND", sizeof(GameEventClass::RTTIEventStruct) },
    { "ABANDON", sizeof(GameEventClass::RTTIEventStruct) },
    { "PRIMARY", sizeof(GameEventClass::WhomEventStruct) },
    { "SPECIAL_PLACE", sizeof(GameEventClass::SpecialPlaceEventStruct) },
    { "EXIT", 0 }, // No extra data.
    { "ANIMATION", sizeof(GameEventClass::AnimEventStruct) },
    { "REPAIR", sizeof(GameEventClass::WhomEventStruct) },
    { "SELL", sizeof(GameEventClass::WhomEventStruct) },
    { "SELLCELL", sizeof(GameEventClass::CellEventStruct) },
    { "SPECIAL", sizeof(GameEventClass::SpecialEventStruct) },
    { "FRAME_SYNC", 0 }, // No extra data.
    { "MESSAGE", 0 }, // No extra data.
    { "RESPONSE_TIME", sizeof(GameEventClass::TimeEventStruct) },
    { "FRAME_INFO", sizeof(GameEventClass::FrameEventStruct) },
    { "SAVEGAME", 0 }, // No extra data.
    { "ARCHIVE", sizeof(GameEventClass::WhomTargetEventStruct) },
    { "ADDPLAYER", 4 }, // original table has it as 4 but the event is actually 8.
    { "TIMING", sizeof(GameEventClass::TimingEventStruct) },
    { "PROCESS_TIME", 2 },
    { "PROPOSE_DRAW", 0 }, // No extra data.
    { "RETRACT_DRAW", 0 } // No extra data.
};

GameEventClass::GameEventClass() :
    m_Type(EVENT_EMPTY),
    m_EventFrame(g_GameFrame),
    m_HouseID(0),
    m_IsExecuted(false)
{
    // Removed, as this is used at the default constructor, so it will fire a lot of times...
    // EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));
}

GameEventClass::GameEventClass(const GameEventClass &that) :
    m_Type(that.m_Type),
    m_EventFrame(that.m_EventFrame),
    m_HouseID(that.m_HouseID),
    m_IsExecuted(that.m_IsExecuted)
{
    memcpy(&m_EventData, &that.m_EventData, sizeof(m_EventData));
}

GameEventClass &GameEventClass::operator=(const GameEventClass &that)
{
    if (this != &that) {
        m_Type = that.m_Type;
        m_EventFrame = that.m_EventFrame;
        m_HouseID = that.m_HouseID;
        m_IsExecuted = that.m_IsExecuted;
        memcpy(&m_EventData, &that.m_EventData, sizeof(m_EventData));
    }
    return *this;
}

GameEventClass::GameEventClass(GameEventType type) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));
}

GameEventClass::GameEventClass(GameEventType type, cell_t cell) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Cell.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, uint32_t gamespeed) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_GameSpeed.m_GameSpeed = gamespeed;
}

GameEventClass::GameEventClass(GameEventType type, HousesType house) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_House.m_Whom = house;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTI.m_RTTI = rtti;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti, uint32_t heap_id) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTIHeapID.m_RTTI = rtti;
    m_EventData.u_RTTIHeapID.m_HeapID = heap_id;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti, cell_t cell) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTICell.m_RTTI = rtti;
    m_EventData.u_RTTICell.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, SpecialWeaponType special, cell_t cell) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_SpecialPlace.m_SpecialWeapon = special;
    m_EventData.u_SpecialPlace.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, void *a2, uint32_t a3) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    // Not called... ADDPLAYER?
}

GameEventClass::GameEventClass(GameEventType type, TargetClass whom) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Whom.m_Whom = whom;
}

GameEventClass::GameEventClass(GameEventType type, TargetClass whom, TargetClass target) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_WhomTarget.m_Whom = whom;
    m_EventData.u_WhomTarget.m_Target = target;
}

GameEventClass::GameEventClass(GameEventType type, AnimType anim, HousesType owner, coord_t coord) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event. Anim:%s.",
        Name_From(m_Type),
        AnimTypeClass::Name_From(anim));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Anim.m_Anim = anim;
    m_EventData.u_Anim.m_Owner = owner;
    m_EventData.u_Anim.m_Coord = coord;
}

GameEventClass::GameEventClass(GameEventType type, uint32_t crc, uint16_t cmd_count, uint8_t delay) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Frame.m_CRC = crc;
    m_EventData.u_Frame.m_CommandCount = cmd_count;
    m_EventData.u_Frame.m_Delay = delay;
}

GameEventClass::GameEventClass(GameEventType type, uint16_t desired_frame_rate, uint16_t max_ahead) :
    m_Type(type),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Timing.m_DesiredFrameRate = desired_frame_rate;
    m_EventData.u_Timing.m_MaxAhead = max_ahead;
}

GameEventClass::GameEventClass(TargetClass whom, MissionType mission, TargetClass target, TargetClass dest) :
    m_Type(EVENT_MEGAMISSION),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating EVENT_MEGAMISSION event.");

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_MegaMission.m_Whom = whom;
    m_EventData.u_MegaMission.m_Mission = mission;
    m_EventData.u_MegaMission.m_Target = target;
    m_EventData.u_MegaMission.m_Dest = dest;
}

GameEventClass::GameEventClass(
    TargetClass whom, MissionType mission, TargetClass target, TargetClass dest, SpeedType speed, MPHType max_speed) :
    m_Type(EVENT_MEGAMISSION_F),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating EVENT_MEGAMISSION_F event.");

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_MegaMissionF.m_Whom = whom;
    m_EventData.u_MegaMissionF.m_Mission = mission;
    m_EventData.u_MegaMissionF.m_Target = target;
    m_EventData.u_MegaMissionF.m_Dest = dest;
    m_EventData.u_MegaMissionF.m_FormSpeed = speed;
    m_EventData.u_MegaMissionF.m_FormMaxSpeed = max_speed;
}

GameEventClass::GameEventClass(SpecialClass special) :
    m_Type(EVENT_SPECIAL),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    s_Special = special;
}

GameEventClass::GameEventClass(AnimType anim, HousesType owner, coord_t coord) :
    m_Type(EVENT_ANIMATION),
    m_EventFrame(g_GameFrame),
    m_HouseID(g_PlayerPtr->Get_Heap_ID()),
    m_IsExecuted(false)
{
    EVENT_DEBUG_LOG(
        "GameEventClass::GameEventClass() - Creating ANIMATION event. Anim:%s.\n", AnimTypeClass::Name_From(anim));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Anim.m_Anim = anim;
    m_EventData.u_Anim.m_Owner = owner;
    m_EventData.u_Anim.m_Coord = coord;
}

void GameEventClass::Execute()
{
    EVENT_DEBUG_LOG("GameEventClass::Execute() - Executing '%s' event (GameFrame:%d, EventFrame:%d, HouseID:%d).",
        Name_From(m_Type),
        g_GameFrame,
        m_EventFrame,
        m_HouseID);

    switch (m_Type) {
        case EVENT_ALLY:
            break;
        case EVENT_MEGAMISSION:
            EVENT_DEBUG_LOG("    MEGAMISSION - Whom:%08X, Mission:%s, Target:%08X, Dest:%08X.",
                m_EventData.u_MegaMission.m_Whom,
                MissionClass::Name_From(m_EventData.u_MegaMission.m_Mission),
                m_EventData.u_MegaMission.m_Target,
                m_EventData.u_MegaMission.m_Dest);
            break;
        case EVENT_MEGAMISSION_F:
            EVENT_DEBUG_LOG(
                "    MEGAMISSION_F - Whom:%08X, Mission:%s, Target:%08X\n"
                "                    Dest:%08X, FormSpeed:%d, FormMaxSpeed:%d.\n",
                m_EventData.u_MegaMissionF.m_Whom,
                MissionClass::Name_From(m_EventData.u_MegaMissionF.m_Mission),
                m_EventData.u_MegaMissionF.m_Target,
                m_EventData.u_MegaMissionF.m_Dest,
                m_EventData.u_MegaMissionF.m_FormSpeed,
                m_EventData.u_MegaMissionF.m_FormMaxSpeed);
            break;
        case EVENT_IDLE:
            EVENT_DEBUG_LOG("    IDLE.");
            break;
        case EVENT_SCATTER:
            EVENT_DEBUG_LOG("    SCATTER.");
            break;
        case EVENT_DESTRUCT:
            EVENT_DEBUG_LOG("    DESTRUCT.");
            break;
        case EVENT_DEPLOY:
            EVENT_DEBUG_LOG("    DEPLOY.");
            break;
        case EVENT_PLACE:
            EVENT_DEBUG_LOG("    PLACE.");
            break;
        case EVENT_OPTIONS:
            EVENT_DEBUG_LOG("    OPTIONS.");
            break;
        case EVENT_GAMESPEED:
            EVENT_DEBUG_LOG("    GAMESPEED - GameSpeed:%d.", m_EventData.u_GameSpeed.m_GameSpeed);
            break;
        case EVENT_PRODUCE:
            EVENT_DEBUG_LOG("    PRODUCE.");
            break;
        case EVENT_SUSPEND:
            EVENT_DEBUG_LOG("    SUSPEND.");
            break;
        case EVENT_ABANDON:
            EVENT_DEBUG_LOG("    ABANDON.");
            break;
        case EVENT_PRIMARY:
            EVENT_DEBUG_LOG("    PRIMARY.");
            break;
        case EVENT_SPECIAL_PLACE:
            EVENT_DEBUG_LOG("    SPECIAL_PLACE.");
            break;
        case EVENT_EXIT:
            EVENT_DEBUG_LOG("    EXIT.");
            break;
        case EVENT_ANIMATION:
            EVENT_DEBUG_LOG("    ANIMATION - Anim:%s, Coord: X:%d Y:%d.",
                AnimTypeClass::Name_From(m_EventData.u_Anim.m_Anim),
                Coord_Lepton_X(m_EventData.u_Anim.m_Coord),
                Coord_Lepton_Y(m_EventData.u_Anim.m_Coord));
            break;
        case EVENT_REPAIR:
            EVENT_DEBUG_LOG("    REPAIR.");
            break;
        case EVENT_SELL:
            EVENT_DEBUG_LOG("    SELL.");
            break;
        case EVENT_SELL_CELL:
            EVENT_DEBUG_LOG("    SELL_CELL.");
            break;
        case EVENT_SPECIAL:
            EVENT_DEBUG_LOG("    SPECIAL.");
            break;
        case EVENT_FRAME_SYNC:
            EVENT_DEBUG_LOG("    FRAME_SYNC.");
            break;
        case EVENT_MESSAGE:
            EVENT_DEBUG_LOG("    MESSAGE.");
            break;
        case EVENT_RESPONSE_TIME:
            EVENT_DEBUG_LOG("    RESPONSE_TIME.");
            break;
        case EVENT_FRAME_INFO:
            EVENT_DEBUG_LOG("    FRAME_INFO.");
            break;
        case EVENT_SAVE_GAME:
            EVENT_DEBUG_LOG("    SAVE_GAME.");
            break;
        case EVENT_ARCHIVE:
            EVENT_DEBUG_LOG("    ARCHIVE - Whom:%08X, Target:%08X.", m_EventData.u_WhomTarget.m_Whom, m_EventData.u_WhomTarget.m_Target);
            break;
        case EVENT_ADD_PLAYER:
            EVENT_DEBUG_LOG("    ADD_PLAYER.");
            break;
        case EVENT_TIMING:
            EVENT_DEBUG_LOG("    TIMING.");
            break;
        case EVENT_PROCESS_TIME:
            EVENT_DEBUG_LOG("    PROCESS_TIME.");
            break;
        case EVENT_PROPOSE_DRAW:
            EVENT_DEBUG_LOG("    PROPOSE_DRAW.");
            break;
        case EVENT_RETRACT_DRAW:
            EVENT_DEBUG_LOG("    RETRACT_DRAW.");
            break;
        default:
            break;
    };

#ifdef GAME_DLL
    void (*func)(GameEventClass *) = reinterpret_cast<void (*)(GameEventClass *)>(0x004BD0C8);
    func(this);
#else
#endif
}

char const *GameEventClass::Name_From(GameEventClass::GameEventType type)
{
    captainslog_assert(type < EVENT_COUNT);

    if (type != EVENT_EMPTY && type < EVENT_COUNT) {
        return s_EventTypeList[type].Name;
    }

    return "<none>";
}

GameEventClass::GameEventType GameEventClass::From_Name(char const *name)
{
    captainslog_assert(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return EVENT_EMPTY;
    }

    if (name != nullptr) {
        for (GameEventClass::GameEventType type = EVENT_FIRST; type < EVENT_COUNT; ++type) {
            if (strcasecmp(name, Name_From(type)) == 0) {
                return type;
            }
        }
    }

    return EVENT_EMPTY;
}
