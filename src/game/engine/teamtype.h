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
#pragma once

#ifndef TEAMTYPE_H
#define TEAMTYPE_H

#include "always.h"
#include "abstracttype.h"
#include "dialog.h"
#include "gameptr.h"
#include "gametypes.h"
#include <new>

class HouseClass;
class TeamClass;
class FootClass;
class ObjectTypeClass;
class TriggerTypeClass;
class GameINIClass;

enum TeamMissionType
{
    TMISSION_ATTACK, // "Attack..."
    TMISSION_ATTACK_WAYPOINT, // "Attack Waypoint..."
    TMISSION_CHANGE_FORMATION, // "Change Formation to..."
    TMISSION_MOVE_TO_WAYPOINT, // "Move to waypoint..."
    TMISSION_MOVE_TO_CELL, // "Move to Cell..."
    TMISSION_GUARD_AREA, // "Guard area (1/10th min)..."
    TMISSION_JUMP_TO_LINE, // "Jump to line #..."
    TMISSION_ATTACK_TARCOM, // "Attack Tarcom"
    TMISSION_UNLOAD, // "Unload"
    TMISSION_DEPLOY, // "Deploy"
    TMISSION_FOLLOW_FRIENDLIES, // "Follow friendlies"
    TMISSION_DO_THIS, // "Do this..."
    TMISSION_SET_GLOBAL, // "Set global..."
    TMISSION_INVULNERABLE, // "Invulnerable"
    TMISSION_LOAD_TRANSPORT, // "Load onto Transport"
    TMISSION_SPY_ON_BUILDING, // "Spy on bldg @ waypt..."
    TMISSION_PATROL_WAYPOINT, // "Patrol to waypoint..."

    TMISSION_COUNT,

    TMISSION_NONE = -1,
    TMISSION_FIRST = TMISSION_ATTACK,
};

DEFINE_ENUMERATION_OPERATORS(TeamMissionType);

class TeamMissionClass
{
    friend class TeamTypeClass;

public:
    TeamMissionClass() : m_TMission(TMISSION_NONE) {}
    TeamMissionClass(TeamMissionType mission, int value = 0) : m_TMission(mission) {}
    ~TeamMissionClass() {}

protected:
    TeamMissionType m_TMission;
    int32_t m_TMissionData;

private:
    static TeamMissionClass s_TeamMissions[TMISSION_COUNT];
};

struct TeamTypeContent
{
    int m_Count;
    ObjectTypeClass *m_Object;
};

class TeamTypeClass : public AbstractTypeClass
{
public:
    TeamTypeClass();
    // TeamTypeClass(const TeamTypeClass &that);
    TeamTypeClass(const NoInitClass &noinit) : AbstractTypeClass(noinit) {}
    virtual ~TeamTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    TeamClass *Create_One_Of() const;
    void Destroy_All_Of() const;
    NeedType TeamMission_Needs(TeamMissionType tmission);
    BOOL Do_Reinforcements();
    void Detach(target_t target, BOOL a2 = true);
    void Draw_It(int index, int x, int y, int x_max, int y_max, BOOL selected, TextPrintType style);
    void Edit();
    void Code_Pointers();
    void Decode_Pointers();

    BOOL Roundabout() const { return m_Roundabout; }
    void Add_Instance() { ++m_Instances; }
    void Remove_Instance() { --m_Instances; }
    HousesType Get_Owner() const { return m_Owner; }
    int Get_Waypoint() const { return m_Waypoint; }
    const GamePtr<TriggerTypeClass> &Get_TriggerType() const { return m_TriggerType; }

    static void Init();
    static void Read_INI(GameINIClass &ini);
    static void Write_INI(GameINIClass &ini);
    static TeamMissionType Mission_From_Name(const char *name);
    static const char *Name_From_Mission(TeamMissionType tmission);
    static TeamTypeClass *As_Pointer(const char *name);
    static TeamTypeClass *From_Name(const char *name);
    static TeamTypeClass *Suggested_New_Team(HouseClass *house, int32_t avail_air, int32_t avail_unit, int32_t avail_inf,
        int32_t avail_vessel, BOOL allow_autocreate);

#ifdef GAME_DLL
    TeamTypeClass *Hook_Ctor() { return new (this) TeamTypeClass; }
#endif

private:
    void Fill_In(const char *entry, char *data);
    void Build_INI_Entry(char *buffer);
    const char *Description();
    const char *Member_Description();

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1; // 1
    BOOL m_Roundabout : 1; // 2
    BOOL m_Suicide : 1; // 4
    BOOL m_Autocreate : 1; // 8
    BOOL m_Prebuild : 1; // 16
    BOOL m_Reinforce : 1; // 32
    BOOL m_Bit64 : 1; // 64
#else
    bool m_IsActive;
    bool m_Roundabout; // Always take the safest route, even if it's a detour.
    bool m_Suicide; // Charge at target ignoring enemy units/enemy fire.
    bool m_Autocreate; // Team is only used by autocreate AI (produced taskforces).
    bool m_Prebuild; // Prebuild team members before creating.
    bool m_Reinforce; // Automatically reinforce.
    bool m_Bit64;
#endif
    int m_Priority;
    uint8_t m_InitialNumber;
    uint8_t m_MaxAllowed; // Maximum number of instances of this TeamType that can exist at the same time.
    uint8_t m_Fear;
    HousesType m_Owner;
    GamePtr<TriggerTypeClass> m_TriggerType;
    int m_Waypoint;
    int m_Instances;
    int m_MissionCount;
    TeamMissionClass m_Missions[20];
    int m_MemberCount;
    TeamTypeContent m_Content[5];

private:
    struct TMissionInfoStruct
    {
        const char *m_Name;
        const char *m_Description;
        const char *m_ExpectedValue;
    };

    static TMissionInfoStruct s_TMissions[TMISSION_COUNT];
};

#ifdef GAME_DLL
extern TFixedIHeapClass<TeamTypeClass> &g_TeamTypes;
#else
extern TFixedIHeapClass<TeamTypeClass> g_TeamTypes;
#endif

#endif // TEAMTYPE_H
