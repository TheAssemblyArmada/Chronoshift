/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Classes holding action information for the trigger system.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "taction.h"
#include "aircraft.h"
#include "anim.h"
#include "building.h"
#include "coord.h"
#include "gbuffer.h"
#include "globals.h"
#include "house.h"
#include "infantry.h"
#include "iomap.h"
#include "mouse.h"
#include "movie.h"
#include "object.h"
#include "palette.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "target.h"
#include "teamtype.h"
#include "theme.h"
#include "tracker.h"
#include "trigger.h"
#include "triggertype.h"
#include "unit.h"
#include "voc.h"
#include "vox.h"
#include <algorithm>
#include <cstdio>

using std::sprintf;

TActionClass::ActionTextStruct TActionClass::s_ActionText[TACTION_COUNT] = {
    { "-No Action-",
        "This is a null action. It will do nothing and is equivalent to not having an action at all. Why use it?" },
    { "Winner is...",
        "The winner will be forced to be the house specified. The game will end immediately. Typically, the player's house "
        "is specified." },
    { "Loser is...",
        "The loser will be force to be the house specified. The game will end immediately. Typically, the player's house is "
        "specified." },
    { "Production Begins", "The computer's house (as specified) will begin production of units and structures." },
    { "Create Team...",
        "Creates a team of the type specified (owned by the house of this trigger). The team member are NOT automatically "
        "created however." },
    { "Destroy All Teams",
        "Destroys all instances of the team type specified. The units in those existing teams will remain and be available "
        "for recruiting into other teams." },
    { "All to Hunt...",
        "Forces all units, of the house specified, into 'hunt' mode. They will seek out and destroy their enemies." },
    { "Reinforcement (team)...",
        "Create a reinforcement of the specified team. The members of the team WILL be created magically by this action." },
    { "Drop Zone Flare (waypoint)...",
        "Display a drop zone flair at the waypoint specified. The map will also be reaveald around that location." },
    { "Fire Sale...",
        "Cause all buildings of the specified house to be sold (for cash and prizes). Typically this is used in the final "
        "assault by the computer." },
    { "Play Movie...",
        "Displays the specified movie (full screen). The game is paused while this occurs and resumes normally after it "
        "completes." },
    { "Text Trigger (ID num)...", "Display the text identified by the string file <label>." },
    { "Destroy Trigger...",
        "Destroy all current instances of the trigger type specified. This does not prevent future instances of that "
        "trigger >from being created." },
    { "Autocreate Begins...",
        "Initiates autocreate for the house specified. This will cause the computer's house to build autocreate teams as it "
        "sees fit." },
    { "Change House...", "Changes owning house to the one specified for attached objects." },
    { "Allow Win",
        "Removes one 'blockage' from allowing the player to win. The blockage number is equal the number of triggers "
        "created that have this action." },
    { "Reveal all map", "Reveals the entire map to the player." },
    { "Reveal around waypoint...", "Reveals a region of the map to the player around the waypoint specified." },
    { "Reveal zone of waypoint...",
        "Reveals all cells that share the same zone as the waypoing specified. This yields some wierd results. Use with "
        "caution." },
    { "Play sound effect...", "Plays the sound effect specified." },
    { "Play music theme...", "Plays the music theme specified." },
    { "Play speech...", "Plays the speech sound specified." },
    { "Force Trigger...",
        "Force all triggers of this specified type to spring regardless of what it's event flags may indicate." },
    { "Timer Start", "Start the global mission timer." },
    { "Timer Stop", "Stop the global mission timer." },
    { "Timer Extend (1/10th min)...", "Extend the global mission timer by the time specified." },
    { "Timer Shorten (1/10th min)...",
        "Shorten the global mission timer by the time specified. It can never be reduced below 'zero' time." },
    { "Timer Set (1/10th min)...", "Set the global mission timer to the value specified." },
    { "Global Set...",
        "Set the global flag. Global flags are named in the file Globals.INI. Global flags can be either 'true' or "
        "'false'." },
    { "Global Clear...",
        "Clear the global flag. Global flags are named in the file Globals.INI. Global flags can either be 'true' or "
        "'false'." },
    { "Auto Base Building...",
        "Initialize the computer skirmish mode build control to either 'on' or 'off' state. When 'on', the computer takes "
        "over as if it were in skirmish mode. (gs make sure he has a con yard)." },
    { "Grow shroud one 'step'", "Increase the shroud darkness by one step (cell)." },
    { "Destroy attached building", "Destroy any buildings, bridges, or units that this trigger is attached to." },
    { "Add 1-time special weapon...", "Add a one-shot special weapon (as indicated) to the trigger's house." },
    { "Add repeating special weapon...", "Add a permanent special weapon (as indicated) to the trigger's house." },
    { "Preferred target...",
        "Specify what the trigger's house should use as its preferred target when using special weapon attacks." },
    { "Launch Nukes", "<TODO>" }
};

/**
 * Operator for processing a trigger action.
 */
BOOL TActionClass::operator()(HousesType house, ObjectClass *object, int trigger_id, cell_t cell_num)
{
    HouseClass *hp = HouseClass::As_Pointer(house);
    TriggerClass *tp = nullptr;
    ObjectClass *op = object;
    BOOL ret = true;

    if (trigger_id != -1) {
        TriggerClass *tp = &g_Triggers[trigger_id];
    }

    // op is null if object isn't active.
    if (object != nullptr && !object->Is_Active()) {
        op = nullptr;
    }

    switch (m_Type) {
        case TACTION_WINNER_IS:
            if (g_PlayerPtr->What_Type() != (HousesType)m_IntegerValue) {
                g_PlayerPtr->Flag_To_Lose();
            } else {
                g_PlayerPtr->Flag_To_Win();
            }

            return true;
        case TACTION_LOSER_IS:
            if (g_PlayerPtr->What_Type() == (HousesType)m_IntegerValue) {
                g_PlayerPtr->Flag_To_Lose();
            } else {
                g_PlayerPtr->Flag_To_Win();
            }

            return true;
        case TACTION_PROD_BEGIN:
            if ((HousesType)m_IntegerValue != HOUSES_NONE) {
                HouseClass::As_Pointer((HousesType)m_IntegerValue)->Set_Production(true);
            }

            return true;
        case TACTION_CREATE_TEAM:
            ++ScenarioInit;
            m_TeamType->Create_One_Of();
            --ScenarioInit;

            return true;
        case TACTION_DESTROY_ALL_TEAMS:
            m_TeamType->Destroy_All_Of();

            return true;
        case TACTION_ALL_TO_HUNT:
            HouseClass::As_Pointer((HousesType)m_IntegerValue)->Do_All_To_Hunt();

            return true;
        case TACTION_REINFORCE_TEAM:
            return m_TeamType->Do_Reinforcements();

        case TACTION_DROP_FLARE:
            new AnimClass(ANIM_LZ_SMOKE, Cell_To_Coord(Scen.Get_Waypoint(m_IntegerValue)));

            return true;
        case TACTION_FIRE_SALE:
            if ((HousesType)m_IntegerValue != HOUSES_NONE) {
                HouseClass::As_Pointer((HousesType)m_IntegerValue)->Set_Smarties(URGENCY_FIRE_SALE);
            }

            return true;
        case TACTION_PLAY_MOVIE:
            g_wwmouse->Hide_Mouse();
            g_seenBuff.Clear();
            Play_Movie((MovieType)m_IntegerValue);
            GamePalette.Set();
            Map.Flag_To_Redraw(true);
            g_wwmouse->Show_Mouse();

            return true;
        case TACTION_DISPLAY_TEXT:
            Session.Get_Messages().Add_Message(nullptr,
                0,
                g_TutorialText[m_IntegerValue],
                PLAYER_COLOR_GREEN,
                TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL,
                900 * Rule.Message_Delay());

            return true;
        case TACTION_DESTROY_TRIGGER:
            if (m_TriggerType != nullptr) {
                for (int i = 0; i < g_Triggers.Count(); ++i) {
                    if (&g_Triggers[i].Class_Of() == m_TriggerType) {
                        Detach_This_From_All(m_TriggerType->As_Target(), true);
                        delete &g_Triggers[i];
                    }
                }
            }

            return true;
        case TACTION_ENABLE_AUTOCREATE:
            if ((HousesType)m_IntegerValue != HOUSES_NONE) {
                HouseClass::As_Pointer((HousesType)m_IntegerValue)->Set_Autocreate(true);
            }

            return true;
        case TACTION_CHANGE_HOUSE: // This one is flagged as don't use in editor and doesn't work.
        case TACTION_ALLOW_WIN:
            break;
        case TACTION_REVEAL_MAP:
            if (!g_PlayerPtr->Visionary()) {
                g_PlayerPtr->Set_Visionary(true);

                for (cell_t i = 0; i < MAP_MAX_AREA; ++i) {
                    Map.Map_Cell(i, g_PlayerPtr);
                }
            }

            return true;
        case TACTION_REVEAL_WAYPOINT:
            if (!g_PlayerPtr->Visionary()) {
                // TODO gap radius is used here, but really it should be a separate constant as in later games.
                Map.Sight_From(Scen.Get_Waypoint(m_IntegerValue), Rule.Gap_Radius(), g_PlayerPtr, false);
            }

            return true;
        case TACTION_REVEAL_WAYPOINT_ZONE:
            if (!g_PlayerPtr->Visionary()) {
                uint8_t zone = Map[Scen.Get_Waypoint(m_IntegerValue)].Get_Zone(MZONE_CRUSHER);

                for (cell_t i = 0; i < MAP_MAX_AREA; ++i) {
                    if (Map[i].Get_Zone(MZONE_CRUSHER) == zone) {
                        Map.Map_Cell(i, g_PlayerPtr);
                    }
                }
            }

            return true;
        case TACTION_PLAY_SOUND_EFFECT:
            Sound_Effect((VocType)m_IntegerValue);

            return true;
        case TACTION_PLAY_MUSIC_THEME:
            Theme.Queue_Song((ThemeType)m_IntegerValue);

            return true;
        case TACTION_PLAY_SPEECH:
            Speak((VoxType)m_IntegerValue);

            return true;
        case TACTION_FORCE_TRIGGER:
            if (m_TriggerType != nullptr) {
                m_TriggerType->Find_Or_Make()->Spring(TEVENT_ANY, nullptr, 0, true);
            }

            return true;
        case TACTION_TIMER_START:
            if (!Scen.Global_Timer_Running()) {
                Scen.Start_Global_Timer();
                Map.Set_Tab_Redraw(true);
                Map.Flag_To_Redraw();
            }

            return true;
        case TACTION_TIMER_STOP:
            if (Scen.Global_Timer_Running()) {
                Scen.Stop_Global_Timer();
                Map.Set_Tab_Redraw(true);
                Map.Flag_To_Redraw();
            }

            return true;
        case TACTION_TIMER_EXTEND:
            Scen.Set_Global_Time(Scen.Get_Global_Time() + 90 * m_IntegerValue);
            Map.Set_Tab_Redraw(true);
            Map.Flag_To_Redraw();

            return true;
        case TACTION_TIMER_REDUCE:
            Scen.Set_Global_Time(std::max(Scen.Get_Global_Time() - 90 * m_IntegerValue, 0));
            Map.Set_Tab_Redraw(true);
            Map.Flag_To_Redraw();

            return true;
        case TACTION_TIMER_SET:
            Scen.Set_Global_Time(90 * m_IntegerValue);
            Map.Set_Tab_Redraw(true);
            Map.Flag_To_Redraw();

            return true;
        case TACTION_GLOBAL_SET:
            Scen.Set_Global_To(m_IntegerValue, true);

            return true;
        case TACTION_GLOBAL_CLEAR:
            Scen.Set_Global_To(m_IntegerValue, false);

            return true;
        case TACTION_AUTO_BASE_AI:
            hp->Set_Auto_Base_AI(m_IntegerValue != 0);

            return true;
        case TACTION_GROW_SHROUD:
            Map.Encroach_Shadow();

            return true;
        case TACTION_DESTROY_ATTACHED:
            if (op != nullptr) {
                int dmg = op->Get_Health();
                op->Take_Damage(dmg, 0, WARHEAD_AP, nullptr, true);
            } else {
                ret = false;
            }

            if (cell_num != 0) {
                Map.Destroy_Bridge_At(cell_num);
            }

            if (tp != nullptr) {
                for (int i = 0; i < g_Units.Count(); ++i) {
                    if (&g_Units[i] != nullptr && g_Units[i].Get_Attached_Trigger() == tp) {
                        g_Units[i].Invalidate_Attached_Trigger();
                        int dmg = g_Units[i].Get_Health();
                        g_Units[i].Take_Damage(dmg, 0, WARHEAD_AP, nullptr, true);
                    }
                }

                for (int i = 0; i < g_Infantry.Count(); ++i) {
                    if (&g_Infantry[i] != nullptr && g_Infantry[i].Get_Attached_Trigger() == tp) {
                        g_Infantry[i].Invalidate_Attached_Trigger();
                        int dmg = g_Infantry[i].Get_Health();
                        g_Infantry[i].Take_Damage(dmg, 0, WARHEAD_AP, nullptr, true);
                    }
                }

                for (int i = 0; i < g_Aircraft.Count(); ++i) {
                    if (&g_Aircraft[i] != nullptr && g_Aircraft[i].Get_Attached_Trigger() == tp) {
                        g_Aircraft[i].Invalidate_Attached_Trigger();
                        int dmg = g_Aircraft[i].Get_Health();
                        g_Aircraft[i].Take_Damage(dmg, 0, WARHEAD_AP, nullptr, true);
                    }
                }

                for (int i = 0; i < g_Buildings.Count(); ++i) {
                    if (&g_Buildings[i] != nullptr && g_Buildings[i].Get_Attached_Trigger() == tp) {
                        g_Buildings[i].Invalidate_Attached_Trigger();
                        int dmg = g_Buildings[i].Get_Health();
                        g_Buildings[i].Take_Damage(dmg, 0, WARHEAD_AP, nullptr, true);
                    }
                }

                // TODO Again the original code looks bugged related to vessels as they will not be destroyed.
            }

            return ret;
        case TACTION_ONE_TIME_SUPER:
        case TACTION_REPEATING_SUPER:
            hp->Enable_Superweapon((SpecialWeaponType)m_IntegerValue, m_Type == TACTION_ONE_TIME_SUPER);

            if (hp == g_PlayerPtr) {
                Map.Add(RTTI_SPECIAL, (SpecialWeaponType)m_IntegerValue);
                Map.Flag_Strip_Redraw(COLUMN_RIGHT);
            }

            return true;
        case TACTION_PREFERRED_TARGET:
            if (hp != nullptr) {
                hp->Set_Preferred_Target((QuarryType)m_IntegerValue);
            }

            return true;
        case TACTION_LAUNCH_NUKES:
            for (int i = 0; i < g_Buildings.Count(); ++i) {
                if (g_Buildings[i].What_Type() == BUILDING_MSLO) {
                    g_Buildings[i].Assign_Mission(MISSION_MISSILE);
                }
            }

            return true;
        default:
            break;
    }

    return true;
}

/**
 * Detaches teamtype and triggertype targets from this action.
 */
void TActionClass::Detach(target_t target)
{
    if (Target_Get_RTTI(target) == RTTI_TEAMTYPE) {
        if (m_TeamType == As_TeamType(target)) {
            m_TeamType = nullptr;
        }
    }

    if (Target_Get_RTTI(target) == RTTI_TRIGGERTYPE) {
        if (m_TriggerType == As_TriggerType(target)) {
            m_TriggerType = nullptr;
        }
    }
}

/**
 * Builds an action part of a trigger string for an ini entry. Uses NewINIFormat=3 format.
 */
void TActionClass::Build_INI_Entry(char *entry_buffer) const
{
    sprintf(entry_buffer,
        "%d,%d,%d,%d",
        m_Type,
        g_TeamTypes.Logical_ID(m_TeamType),
        g_TriggerTypes.Logical_ID(m_TriggerType),
        m_IntegerValue);
}

/**
 * Reads the action part of a trigger string from an ini entry.
 */
void TActionClass::Read_INI()
{
    switch (g_iniFormat) {
        case INIFORMAT_0:
        case INIFORMAT_1:
            m_Type = (TActionType)atoi(strtok(nullptr, ","));
            m_TeamType = TeamTypeClass::From_Name(strtok(nullptr, ","));
            // Original code appears to do this which is abusing pointer as int.
            // Bad juju and not portable to 64bit. Fortunately no maps use this format.
            m_TriggerType.Set_ID((uintptr_t)strdup(strtok(nullptr, ",")));
            m_IntegerValue = atoi(strtok(nullptr, ","));
            break;

        default: // Format 2 and upwards.
            m_Type = (TActionType)atoi(strtok(nullptr, ","));
            m_TeamType.Set_ID(atoi(strtok(nullptr, ",")));
            m_TriggerType.Set_ID(atoi(strtok(nullptr, ",")));
            m_IntegerValue = atoi(strtok(nullptr, ","));

            break;
    };
}

/**
 * Gets the action type from the action's name.
 */
TActionType TActionClass::Action_From_Name(const char *name)
{
    for (TActionType i = TACTION_FIRST; i < TACTION_COUNT; ++i) {
        if (strcmp(name, s_ActionText[i].m_Name) == 0) {
            return i;
        }
    }

    return TACTION_NO_ACTION;
}

/**
 * Gets the action's name from the action type.
 */
const char *TActionClass::Name_From_Action(TActionType taction)
{
    return s_ActionText[taction].m_Name;
}

/**
 * Determines what the action type needs to be provided with.
 */
NeedType TActionClass::Action_Needs(TActionType taction)
{
    switch (taction) {
        case TACTION_WINNER_IS:
        case TACTION_LOSER_IS:
        case TACTION_PROD_BEGIN:
        case TACTION_ALL_TO_HUNT:
        case TACTION_FIRE_SALE:
        case TACTION_ENABLE_AUTOCREATE:
            return NEED_HOUSE;

        case TACTION_CREATE_TEAM:
        case TACTION_DESTROY_ALL_TEAMS:
        case TACTION_REINFORCE_TEAM:
            return NEED_TEAM;

        case TACTION_DROP_FLARE:
        case TACTION_REVEAL_WAYPOINT:
        case TACTION_REVEAL_WAYPOINT_ZONE:
            return NEED_WAYPOINT;

        case TACTION_PLAY_MOVIE:
            return NEED_MOVIE;

        case TACTION_DISPLAY_TEXT:
        case TACTION_TIMER_EXTEND:
        case TACTION_TIMER_REDUCE:
        case TACTION_TIMER_SET:
        case TACTION_GLOBAL_SET:
        case TACTION_GLOBAL_CLEAR:
            return NEED_NUMBER;

        case TACTION_DESTROY_TRIGGER:
        case TACTION_FORCE_TRIGGER:
            return NEED_TRIGGER;

        case TACTION_PLAY_SOUND_EFFECT:
            return NEED_SOUND;

        case TACTION_PLAY_MUSIC_THEME:
            return NEED_THEME;

        case TACTION_PLAY_SPEECH:
            return NEED_SPEECH;

        case TACTION_AUTO_BASE_AI:
            return NEED_BOOL;

        case TACTION_ONE_TIME_SUPER:
        case TACTION_REPEATING_SUPER:
            return NEED_SPECIAL;

        case TACTION_PREFERRED_TARGET:
            return NEED_QUARRY;

        default:
            break;
    }

    return NEED_NOTHING;
}
