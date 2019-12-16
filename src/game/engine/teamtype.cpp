/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief TeamTypes used to hold collections of units for reinforcements and such.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "teamtype.h"
#include "aircrafttype.h"
#include "gadget.h"
#include "gameini.h"
#include "gbuffer.h"
#include "infantrytype.h"
#include "objecttype.h"
#include "scenario.h"
#include "target.h"
#include "team.h"
#include "triggertype.h"
#include "unittype.h"
#include "vesseltype.h"
#include <cstdio>
#include <cstring>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using std::sprintf;
using std::snprintf;
using std::strlen;
using std::strtok;
using std::memset;

#ifndef GAME_DLL
TFixedIHeapClass<TeamTypeClass> g_TeamTypes;
#endif

TeamTypeClass::TMissionInfoStruct TeamTypeClass::s_TMissions[TMISSION_COUNT] = {
    { "Attack...", "", "QuarryType" }, 
    { "Attack Waypoint...", "", "Waypoint" },
    { "Change Formation to...", "", "FormationType" },
    {"Move to waypoint...", "", "Waypoint" },
    { "Move to Cell...", "", "Cell" },
    { "Guard area (1/10th min)...", "", "<TODO>" }, 
    { "Jump to line #...", "", "Index <TODO>" },
    { "Attack Tarcom", "", "None <TODO>" },
    { "Unload", "", "None <TODO>" },
    { "Deploy", "", "None <TODO>" },
    { "Follow friendlies", "", "None <TODO>" }, 
    { "Do this...", "", "MissionType" },
    { "Set global...", "", "Index <TODO>" }, 
    { "Invulnerable", "", "None <TODO>" },
    { "Load onto Transport", "", "None <TODO>" },
    { "Spy on bldg @ waypt...", "", "Waypoint" },
    { "Patrol to waypoint...", "", "Waypoint" }
};

TeamTypeClass::TeamTypeClass() :
    AbstractTypeClass(RTTI_TEAMTYPE, g_TeamTypes.ID(this), 0, ""),
    m_IsActive(true), // Should be set by operator new, but best set it here as well.
    m_AvoidThreats(false),
    m_Suicide(false),
    m_Autocreate(false),
    m_Prebuild(true),
    m_Reinforce(true),
    m_Priority(7),
    m_Number(0),
    m_Max(0),
    m_Unused(0),
    m_Owner(HOUSES_NONE),
    m_TriggerType(nullptr),
    m_Location(-1),
    m_Instances(0),
    m_MissionCount(0),
    m_MemberCount(0)
{
    memset(m_Content, 0, sizeof(m_Content));
}

void *TeamTypeClass::operator new(size_t size)
{
    void *ptr = g_TeamTypes.Allocate();
    static_cast<TeamTypeClass *>(ptr)->m_IsActive = true;

    return ptr;
}

void TeamTypeClass::operator delete(void *ptr)
{
    static_cast<TeamTypeClass *>(ptr)->m_IsActive = false;
    g_TeamTypes.Free(ptr);
}

/**
 * Generates the text for a teamtypes ini entry.
 */
void TeamTypeClass::Build_INI_Entry(char *buffer)
{
    // This function only builds NewINIFormat >= 3 entries.
    char *putp = buffer;
    int32_t booleans =
        (m_AvoidThreats << 1) | (m_Suicide << 2) | (m_Autocreate << 3) | (m_Prebuild << 4) | (m_Reinforce << 5);

    // TODO strlen is how the original advanced the pointer, but using the return of sprintf should be more efficient.
    sprintf(putp, "%d,%d,%d,%d,%d,%d,%d", m_Owner, booleans, m_Priority, m_Number, m_Max, m_Location, m_TriggerType.Get_ID());
    putp += strlen(putp);
    sprintf(putp, ",%d", m_MemberCount);
    putp += strlen(putp);

    for (int i = 0; i < m_MemberCount; ++i) {
        sprintf(putp, ",%s:%d", m_Content[i].m_Object->Get_Name(), m_Content[i].m_Count);
        putp += strlen(putp);
    }

    sprintf(putp, ",%d", m_MissionCount);
    putp += strlen(putp);

    for (int i = 0; i < m_MissionCount; ++i) {
        sprintf(putp, ",%d:%d", m_Missions[i].m_TMission, m_Missions[i].m_TMissionData);
        putp += strlen(putp);
    }
}

/**
 * Fills in a teamtypes members from its ini entry.
 */
void TeamTypeClass::Fill_In(const char *entry, char *data)
{
    strlcpy(m_Name, entry, sizeof(m_Name));
    m_Owner = HousesType(atoi(strtok(data, ",")));

    // INIFormat 1 stores the bools in separate entries,
    // they are packed into a single int in later formats.
    if (g_INIFormat <= 1) {
        int val = atoi(strtok(nullptr, ","));
        m_AvoidThreats = (val & 1) != 0;
        val = atoi(strtok(nullptr, ","));
        m_Suicide = (val & 1) != 0;
        val = atoi(strtok(nullptr, ","));
        m_Autocreate = (val & 1) != 0;
        val = atoi(strtok(nullptr, ","));
        m_Prebuild = (val & 1) != 0;
        val = atoi(strtok(nullptr, ","));
        m_Reinforce = (val & 1) != 0;
    } else {
        int val = atoi(strtok(nullptr, ","));
        m_AvoidThreats = (val & 2) != 0;
        m_Suicide = (val & 4) != 0;
        m_Autocreate = (val & 8) != 0;
        m_Prebuild = (val & 16) != 0;
        m_Reinforce = (val & 32) != 0;
    }

    m_Priority = atoi(strtok(nullptr, ","));
    m_Number = atoi(strtok(nullptr, ","));
    m_Max = atoi(strtok(nullptr, ","));
    m_Location = atoi(strtok(nullptr, ","));

    if (g_INIFormat <= 1) {
        strtok(nullptr, ","); // Skip next entry in format 1? This entry is at the end instead?
    } else {
        m_TriggerType.Set_ID(atoi(strtok(nullptr, ",")));
    }

    m_MemberCount = atoi(strtok(nullptr, ","));

    for (int i = 0; i < m_MemberCount; ++i) {
        const char *obj_name = strtok(nullptr, ",:");
        const char *val_str = strtok(nullptr, ",:");
        ObjectTypeClass *object = nullptr;
        int type;

        // Work out what kind of object we are dealing with from its name.
        if ((type = InfantryTypeClass::From_Name(obj_name)) != -1) {
            object = &InfantryTypeClass::As_Reference(InfantryType(type));
        } else if ((type = UnitTypeClass::From_Name(obj_name)) != -1) {
            object = &UnitTypeClass::As_Reference(UnitType(type));
        } else if ((type = AircraftTypeClass::From_Name(obj_name)) != -1) {
            object = &AircraftTypeClass::As_Reference(AircraftType(type));
        } else if ((type = VesselTypeClass::From_Name(obj_name)) != -1) {
            object = &VesselTypeClass::As_Reference(VesselType(type));
        }

        if (object != nullptr) {
            m_Content[i].m_Object = object;
            m_Content[i].m_Count = atoi(val_str);
        } else {
            --m_MemberCount;

            if (i == 0) {
                break;
            }

            --i;
        }
    }

    m_MissionCount = atoi(strtok(nullptr, ","));

    for (int i = 0; i < m_MissionCount; ++i) {
        m_Missions[i].m_TMission = TeamMissionType(atoi(strtok(nullptr, ",")));
        m_Missions[i].m_TMissionData = atoi(strtok(nullptr, ","));
    }

    if (g_INIFormat <= 1) {
        m_TriggerType.Set_ID(atoi(strtok(nullptr, ",")));
    }
}

/**
 * Creates a team instance templated off of a team type.
 */
TeamClass *TeamTypeClass::Create_One_Of() const
{
    if (!g_ScenarioInit && m_Max <= m_Instances) {
        return nullptr;
    }

    return new TeamClass(const_cast<TeamTypeClass *>(this), HouseClass::As_Pointer(m_Owner));
}

/**
 * Destroyed all team instances bassed of the team type.
 */
void TeamTypeClass::Destroy_All_Of() const
{
    for (int index = 0; index < g_Teams.Count(); ++index) {
        TeamClass *tptr = &g_Teams[index];

        if (tptr->Get_Class() == const_cast<TeamTypeClass *>(this)) {
            delete tptr;
            --index; // Adjust index back to compensate for entry being removed?
        }
    }
}

/**
 * Determines what kind of parameters a team types mission requires.
 */
NeedType TeamTypeClass::TeamMission_Needs(TeamMissionType tmission)
{
    switch (tmission) {
        case TMISSION_CHANGE_FORMATION:
            return NEED_FORMATION;

        case TMISSION_ATTACK:
            return NEED_QUARRY;

        case TMISSION_MOVE_TO_CELL:
            return NEED_CELL;

        case TMISSION_GUARD_AREA:
        case TMISSION_JUMP_TO_LINE:
        case TMISSION_SET_GLOBAL:
            return NEED_NUMBER;

        case TMISSION_ATTACK_WAYPOINT:
        case TMISSION_MOVE_TO_WAYPOINT:
        case TMISSION_SPY_ON_BUILDING:
        case TMISSION_PATROL_WAYPOINT:
            return NEED_WAYPOINT;

        case TMISSION_DO_THIS:
            return NEED_MISSION;

        case TMISSION_ATTACK_TARCOM:
        case TMISSION_UNLOAD:
        case TMISSION_DEPLOY:
        case TMISSION_FOLLOW_FRIENDLIES:
        case TMISSION_INVULNERABLE:
        case TMISSION_LOAD_TRANSPORT:
        default:
            break;
    }

    return NEED_NOTHING;
}

// TODO Verify return.
// This was a free function in original game, but makes more sense as a member.
BOOL TeamTypeClass::Do_Reinforcements()
{
#ifdef GAME_DLL
    BOOL (*func)(const TeamTypeClass *) = reinterpret_cast<BOOL (*)(const TeamTypeClass *)>(0x00533178);
    return func(this);
#else
    return false;
#endif
}

/**
 * Detaches a trigger type target from a team type.
 */
void TeamTypeClass::Detach(target_t target, int a2)
{
    if (Target_Get_RTTI(target) != RTTI_TRIGGERTYPE || m_TriggerType == nullptr) {
        return;
    }

    if (m_TriggerType == As_TriggerType(target)) {
        m_TriggerType = nullptr;
    }
}

/**
 * Draws the team types information, used for built in editor.
 */
void TeamTypeClass::Draw_It(int index, int x, int y, int x_max, int y_max, BOOL selected, TextPrintType style)
{
    static int _tabs[] = { 35, 60, 80, 100 };
    RemapControlType *remapper = GadgetClass::Get_Color_Scheme();

    if ((style & TPF_FONTS) == TPF_6PT_GRAD || (style & TPF_FONTS) == TPF_EDITOR) {
        if (selected) {
            style |= TPF_USE_BRIGHT;
            g_LogicPage->Fill_Rect(x, y, ((x + x_max) - 1), ((y + y_max) - 1), remapper->WindowPalette[0]);
        } else if (!(style & TPF_USE_GRAD_PAL)) {
            style |= TPF_USE_MEDIUM;
        }
    } else {
        remapper = (selected ? &g_ColorRemaps[REMAP_10] : &g_ColorRemaps[REMAP_5]);
    }

    Conquer_Clip_Text_Print(Description(), x, y, remapper, COLOR_TBLACK, style, x_max, _tabs);
}

/**
 * Edit window for internal map editor..
 */
void TeamTypeClass::Edit()
{
    // TODO required for scenario editor, but not main game.
}

/**
 * Encodes pointers for serialisation.
 */
void TeamTypeClass::Code_Pointers()
{
    for (int i = 0; i < m_MemberCount; ++i) {
        m_Content[i].m_Object = reinterpret_cast<ObjectTypeClass *>(m_Content[i].m_Object->As_Target());
    }
}

/**
 * Decodes pointers for deserialisation.
 */
void TeamTypeClass::Decode_Pointers()
{
    for (int i = 0; i < m_MemberCount; ++i) {
        m_Content[i].m_Object = As_TechnoType((uintptr_t)m_Content[i].m_Object);
    }
}

/**
 * Initialises the team types heap.
 */
void TeamTypeClass::Init()
{
    g_TeamTypes.Free_All();
}

/**
 * Reads team types from an ini file.
 */
void TeamTypeClass::Read_INI(GameINIClass &ini)
{
    char buffer[512];
    int entry_count = ini.Entry_Count("TeamTypes");

    for (int index = 0; index < entry_count; ++index) {
        TeamTypeClass *teamtype = new TeamTypeClass;

        if (teamtype != nullptr) {
            char const *entry = ini.Get_Entry("TeamTypes", index);
            ini.Get_String("TeamTypes", entry, nullptr, buffer, sizeof(buffer));
            teamtype->Fill_In(entry, buffer);
        }
    }
}

/**
 * Writes team types to an ini file.
 */
void TeamTypeClass::Write_INI(GameINIClass &ini)
{
    char entry[256];
    ini.Clear("TeamTypes");

    for (int index = 0; index < g_TeamTypes.Count(); ++index) {
        entry[0] = '\0';
        g_TeamTypes[index].Build_INI_Entry(entry);
        ini.Put_String("TeamTypes", g_TeamTypes[index].Get_Name(), entry);
    }
}

/**
 * Get a mission type from its name.
 */
TeamMissionType TeamTypeClass::Mission_From_Name(const char *name)
{
    for (TeamMissionType i = TMISSION_FIRST; i < TMISSION_COUNT; ++i) {
        if (strcasecmp(name, s_TMissions[i].m_Name) == 0) {
            return i;
        }
    }

    return TMISSION_NONE;
}

/**
 * Get a missions name from its mission type.
 */
const char *TeamTypeClass::Name_From_Mission(TeamMissionType tmission)
{
    return s_TMissions[tmission].m_Name;
}

/**
 * Get a pointer to a team type from its name.
 */
TeamTypeClass *TeamTypeClass::As_Pointer(const char *name)
{
    return From_Name(name);
}

/**
 * Get a pointer to a team type from its name.
 */
TeamTypeClass *TeamTypeClass::From_Name(const char *name)
{
    if (name == nullptr) {
        return nullptr;
    }

    for (int i = 0; i < g_TeamTypes.Count(); ++i) {
        if (strcasecmp(g_TeamTypes[i].Get_Name(), name) == 0) {
            return &g_TeamTypes[i];
        }
    }

    return nullptr;
}

/**
 * Get a pointer to a pseudo randomly chosen team type.
 */
TeamTypeClass *TeamTypeClass::Suggested_New_Team(
    HouseClass *house, int32_t avail_air, int32_t avail_unit, int32_t avail_inf, int32_t avail_vessel, BOOL allow_autocreate)
{
    int possible_teams_count = 0;
    TeamTypeClass *possible_teams[20];

    for (int i = 0; i < g_TeamTypes.Count() && possible_teams_count < ARRAY_SIZE(possible_teams); ++i) {
        TeamTypeClass &ttptr = g_TeamTypes[i];
        int max_instances = ttptr.m_Max;

        if ((allow_autocreate && !ttptr.m_Autocreate) || (!allow_autocreate && ttptr.m_Autocreate)) {
            max_instances = 0;
        }

        if (house->What_Type() == ttptr.m_Owner && max_instances > ttptr.m_Instances) {
            possible_teams[possible_teams_count] = &ttptr;
            ++possible_teams_count;
        }
    }

    if (possible_teams_count > 0) {
        return possible_teams[g_Scen.Get_Random_Value(0, possible_teams_count - 1)];
    }

    return nullptr;
}

/**
 * Generate a description of the team type.
 */
const char *TeamTypeClass::Description()
{
    static char _buffer[128];
    char unk = m_Autocreate ? '*' : ' ';
    char wp[3] = { 0 };

    if (m_Location > -1) {
        if (m_Location > 26) {
            wp[0] = m_Location / 26 + '@';
            wp[1] = m_Location % 26 + 'A';
        } else {
            wp[0] = m_Location + 'A';
        }
    }

    snprintf(_buffer,
        sizeof(_buffer),
        "%s\t%s\t%c%s\t%d\t%s",
        m_Name,
        HouseTypeClass::As_Reference(m_Owner).Get_Name(),
        unk,
        wp,
        m_MissionCount,
        Member_Description());

    return _buffer;
}

/**
 * Generate a description of the team type members.
 */
const char *TeamTypeClass::Member_Description()
{
    static char _buffer[128];

    for (int i = 0; i < m_MemberCount; ++i) {
        char tmp[24];
        strlcat(_buffer, m_Content[i].m_Object->Get_Name(), sizeof(_buffer));
        strlcat(_buffer, ":", sizeof(_buffer));
        snprintf(tmp, sizeof(tmp), "%d", m_Content[i].m_Count);
        strlcat(_buffer, tmp, sizeof(_buffer));

        if (i < m_MemberCount - 1) {
            strlcat(_buffer, ",", sizeof(_buffer));
        }
    }

    // Limits string to 25 characters despite buffer being 128?
    if (strlen(_buffer) > 25) {
        strcpy(_buffer + 22, "...");
    }

    return _buffer;
}
