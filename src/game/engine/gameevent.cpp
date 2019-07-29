/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
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
#include "anim.h"
#include "animtype.h"
#include "building.h"
#include "callback.h"
#include "coord.h"
#include "foot.h"
#include "gamedebug.h"
#include "gameoptions.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"
#include "mission.h"
#include "msgbox.h"
#include "rules.h"
#include "saveload.h"
#include "scenario.h"
#include "session.h"
#include "team.h"
#include "techno.h"
#include "theme.h"
#include "vessel.h"

#define EVENT_DEBUG_LOG(x, ...) /*if ( g_Debug_Print_Events )*/ \
    { \
        DEBUG_LOG(x, ##__VA_ARGS__); \
    }

GameEventClass::EventInfoStruct GameEventClass::EventTypeList[EVENT_COUNT] = {
    { "EMPTY", 0 }, // No extra data.
    { "ALLY", sizeof(GameEventClass::HouseEventStruct) },
    { "MEGAMISSION", sizeof(GameEventClass::MegaMissionEventStruct) },
    { "MEGAMISSION_F", sizeof(GameEventClass::MegaMissionFEventStruct) },
    { "IDLE", sizeof(GameEventClass::WhomEventStruct) },
    { "SCATTER", sizeof(GameEventClass::WhomEventStruct) },
    { "DESTRUCT", 0 }, // No extra data.
    { "DEPLOY", 0 }, // No extra data.
    { "PLACE", sizeof(GameEventClass::RTTICellEventStruct) },
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
    { "RESPONSE_TIME", sizeof(GameEventClass::ResponseTimeEventStruct) },
    { "FRAME_INFO", sizeof(GameEventClass::FrameEventStruct) },
    { "SAVEGAME", 0 }, // No extra data.
    { "ARCHIVE", sizeof(GameEventClass::WhomTargetEventStruct) },
    { "ADDPLAYER", 4 }, // original table has it as 4 but the event is actually 8
    { "TIMING", sizeof(GameEventClass::TimingEventStruct) },
    { "PROCESS_TIME", sizeof(GameEventClass::ProcessTimeEventStruct) },
    { "PROPOSE_DRAW", 0 }, // No extra data.
    { "RETRACT_DRAW", 0 } // No extra data.
};

GameEventClass::GameEventClass() : m_Type(EVENT_EMPTY), m_EventFrame(g_GameFrame), m_HouseID(0), m_IsExecuted(false)
{
    // Removed, as this is used at the default constructor, so it will fire a lot of times...
    // EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));
}

GameEventClass::GameEventClass(const GameEventClass &that) :
    m_Type(that.m_Type), m_EventFrame(that.m_EventFrame), m_HouseID(that.m_HouseID), m_IsExecuted(that.m_IsExecuted)
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
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));
}

GameEventClass::GameEventClass(GameEventType type, cell_t cell) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Cell.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, unsigned int gamespeed) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_GameSpeed.m_GameSpeed = gamespeed;
}

GameEventClass::GameEventClass(GameEventType type, HousesType house) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_House.m_Whom = house;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTI.m_RTTI = rtti;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti, int heap_id) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTIHeapID.m_RTTI = rtti;
    m_EventData.u_RTTIHeapID.m_HeapID = heap_id;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti, cell_t cell) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTICell.m_RTTI = rtti;
    m_EventData.u_RTTICell.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, SpecialWeaponType special, cell_t cell) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_SpecialPlace.m_SpecialWeapon = special;
    m_EventData.u_SpecialPlace.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, void *a2, unsigned long a3) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    // Not called... ADDPLAYER?
}

GameEventClass::GameEventClass(GameEventType type, TargetClass whom) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Whom.m_Whom = whom;
}

GameEventClass::GameEventClass(GameEventType type, TargetClass whom, TargetClass target) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_WhomTarget.m_Whom = whom;
    m_EventData.u_WhomTarget.m_Target = target;
}

GameEventClass::GameEventClass(GameEventType type, AnimType anim, HousesType owner, coord_t coord) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event. Anim:%s.\n",
        Name_From(m_Type),
        AnimTypeClass::Name_From(anim));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Anim.m_Anim = anim;
    m_EventData.u_Anim.m_Owner = owner;
    m_EventData.u_Anim.m_Coord = coord;
}

GameEventClass::GameEventClass(GameEventType type, unsigned int crc, unsigned short cmd_count, unsigned char delay) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Frame.m_CRC = crc;
    m_EventData.u_Frame.m_CommandCount = cmd_count;
    m_EventData.u_Frame.m_Delay = delay;
}

GameEventClass::GameEventClass(GameEventType type, unsigned short desired_frame_rate, unsigned short max_ahead) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Timing.m_DesiredFrameRate = desired_frame_rate;
    m_EventData.u_Timing.m_MaxAhead = max_ahead;
}

GameEventClass::GameEventClass(TargetClass whom, MissionType mission, TargetClass target, TargetClass dest) :
    m_Type(EVENT_MEGAMISSION), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating EVENT_MEGAMISSION event.\n");

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_MegaMission.m_Whom = whom;
    m_EventData.u_MegaMission.m_Mission = mission;
    m_EventData.u_MegaMission.m_Target = target;
    m_EventData.u_MegaMission.m_Dest = dest;
}

GameEventClass::GameEventClass(
    TargetClass whom, MissionType mission, TargetClass target, TargetClass dest, SpeedType speed, MPHType max_speed) :
    m_Type(EVENT_MEGAMISSION_F), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating EVENT_MEGAMISSION_F event.\n");

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_MegaMissionF.m_Whom = whom;
    m_EventData.u_MegaMissionF.m_Mission = mission;
    m_EventData.u_MegaMissionF.m_Target = target;
    m_EventData.u_MegaMissionF.m_Dest = dest;
    m_EventData.u_MegaMissionF.m_FormSpeed = speed;
    m_EventData.u_MegaMissionF.m_FormMaxSpeed = max_speed;
}

GameEventClass::GameEventClass(SpecialClass special) :
    m_Type(EVENT_SPECIAL), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.\n", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Special = special;
}

GameEventClass::GameEventClass(AnimType anim, HousesType owner, coord_t coord) :
    m_Type(EVENT_ANIMATION), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
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
    BOOL formation_event = false;
    FootClass *fptr = nullptr;
    TechnoClass *tptr = nullptr;
    char message[100] = { '\0' };
    char *messageptr = nullptr;
    EVENT_DEBUG_LOG("GameEventClass::Execute() - Executing '%s' event (GameFrame:%d, EventFrame:%d, ID:%d).\n",
        Name_From(m_Type),
        g_GameFrame,
        m_EventFrame,
        m_HouseID);

    switch (m_Type) {
        case EVENT_EMPTY:
            break;
        // todo confirm its working when we have online play
        case EVENT_ALLY: {
            if (g_Houses[m_HouseID].Is_Ally((HousesType)m_EventData.u_House.m_Whom)) {
                g_Houses[m_HouseID].Make_Enemy((HousesType)m_EventData.u_House.m_Whom);
            } else {
                g_Houses[m_HouseID].Make_Ally((HousesType)m_EventData.u_House.m_Whom);
            }
            break;
        }
        case EVENT_MEGAMISSION_F: {
            EVENT_DEBUG_LOG(
                "    MEGAMISSION_F - Whom:%08X, Mission:%s, Target:%08X\n"
                "                    Dest:%08X, FormSpeed:%d, FormMaxSpeed:%d.\n",
                m_EventData.u_MegaMissionF.m_Whom,
                MissionClass::Name_From(m_EventData.u_MegaMissionF.m_Mission),
                m_EventData.u_MegaMissionF.m_Target,
                m_EventData.u_MegaMissionF.m_Dest,
                m_EventData.u_MegaMissionF.m_FormSpeed,
                m_EventData.u_MegaMissionF.m_FormMaxSpeed);
            fptr = (FootClass *)(TechnoClass *)As_Techno(m_EventData.u_MegaMissionF.m_Whom);
            if (fptr != nullptr && fptr->Is_Active()) {
                if (fptr->Is_Foot() && fptr->Is_Active()) {
                    fptr->Set_In_Formation(true);
                    fptr->Set_Team_Speed(m_EventData.u_MegaMissionF.m_FormSpeed);
                    g_FormMove = true;
                    fptr->Set_Team_Max_Speed(m_EventData.u_MegaMissionF.m_FormMaxSpeed);
                    formation_event = true;
                    g_FormMaxSpeed = m_EventData.u_MegaMissionF.m_FormMaxSpeed;
                    g_FormSpeed = m_EventData.u_MegaMissionF.m_FormSpeed;
                }
            }
            // fallthough
        }
        case EVENT_MEGAMISSION: {
            EVENT_DEBUG_LOG("    MEGAMISSION - Whom:%08X, Mission:%s, Target:%08X, Dest:%08X.\n",
                m_EventData.u_MegaMission.m_Whom,
                MissionClass::Name_From(m_EventData.u_MegaMission.m_Mission),
                m_EventData.u_MegaMission.m_Target,
                m_EventData.u_MegaMission.m_Dest);
            TechnoClass *whom = As_Techno(m_EventData.u_MegaMission.m_Whom);

            if (whom != nullptr) {
                if (!whom->Is_Active() || !whom->Get_Health() || whom->In_Limbo()) {
                    break;
                }
            }
            ObjectClass *target = nullptr;

            if (Target_Get_RTTI(m_EventData.u_MegaMission.m_Target) != RTTI_NONE) {
                target = As_Object(m_EventData.u_MegaMission.m_Target);
                if (target != nullptr) {
                    if (!target->Is_Active() || !target->Get_Health() || target->In_Limbo()) {
                        break;
                    }
                }
            }
            if (Target_Get_RTTI(m_EventData.u_MegaMission.m_Dest) != RTTI_NONE) {
                target = As_Object(m_EventData.u_MegaMission.m_Dest);
                if (target != nullptr) {
                    if (!target->Is_Active() || !target->Get_Health() || target->In_Limbo()) {
                        break;
                    }
                }
            }
            if (!whom->Is_Tethered()) {
                whom->Transmit_Message(RADIO_OVER_AND_OUT);
            }
            FootClass *whomf = reinterpret_cast<FootClass *>(whom);
            if (whom->Is_Foot()) {
                if (formation_event == false) {
                    whomf->Set_In_Formation(false);
                }
                if (whomf->Get_Team() != nullptr) {
                    whomf->Get_Team()->Remove(whomf, -1);
                }
            }
            if (target != nullptr) {
                if (g_PlayerPtr->Is_Ally(whom)) {
                    target->Clicked_As_Target(7);
                }
            }
            whom->Assign_Mission(m_EventData.u_MegaMission.m_Mission);
            if (whom->Is_Foot()) {
                whomf->Set_Suspended_NavCom(0);
            }
            whom->Set_Suspended_TarCom(0);
            if (m_EventData.u_MegaMission.m_Mission == MISSION_AREA_GUARD && whom->Is_Foot()) {
                whom->Assign_Target(0);
                whom->Assign_Destination(m_EventData.u_MegaMission.m_Target);
                whom->Set_Archive(m_EventData.u_MegaMission.m_Target);
            } else if (m_EventData.u_MegaMission.m_Mission == MISSION_QMOVE && whom->Is_Foot()) {
                whomf->Queue_Navigation_List(m_EventData.u_MegaMission.m_Dest);
            } else {
                if (whom->Is_Foot()) {
                    whomf->Clear_Navigation_List();
                }
                whom->Assign_Target(m_EventData.u_MegaMission.m_Target);
                whom->Assign_Destination(m_EventData.u_MegaMission.m_Dest);
            }
            if (m_EventData.u_MegaMission.m_Mission == MISSION_MOVE) {
                // TODO add code here to check for player and allies,
                // as it stands this seems like it allows repairing at any shipyard, be it yours or enemy's
                if (whom->What_Am_I() == RTTI_VESSEL) {
                    VesselClass *whomv = reinterpret_cast<VesselClass *>(whom);
                    BuildingClass *targetb = reinterpret_cast<BuildingClass *>(target);
                    if (targetb != nullptr && targetb->What_Am_I() == RTTI_BUILDING
                        && (targetb->What_Type() == BUILDING_SYRD || targetb->What_Type() == BUILDING_SPEN)) {
                        whomv->Set_Self_Repair(true);
                    } else {
                        whomv->Set_Self_Repair(false);
                        whomv->Set_Repairing(false);
                    }
                }
            }
            break;
        }
        case EVENT_IDLE: {
            EVENT_DEBUG_LOG("    IDLE - Whom:%08X.\n", m_EventData.u_Whom.m_Whom);
            fptr = (FootClass *)As_Techno(m_EventData.u_Whom.m_Whom);
            if (fptr != nullptr && fptr->Is_Active() && !fptr->In_Limbo() && !fptr->Is_Tethered()
                && fptr->What_Am_I() != RTTI_BUILDING) {
                fptr->Transmit_Message(RADIO_OVER_AND_OUT);
                fptr->Assign_Destination(0);
                fptr->Assign_Target(0);
                fptr->Enter_Idle_Mode();
                if (fptr->Is_Foot()) {
                    fptr->Clear_Navigation_List();
                }
            }
            break;
        }
        case EVENT_SCATTER: {
            EVENT_DEBUG_LOG("    SCATTER - Whom:%08X.\n", m_EventData.u_Whom.m_Whom);
            fptr = (FootClass *)As_Techno(m_EventData.u_Whom.m_Whom);
            if (fptr->Is_Foot() && fptr->Is_Active() && !fptr->In_Limbo() && !fptr->Is_Tethered()) {
                fptr->Set_To_Scatter(true);
                fptr->Scatter(0, 1, 0);
            }
            break;
        }
        case EVENT_DESTRUCT: {
            EVENT_DEBUG_LOG("    DESTRUCT.\n");
            g_Houses[m_HouseID].Flag_To_Die();
            break;
        }
        case EVENT_DEPLOY: {
            EVENT_DEBUG_LOG("    DEPLOY.\n");
            break;
        }
        case EVENT_PLACE: {
            EVENT_DEBUG_LOG("    PLACE - RTTI:%d Cell:%d.\n", m_EventData.u_RTTICell.m_RTTI, m_EventData.u_RTTICell.m_Cell);
            g_Houses[m_HouseID].Place_Object(m_EventData.u_RTTICell.m_RTTI, m_EventData.u_RTTICell.m_Cell);
            break;
        }
        case EVENT_OPTIONS: {
            EVENT_DEBUG_LOG("    OPTIONS.\n");
            if (!Session.Playback_Game()) {
                g_SpecialDialog = SPECIAL_DLG_OPTIONS;
            }
            break;
        }
        case EVENT_GAMESPEED: {
            EVENT_DEBUG_LOG("    GAMESPEED - GameSpeed:%d.\n", m_EventData.u_GameSpeed.m_GameSpeed);
            Options.Set_Game_Speed(m_EventData.u_GameSpeed.m_GameSpeed);
            break;
        }
        case EVENT_PRODUCE: {
            EVENT_DEBUG_LOG("    PRODUCE - RTTI:%d HeapID:%d.\n", m_EventData.u_RTTIHeapID.m_RTTI, m_EventData.u_RTTIHeapID.m_HeapID);
            g_Houses[m_HouseID].Begin_Production(m_EventData.u_RTTIHeapID.m_RTTI, m_EventData.u_RTTIHeapID.m_HeapID);
            break;
        }
        case EVENT_SUSPEND: {
            EVENT_DEBUG_LOG("    SUSPEND - RTTI:%d.\n", m_EventData.u_RTTI.m_RTTI);
            g_Houses[m_HouseID].Suspend_Production(m_EventData.u_RTTI.m_RTTI);
            break;
        }
        case EVENT_ABANDON: {
            EVENT_DEBUG_LOG("    ABANDON - RTTI:%d.\n", m_EventData.u_RTTI.m_RTTI);
            g_Houses[m_HouseID].Abandon_Production(m_EventData.u_RTTI.m_RTTI);
            break;
        }
        case EVENT_PRIMARY: {
            EVENT_DEBUG_LOG("    PRIMARY - Whom:%08X.\n", m_EventData.u_Whom.m_Whom);
            if (Target_Get_RTTI(m_EventData.u_Whom.m_Whom) == RTTI_BUILDING) {
                BuildingClass *bptr = (BuildingClass *)As_Techno(m_EventData.u_Whom.m_Whom);
                if (bptr != nullptr && bptr->Is_Active()) {
                    bptr->Toggle_Primary();
                }
            }
            break;
        }
        case EVENT_SPECIAL_PLACE: {
            EVENT_DEBUG_LOG("    SPECIAL_PLACE - Special Weapon:%d Cell:%d.\n", m_EventData.u_SpecialPlace.m_SpecialWeapon, m_EventData.u_SpecialPlace.m_Cell);
            g_Houses[m_HouseID].Place_Special_Blast(
                (SpecialWeaponType)m_EventData.u_SpecialPlace.m_SpecialWeapon, m_EventData.u_SpecialPlace.m_Cell);
            break;
        }
        case EVENT_EXIT: {
            EVENT_DEBUG_LOG("    EXIT.\n");
            Theme.Fade_Out();
            Stop_Speaking();
            Speak(VOX_BATTLE_CONTROL_TERMINATED);
            while (Is_Speaking()) {
                Call_Back();
            }
            GameActive = false;
            break;
        }
        case EVENT_ANIMATION: {
            EVENT_DEBUG_LOG("    ANIMATION - Anim:%s, Coord: X:%d Y:%d.\n",
                AnimTypeClass::Name_From(m_EventData.u_Anim.m_Anim),
                Coord_Lepton_X(m_EventData.u_Anim.m_Coord),
                Coord_Lepton_Y(m_EventData.u_Anim.m_Coord));
            AnimClass *aptr = new AnimClass(m_EventData.u_Anim.m_Anim, m_EventData.u_Anim.m_Coord, 0, 1);
            if (aptr != nullptr && m_EventData.u_Anim.m_Owner != HOUSES_NONE) {
                if (g_PlayerPtr->What_Type() != m_EventData.u_Anim.m_Owner) {
                    aptr->Make_Invisible();
                }
            }
            break;
        }
        case EVENT_REPAIR: {
            EVENT_DEBUG_LOG("    REPAIR - Whom:%08X.\n", m_EventData.u_Whom.m_Whom);
            tptr = As_Techno(m_EventData.u_Whom.m_Whom);
            if (tptr != nullptr && tptr->Is_Active()) {
                tptr->Repair();
            }
            break;
        }
        case EVENT_SELL: {
            EVENT_DEBUG_LOG("    SELL - Whom:%08X.\n", m_EventData.u_Whom.m_Whom);
            tptr = As_Techno(m_EventData.u_Whom.m_Whom);
            if (tptr != nullptr && tptr->Is_Active()) {
                if (&g_Houses[m_HouseID] == tptr->Get_Owner_House()) {
                    if ((tptr->What_Am_I() == RTTI_BUILDING || tptr->What_Am_I() == RTTI_UNIT)
                        && Map[tptr->Center_Cell()].Cell_Building() != nullptr) {
                        tptr->Sell_Back(-1);
                    }
                }
            }
            break;
        }
        case EVENT_SELL_CELL: {
            EVENT_DEBUG_LOG("    SELL_CELL - Cell:%d.\n", m_EventData.u_Cell.m_Cell);
            g_Houses[m_HouseID].Sell_Wall(m_EventData.u_Cell.m_Cell);
            break;
        }
        // todo test this when we have a functional special options dialog
        case EVENT_SPECIAL: {
            EVENT_DEBUG_LOG("    SPECIAL.\n");
            Special.Set(m_EventData.u_Special.m_Special);
            sprintf(message, Fetch_String(TXT_SPECIAL_WARNING), g_Houses[m_HouseID].Get_Name());
            Session.Get_Messages().Add_Message(
                nullptr, 0, message, g_Houses[m_HouseID].Get_Color(), TPF_12PT_METAL | TPF_OUTLINE, 1200);
            Map.Flag_To_Redraw();
            break;
        }
        case EVENT_FRAME_SYNC: {
            EVENT_DEBUG_LOG("    FRAME_SYNC.\n");
            break;
        }
        case EVENT_MESSAGE: {
            EVENT_DEBUG_LOG("    MESSAGE.\n");
            break;
        }
        // todo confirm its working when we have online play
        case EVENT_RESPONSE_TIME: {
            EVENT_DEBUG_LOG("    RESPONSE_TIME - Changing MaxAhead to frames %d.\n", m_EventData.u_ResponseTime.m_MaxAhead);
            Session.Set_MaxAhead(m_EventData.u_ResponseTime.m_MaxAhead);
            break;
        }
        case EVENT_FRAME_INFO: {
            EVENT_DEBUG_LOG("    FRAME_INFO.\n");
            break;
        }
        // todo confirm its working when we have online play
        case EVENT_SAVE_GAME: {
            EVENT_DEBUG_LOG("    SAVE_GAME.\n");
            if (g_SpecialDialog != SPECIAL_DLG_NONE) {
                Save_Game(-1, Fetch_String(TXT_MULTIPLAYER_GAME));
            } else {
                TCountDownTimerClass<SystemTimerClass> timer(60);
                MessageBoxClass msg;
                msg.Process(TXT_SAVING_GAME);
                Save_Game(-1, Fetch_String(TXT_MULTIPLAYER_GAME));
                while (timer.Time() > 0) {
                    Call_Back();
                }
                g_hidPage.Clear();
                Map.Flag_To_Redraw(true);
                Map.Render();
            }
            break;
        }
        // todo figure out how to test, seems only Conyard invokes it... for what?
        case EVENT_ARCHIVE: {
            EVENT_DEBUG_LOG("    ARCHIVE - Whom:%08X, Target:%08X.\n",
                m_EventData.u_WhomTarget.m_Whom,
                m_EventData.u_WhomTarget.m_Target);
            tptr = As_Techno(m_EventData.u_WhomTarget.m_Whom);
            if (tptr != nullptr && tptr->Is_Active()) {
                tptr->Set_Archive(m_EventData.u_WhomTarget.m_Target);
            }
            break;
        }

        case EVENT_ADD_PLAYER: {
            EVENT_DEBUG_LOG("    ADD_PLAYER.\n");
            for (int i = 0; i < m_EventData.u_AddPlayer.m_uintval; ++i) {
                EVENT_DEBUG_LOG("%d\n", m_EventData.u_AddPlayer.m_pointer[i]);
            }
            if (m_HouseID != g_PlayerPtr->Get_Heap_ID()) {
                delete m_EventData.u_AddPlayer.m_pointer;
            }
            break;
        }
        // todo confirm its working when we have online play
        case EVENT_TIMING: {
            EVENT_DEBUG_LOG("    TIMING - DesiredFrameRate:%d MaxAhead:%d.\n",
                m_EventData.u_Timing.m_DesiredFrameRate,
                m_EventData.u_Timing.m_MaxAhead);
            if (m_EventData.u_Timing.m_MaxAhead < Session.Get_MaxAhead()) {
                g_NewMaxAheadFrame1 = m_EventData.u_Timing.m_DesiredFrameRate;
                g_NewMaxAheadFrame2 = m_EventData.u_Timing.m_MaxAhead + g_NewMaxAheadFrame1;
            }
            Session.Set_Desired_Frame_Rate(m_EventData.u_Timing.m_DesiredFrameRate);
            Session.Set_MaxAhead(m_EventData.u_Timing.m_MaxAhead);
            break;
        }
        // todo this needs to be rechecked
        // todo confirm its working when we have online play
        case EVENT_PROCESS_TIME: {
            NodeNameTag *nodename;
            for (int i = 0; i < Session.Player_Count(); ++i) {
                nodename = Session.Player(i);
                if (m_HouseID == nodename->House) {
                    nodename->field_19 = m_EventData.u_ProcessTime.m_Ticks;
                    break;
                }
            }
            EVENT_DEBUG_LOG("    PROCESS_TIME - %04x ticks.\n", m_EventData.u_ProcessTime.m_Ticks);
            // this was here but, this doesn't seem safe so moved above...
            //nodename->field_19 = m_EventData.u_ProcessTime.m_Ticks;
            break;
        }
        // todo this needs to be rechecked
        // todo confirm its working when we have online play
        case EVENT_PROPOSE_DRAW: {
            EVENT_DEBUG_LOG("    PROPOSE_DRAW.\n");
            if (m_HouseID == g_PlayerPtr->Get_Heap_ID()) {
                if (!Scen.Get_field_7D3()) {
                    Scen.Set_field_7CF(true);
                    break;
                }
                Scen.Set_field_7CF(true);
                messageptr = "You have proposed that the game be declared a draw.";
            } else {
                if (!Scen.Get_field_7CF()) {
                    Scen.Set_field_7D3(true);
                    break;
                }
                for (int i = 0; i < Session.Player_Count(); ++i) {
                    if (m_HouseID == Session.Player(i)->House) {
                        sprintf(
                            message, "%s has proposed that the game be declared a draw.", Session.Player(i)->Name);
                        break;
                    }
                }
                Scen.Set_field_7D3(true);
                messageptr = message;
            }
            Session.Get_Messages().Add_Message(0,
                0,
                messageptr,
                PLAYER_COLOR_YELLOW,
                TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL,
                900 * Rule.Message_Delay());
            Sound_Effect(VOC_INCOMING_MESSAGE);
            break;
        }
        // todo this needs to be rechecked
        // todo confirm its working when we have online play
        case EVENT_RETRACT_DRAW: {
            EVENT_DEBUG_LOG("    RETRACT_DRAW.\n");

            if (m_HouseID == g_PlayerPtr->Get_Heap_ID()) {
                Scen.Set_field_7CF(false);
                messageptr = "You have retracted your offer of a draw.";
            } else {
                for (int i = 0; i < Session.Player_Count(); ++i) {
                    if (m_HouseID == Session.Player(i)->House) {
                        sprintf(message, "%s has retracted the offer of a draw.", Session.Player(i)->Name);
                        break;
                    }
                }
                Scen.Set_field_7CF(false);
                messageptr = message;
            }
            Session.Get_Messages().Add_Message(0,
                0,
                messageptr,
                PLAYER_COLOR_YELLOW,
                TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL,
                900 * Rule.Message_Delay());
            Sound_Effect(VOC_INCOMING_MESSAGE);
            break;
        }
        default:
            break;
    }
}

char const *GameEventClass::Name_From(GameEventClass::GameEventType type)
{
    DEBUG_ASSERT(type < EVENT_COUNT);

    if (type != EVENT_EMPTY && type < EVENT_COUNT) {
        return EventTypeList[type].Name;
    }

    return "<none>";
}

GameEventClass::GameEventType GameEventClass::From_Name(char const *name)
{
    DEBUG_ASSERT(name != nullptr);

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
