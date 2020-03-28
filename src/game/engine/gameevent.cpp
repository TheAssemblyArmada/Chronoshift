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
        captainslog_debug(x, ##__VA_ARGS__); \
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
    // EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

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
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));
}

GameEventClass::GameEventClass(GameEventType type, cell_t cell) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Cell.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, unsigned int gamespeed) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_GameSpeed.m_GameSpeed = gamespeed;
}

GameEventClass::GameEventClass(GameEventType type, HousesType house) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_House.m_Whom = house;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTI.m_RTTI = rtti;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti, unsigned int heap_id) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTIHeapID.m_RTTI = rtti;
    m_EventData.u_RTTIHeapID.m_HeapID = heap_id;
}

GameEventClass::GameEventClass(GameEventType type, RTTIType rtti, cell_t cell) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_RTTICell.m_RTTI = rtti;
    m_EventData.u_RTTICell.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, SpecialWeaponType special, cell_t cell) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_SpecialPlace.m_SpecialWeapon = special;
    m_EventData.u_SpecialPlace.m_Cell = cell;
}

GameEventClass::GameEventClass(GameEventType type, void *a2, unsigned long a3) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    // Not called... ADDPLAYER?
}

GameEventClass::GameEventClass(GameEventType type, TargetClass whom) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Whom.m_Whom = whom;
}

GameEventClass::GameEventClass(GameEventType type, TargetClass whom, TargetClass target) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_WhomTarget.m_Whom = whom;
    m_EventData.u_WhomTarget.m_Target = target;
}

GameEventClass::GameEventClass(GameEventType type, AnimType anim, HousesType owner, coord_t coord) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event. Anim:%s.",
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
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Frame.m_CRC = crc;
    m_EventData.u_Frame.m_CommandCount = cmd_count;
    m_EventData.u_Frame.m_Delay = delay;
}

GameEventClass::GameEventClass(GameEventType type, unsigned short desired_frame_rate, unsigned short max_ahead) :
    m_Type(type), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Timing.m_DesiredFrameRate = desired_frame_rate;
    m_EventData.u_Timing.m_MaxAhead = max_ahead;
}

GameEventClass::GameEventClass(TargetClass whom, MissionType mission, TargetClass target, TargetClass dest) :
    m_Type(EVENT_MEGAMISSION), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
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
    m_Type(EVENT_MEGAMISSION_F), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
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
    m_Type(EVENT_SPECIAL), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG("GameEventClass::GameEventClass() - Creating '%s' event.", Name_From(m_Type));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Special = special;
}

GameEventClass::GameEventClass(AnimType anim, HousesType owner, coord_t coord) :
    m_Type(EVENT_ANIMATION), m_EventFrame(g_GameFrame), m_HouseID(g_PlayerPtr->Get_Heap_ID()), m_IsExecuted(false)
{
    EVENT_DEBUG_LOG(
        "GameEventClass::GameEventClass() - Creating ANIMATION event. Anim:%s.", AnimTypeClass::Name_From(anim));

    memset(&m_EventData, 0, sizeof(m_EventData));

    m_EventData.u_Anim.m_Anim = anim;
    m_EventData.u_Anim.m_Owner = owner;
    m_EventData.u_Anim.m_Coord = coord;
}

void GameEventClass::Execute()
{
    BOOL formation_event = false;
    char message[100] = { '\0' };
    char *messageptr = nullptr;
    // EVENT_DEBUG_LOG("GameEventClass::Execute() - Executing '%s' event (GameFrame:%d, EventFrame:%d, ID:%d).",
    //    Name_From(m_Type),
    //    g_GameFrame,
    //    m_EventFrame,
    //    m_HouseID);
    FootClass *whom = nullptr;

    switch (m_Type) {
        case EVENT_EMPTY:
            break;

        // verified
        case EVENT_ALLY: {
            EVENT_DEBUG_LOG("    ALLY - House:%d.", m_EventData.u_House.m_Whom);
            if (g_Houses[m_HouseID].Is_Ally((HousesType)m_EventData.u_House.m_Whom)) {
                g_Houses[m_HouseID].Make_Enemy((HousesType)m_EventData.u_House.m_Whom);
            } else {
                g_Houses[m_HouseID].Make_Ally((HousesType)m_EventData.u_House.m_Whom);
            }
            break;
        }

        // verified
        case EVENT_MEGAMISSION_F: {
            EVENT_DEBUG_LOG(
                "    MEGAMISSION_F - Whom:%08X, Mission:%s, Target:%08X\n"
                "                    Dest:%08X, FormSpeed:%d, FormMaxSpeed:%d.",
                m_EventData.u_MegaMissionF.m_Whom,
                MissionClass::Name_From(m_EventData.u_MegaMissionF.m_Mission),
                m_EventData.u_MegaMissionF.m_Target,
                m_EventData.u_MegaMissionF.m_Dest,
                m_EventData.u_MegaMissionF.m_FormSpeed,
                m_EventData.u_MegaMissionF.m_FormMaxSpeed);
            whom = (FootClass *)m_EventData.u_MegaMissionF.m_Whom.As_Techno();
            if (whom != nullptr && whom->Is_Active() && whom->Is_Foot()) {
                whom->Set_In_Formation(true);
                whom->Set_Team_Speed(m_EventData.u_MegaMissionF.m_FormSpeed);
                whom->Set_Team_Max_Speed(m_EventData.u_MegaMissionF.m_FormMaxSpeed);
                g_FormMove = true;
                g_FormSpeed = m_EventData.u_MegaMissionF.m_FormSpeed;
                g_FormMaxSpeed = m_EventData.u_MegaMissionF.m_FormMaxSpeed;
                formation_event = true;
            }
            // fallthough
        }

        case EVENT_MEGAMISSION: {
            EVENT_DEBUG_LOG("    MEGAMISSION - Whom:%08X, Mission:%s, Target:%08X, Dest:%08X.",
                m_EventData.u_MegaMission.m_Whom,
                MissionClass::Name_From(m_EventData.u_MegaMission.m_Mission),
                m_EventData.u_MegaMission.m_Target,
                m_EventData.u_MegaMission.m_Dest);
            FootClass *whom = (FootClass *)m_EventData.u_MegaMission.m_Whom.As_Techno();

            if (whom != nullptr) {
                if (whom->Is_Active() && whom->Get_Health() > 0 && !whom->In_Limbo()) {
                    ObjectClass *target = nullptr;

                    if (m_EventData.u_MegaMission.m_Target.Valid_RTTI()) {
                        target = m_EventData.u_MegaMission.m_Target.As_Object();
                        if (target != nullptr) {
                            if (!target->Is_Active() || !target->Get_Health() || target->In_Limbo()) {
                                break;
                            }
                        }
                    }
                    if (m_EventData.u_MegaMission.m_Dest.Valid_RTTI()) {
                        target = m_EventData.u_MegaMission.m_Dest.As_Object();
                        if (target != nullptr) {
                            if (!target->Is_Active() || !target->Get_Health() || target->In_Limbo()) {
                                break;
                            }
                        }
                    }

                    if (!whom->Is_Tethered()) {
                        whom->Transmit_Message(RADIO_OVER_AND_OUT);
                    }

                    if (whom->Is_Foot()) {
                        if (formation_event == false) {
                            whom->Set_In_Formation(false);
                        }

                        if (whom->Get_Team() != nullptr) {
                            whom->Get_Team()->Remove(whom, -1);
                        }
                    }
                    if (target != nullptr) {
                        if (g_PlayerPtr->Is_Ally(whom)) {
                            target->Clicked_As_Target(7);
                        }
                    }

                    whom->Assign_Mission(m_EventData.u_MegaMission.m_Mission);
                    if (whom->Is_Foot()) {
                        whom->Set_Suspended_NavCom(0);
                    }
                    whom->Set_Suspended_TarCom(0);

                    if (m_EventData.u_MegaMission.m_Mission == MISSION_AREA_GUARD && whom->Is_Foot()) {
                        whom->Assign_Target(0);
                        whom->Assign_Destination(m_EventData.u_MegaMission.m_Target);
                        whom->Set_Archive(m_EventData.u_MegaMission.m_Target);

                    } else if (m_EventData.u_MegaMission.m_Mission == MISSION_QMOVE && whom->Is_Foot()) {
                        whom->Queue_Navigation_List(m_EventData.u_MegaMission.m_Dest);

                    } else {
                        if (whom->Is_Foot()) {
                            whom->Clear_Navigation_List();
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
                }
            }
            break;
        }

        // verified
        case EVENT_IDLE: {
            EVENT_DEBUG_LOG("    IDLE - Whom:%08X.", m_EventData.u_Whom.m_Whom);
            whom = (FootClass *)m_EventData.u_Whom.m_Whom.As_Techno();
            if (whom != nullptr && whom->Is_Active() && !whom->In_Limbo() && !whom->Is_Tethered()
                && whom->What_Am_I() != RTTI_BUILDING) {
                whom->Transmit_Message(RADIO_OVER_AND_OUT);
                whom->Assign_Destination(0);
                whom->Assign_Target(0);
                whom->Enter_Idle_Mode();
                if (whom->Is_Foot()) {
                    whom->Clear_Navigation_List();
                }
            }
            break;
        }

        // verified
        case EVENT_SCATTER: {
            EVENT_DEBUG_LOG("    SCATTER - Whom:%08X.", m_EventData.u_Whom.m_Whom);
            whom = (FootClass *)m_EventData.u_Whom.m_Whom.As_Techno();
            if (whom != nullptr && whom->Is_Foot() && whom->Is_Active() && !whom->In_Limbo() && !whom->Is_Tethered()) {
                whom->Set_To_Scatter(true);
                whom->Scatter(0, 1, 0);
            }
            break;
        }

        // verified
        case EVENT_DESTRUCT: {
            EVENT_DEBUG_LOG("    DESTRUCT.");
            g_Houses[m_HouseID].Flag_To_Die();
            break;
        }

        // verified
        // not handled by the game
        case EVENT_DEPLOY: {
            EVENT_DEBUG_LOG("    DEPLOY.");
            break;
        }

        // verified
        case EVENT_PLACE: {
            EVENT_DEBUG_LOG("    PLACE - RTTI:%d Cell: X:%d Y:%d.",
                m_EventData.u_RTTICell.m_RTTI,
                Cell_Get_X(m_EventData.u_RTTICell.m_Cell),
                Cell_Get_Y(m_EventData.u_RTTICell.m_Cell));
            g_Houses[m_HouseID].Place_Object(m_EventData.u_RTTICell.m_RTTI, m_EventData.u_RTTICell.m_Cell);
            break;
        }

        // verified
        case EVENT_OPTIONS: {
            EVENT_DEBUG_LOG("    OPTIONS.");
            if (!g_Session.Playback_Game()) {
                g_SpecialDialog = SPECIAL_DLG_OPTIONS;
            }
            break;
        }

        // verified
        case EVENT_GAMESPEED: {
            EVENT_DEBUG_LOG("    GAMESPEED - GameSpeed:%d.", m_EventData.u_GameSpeed.m_GameSpeed);
            g_Options.Set_Game_Speed(m_EventData.u_GameSpeed.m_GameSpeed);
            break;
        }

        // verified
        case EVENT_PRODUCE: {
            EVENT_DEBUG_LOG(
                "    PRODUCE - RTTI:%d HeapID:%d.", m_EventData.u_RTTIHeapID.m_RTTI, m_EventData.u_RTTIHeapID.m_HeapID);
            g_Houses[m_HouseID].Begin_Production(m_EventData.u_RTTIHeapID.m_RTTI, m_EventData.u_RTTIHeapID.m_HeapID);
            break;
        }

        // verified
        case EVENT_SUSPEND: {
            EVENT_DEBUG_LOG("    SUSPEND - RTTI:%d.", m_EventData.u_RTTI.m_RTTI);
            g_Houses[m_HouseID].Suspend_Production(m_EventData.u_RTTI.m_RTTI);
            break;
        }

        // verified
        case EVENT_ABANDON: {
            EVENT_DEBUG_LOG("    ABANDON - RTTI:%d.", m_EventData.u_RTTI.m_RTTI);
            g_Houses[m_HouseID].Abandon_Production(m_EventData.u_RTTI.m_RTTI);
            break;
        }

        // verified
        case EVENT_PRIMARY: {
            EVENT_DEBUG_LOG("    PRIMARY - Whom:%08X.", m_EventData.u_Whom.m_Whom);
            BuildingClass *whomb = (BuildingClass *)m_EventData.u_Whom.m_Whom.As_Building();
            if (whomb != nullptr && whomb->Is_Active()) {
                whomb->Toggle_Primary();
            }
            break;
        }

        // verified
        case EVENT_SPECIAL_PLACE: {
            EVENT_DEBUG_LOG("    SPECIAL_PLACE - Special Weapon:%d Cell: X:%d Y:%d.",
                m_EventData.u_SpecialPlace.m_SpecialWeapon,
                Cell_Get_X(m_EventData.u_SpecialPlace.m_Cell),
                Cell_Get_Y(m_EventData.u_SpecialPlace.m_Cell));
            g_Houses[m_HouseID].Place_Special_Blast(
                (SpecialWeaponType)m_EventData.u_SpecialPlace.m_SpecialWeapon, m_EventData.u_SpecialPlace.m_Cell);
            break;
        }

        // verified
        case EVENT_EXIT: {
            EVENT_DEBUG_LOG("    EXIT.");
            g_Theme.Fade_Out();
            Stop_Speaking();
            Speak(VOX_BATTLE_CONTROL_TERMINATED);
            while (Is_Speaking()) {
                Call_Back();
            }
            g_GameActive = false;
            break;
        }

        // verified
        case EVENT_ANIMATION: {
            EVENT_DEBUG_LOG("    ANIMATION - Anim:%s, Coord: X:%d Y:%d.",
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

        // verified
        case EVENT_REPAIR: {
            EVENT_DEBUG_LOG("    REPAIR - Whom:%08X.", m_EventData.u_Whom.m_Whom);
            whom = (FootClass *)m_EventData.u_Whom.m_Whom.As_Techno();
            if (whom != nullptr && whom->Is_Active()) {
                whom->Repair();
            }
            break;
        }

        // verified
        case EVENT_SELL: {
            EVENT_DEBUG_LOG("    SELL - Whom:%08X.", m_EventData.u_Whom.m_Whom);
            whom = (FootClass *)m_EventData.u_Whom.m_Whom.As_Techno();
            if (whom != nullptr && whom->Is_Active()) {
                if (&g_Houses[m_HouseID] == whom->Get_Owner_House()) {
                    if ((whom->What_Am_I() == RTTI_BUILDING || whom->What_Am_I() == RTTI_UNIT)
                        && g_Map[whom->Center_Cell()].Cell_Building() != nullptr) {
                        whom->Sell_Back(-1);
                    }
                }
            }
            break;
        }

        // verified
        case EVENT_SELL_CELL: {
            EVENT_DEBUG_LOG("    SELL_CELL - Cell: X:%d Y:%d.",
                Cell_Get_X(m_EventData.u_Cell.m_Cell),
                Cell_Get_Y(m_EventData.u_Cell.m_Cell));
            g_Houses[m_HouseID].Sell_Wall(m_EventData.u_Cell.m_Cell);
            break;
        }

        // verified
        case EVENT_SPECIAL: {
            EVENT_DEBUG_LOG("    SPECIAL.");
            s_Special.Set(m_EventData.u_Special.m_Special);
            sprintf(message, Text_String(TXT_SPECIAL_WARNING), g_Houses[m_HouseID].Get_Name());
            g_Session.Get_Messages().Add_Message(
                nullptr, 0, message, g_Houses[m_HouseID].Get_Color(), TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_OUTLINE, 1200);
            g_Map.Flag_To_Redraw();
            break;
        }

        // verified
        // not handled by the game
        case EVENT_FRAME_SYNC: {
            EVENT_DEBUG_LOG("    FRAME_SYNC.");
            break;
        }

        // verified
        // not handled by the game
        case EVENT_MESSAGE: {
            EVENT_DEBUG_LOG("    MESSAGE.");
            break;
        }

        // verified
        case EVENT_RESPONSE_TIME: {
            EVENT_DEBUG_LOG(
                "    RESPONSE_TIME - Changing MaxAhead to frames %d.", m_EventData.u_ResponseTime.m_MaxAhead);
            g_Session.Set_MaxAhead(m_EventData.u_ResponseTime.m_MaxAhead);
            break;
        }

        // verified
        // not handled by the game
        case EVENT_FRAME_INFO: {
            EVENT_DEBUG_LOG("    FRAME_INFO.");
            break;
        }

        // verified
        case EVENT_SAVE_GAME: {
            EVENT_DEBUG_LOG("    SAVE_GAME.");
            if (g_SpecialDialog != SPECIAL_DLG_NONE) {
                Save_Game(-1, Text_String(TXT_MULTIPLAYER_GAME), 0);
            } else {
                TCountDownTimerClass<SystemTimerClass> timer(60);
                MessageBoxClass msg;
                msg.Process(TXT_SAVING_GAME);
                Save_Game(-1, Text_String(TXT_MULTIPLAYER_GAME), 0);
                while (timer.Time() > 0) {
                    Call_Back();
                }
                g_HidPage.Clear();
                g_Map.Flag_To_Redraw(true);
                g_Map.Render();
            }
            break;
        }

        // verified
        // todo test with conyard deploy undeploy
        case EVENT_ARCHIVE: {
            EVENT_DEBUG_LOG(
                "    ARCHIVE - Whom:%08X, Target:%08X.", m_EventData.u_WhomTarget.m_Whom, m_EventData.u_WhomTarget.m_Target);
            whom = (FootClass *)m_EventData.u_WhomTarget.m_Whom.As_Techno();
            if (whom != nullptr && whom->Is_Active()) {
                whom->Set_Archive(m_EventData.u_WhomTarget.m_Target);
            }
            break;
        }

        // verified
        case EVENT_ADD_PLAYER: {
            EVENT_DEBUG_LOG("    ADD_PLAYER.");
            int *ptr = m_EventData.u_AddPlayer.m_pointer;
            // BUGFIX, original assumed the pointer is always valid
            if (ptr == nullptr) {
                captainslog_debug("EVENT_ADD_PLAYER got a invalid pointer!");
                break;
            }

            for (int i = 0; i < m_EventData.u_AddPlayer.m_uintval; ++i) {
                EVENT_DEBUG_LOG("%d\n", ptr[i]);
            }

            if (m_HouseID != g_PlayerPtr->Get_Heap_ID()) {
                delete m_EventData.u_AddPlayer.m_pointer;
            }
            break;
        }

        // verified
        case EVENT_TIMING: {
            EVENT_DEBUG_LOG("    TIMING - DesiredFrameRate:%d MaxAhead:%d.",
                m_EventData.u_Timing.m_DesiredFrameRate,
                m_EventData.u_Timing.m_MaxAhead);
            if (m_EventData.u_Timing.m_MaxAhead < g_Session.Get_MaxAhead()) {
                g_NewMaxAheadFrame1 = m_EventData.u_Timing.m_DesiredFrameRate;
                g_NewMaxAheadFrame2 = m_EventData.u_Timing.m_MaxAhead + g_NewMaxAheadFrame1;
            }
            g_Session.Set_Desired_Frame_Rate(m_EventData.u_Timing.m_DesiredFrameRate);
            g_Session.Set_MaxAhead(m_EventData.u_Timing.m_MaxAhead);
            break;
        }

        case EVENT_PROCESS_TIME: {
            EVENT_DEBUG_LOG("    PROCESS_TIME - %04x ticks.", m_EventData.u_ProcessTime.m_Ticks);
            for (int i = 0; i < g_Session.Player_Count(); ++i) {
                NodeNameTag *nodename = g_Session.Player(i);

                if (nodename == nullptr) {
                    captainslog_debug("EVENT_PROCESS_TIME got a invalid pointer! This should never happen!");
                    break;
                }

                if (m_HouseID == nodename->m_AltHouse) {
                    nodename->field_19 = m_EventData.u_ProcessTime.m_Ticks;
                    break;
                }
            }
            break;
        }

        case EVENT_PROPOSE_DRAW: {
            EVENT_DEBUG_LOG("    PROPOSE_DRAW.");
            if (m_HouseID != g_PlayerPtr->Get_Heap_ID()) {
                if (g_Scen.Get_field_7CF()) {
                    g_Scen.Set_field_7D3(true);
                    break;
                }
                for (int i = 0; i < g_Session.Player_Count(); ++i) {
                    if (m_HouseID == g_Session.Player(i)->m_AltHouse) {
                        g_Scen.Set_field_7D3(true);
                        sprintf(message, "%s has proposed that the game be declared a draw.", g_Session.Player(i)->m_Name);
                        messageptr = message;
                        break;
                    }
                }
            } else {
                if (g_Scen.Get_field_7D3()) {
                    g_Scen.Set_field_7CF(true);
                    break;
                }
                g_Scen.Set_field_7CF(true);
                messageptr = "You have proposed that the game be declared a draw.";
            }
            g_Session.Get_Messages().Add_Message(0,
                0,
                messageptr,
                PLAYER_COLOR_YELLOW,
                TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL,
                900 * g_Rule.Message_Delay());
            Sound_Effect(VOC_INCOMING_MESSAGE);
            break;
        }

        case EVENT_RETRACT_DRAW: {
            EVENT_DEBUG_LOG("    RETRACT_DRAW.");

            if (m_HouseID == g_PlayerPtr->Get_Heap_ID()) {
                g_Scen.Set_field_7CF(false);
                messageptr = "You have retracted your offer of a draw.";
            } else {
                for (int i = 0; i < g_Session.Player_Count(); ++i) {
                    if (m_HouseID == g_Session.Player(i)->m_AltHouse) {
                        sprintf(message, "%s has retracted the offer of a draw.", g_Session.Player(i)->m_Name);
                        break;
                    }
                }
                g_Scen.Set_field_7D3(false);
                messageptr = message;
            }
            g_Session.Get_Messages().Add_Message(0,
                0,
                messageptr,
                PLAYER_COLOR_YELLOW,
                TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL,
                900 * g_Rule.Message_Delay());
            Sound_Effect(VOC_INCOMING_MESSAGE);
            break;
        }
        default:
            break;
    }
    EVENT_DEBUG_LOG("\n");
    // EVENT_DEBUG_LOG("EventClass::Execute - Complete\n");
}

char const *GameEventClass::Name_From(GameEventClass::GameEventType type)
{
    captainslog_assert(type < EVENT_COUNT);

    if (type != EVENT_EMPTY && type < EVENT_COUNT) {
        return EventTypeList[type].Name;
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
