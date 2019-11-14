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
#include "gameptr.h"
#include "gametypes.h"

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
    TMISSION_14, // "Load onto Transport"
    TMISSION_SPY_ON_BUILDING, // "Spy on bldg @ waypt..."
    TMISSION_PATROL_WAYPOINT, // "Patrol to waypoint..."

    TMISSION_COUNT,

    TMISSION_NONE = -1,
    TMISSION_FIRST = TMISSION_ATTACK,
};

DEFINE_ENUMERATION_OPERATORS(TeamMissionType);

class TeamMissionClass
{
public:
    TeamMissionClass() : m_TMission(TMISSION_NONE) {}
    TeamMissionClass(TeamMissionType mission, int value = 0) : m_TMission(mission) {}
    ~TeamMissionClass() {}

protected:
    TeamMissionType m_TMission; // 0x00

    // small note, CCWMAP lists this as "Time", looks like its anything the "mission" requires; cell num, time etc
    union TMissionDataUnion
    { // 0x01
        int32_t m_Data; // General data holder
        cell_t m_Cell;
    } u_TMissionData;

private:
    static TeamMissionClass TeamMissions[TMISSION_COUNT];
};

struct TeamTypeContent
{
    int Count; // 0x0
    ObjectTypeClass *Object; // 0x04
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

    void Build_INI_Entry(char *entry_buffer);
    void Fill_In(const char *, const char *);
    TeamClass *Create_One_Of() const;
    void Destroy_All_Of() const;
    NeedType TeamMission_Needs(TeamMissionType tmission);
    BOOL Do_Reinforcements();
    void Detach(target_t target, int a2);

    void Code_Pointers();
    void Decode_Pointers();

    BOOL Validate() { return false; }
    BOOL Avoid_Threats() const { return m_AvoidThreats; }

    static void Init();
    static void Read_Scenario_INI(GameINIClass &ini);
    static void Write_Scenario_INI(GameINIClass &ini);
    static TeamMissionType Mission_From_Name(const char *name);
    static const char *Name_From_Mission(TeamMissionType tmission);
    static TeamTypeClass &As_Reference(const char *name);
    static TeamTypeClass *As_Pointer(const char *name);
    static TeamTypeClass *From_Name(const char *name);
    static const char *Name_From(TeamTypeClass *team);
    static TeamTypeClass *Suggested_New_Team(HouseClass *house, long, long, long, long, int);

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1; // 1
    BOOL m_AvoidThreats : 1; // 2
    BOOL m_Suicide : 1; // 4
    BOOL m_Autocreate : 1; // 8
    BOOL m_Prebuild : 1; // 16
    BOOL m_Reinforce : 1; // 32
#else
    bool m_IsActive;
    bool m_AvoidThreats; // Always take the safest route, even if it’s a detour.
    bool m_Suicide; // Charge at target ignoring enemy units/enemy fire.
    bool m_Autocreate; // Team is only used by autocreate AI (produced taskforces).
    bool m_Prebuild; // Prebuild team members before creating.
    bool m_Reinforce; // Automatically reinforce.
#endif
    int m_Priority; // 0x29 //confirmed
    uint8_t m_field_2D; // 0x2D
    uint8_t m_Max; // 0x2E //confirmed 	//max instance count?
    uint8_t m_field_2F; // 0x2F
    HousesType m_Owner; // 0x30
    GamePtr<TriggerTypeClass> m_TriggerType; // 0x31
    int m_field_35; // 0x35  //some waypoint?
    int m_Instances; // 0x39  //instance count?   //needs confirming, doesnt seem like a right name
    int m_MissionCount; // 0x3D //team mission count?
    TeamMissionClass m_Missions[20]; // 0x41 // A TeamType can have maximum of 20 missions asigned to it
    int m_MemberCount; // 0xA5 	//im not sure if this is exact member count or how many different types we have.
    TeamTypeContent m_Content[5]; // 0xA9 	//members?

private:
    struct TMissionInfoStruct
    {
        const char *Name;
        const char *Description;
        const char *ExpectedValue; // TODO: Rename this.
    };

    static TMissionInfoStruct TMissions[TMISSION_COUNT];
};

#ifdef GAME_DLL
extern TFixedIHeapClass<TeamTypeClass> &g_TeamTypes;
#else
extern TFixedIHeapClass<TeamTypeClass> g_TeamTypes;
#endif

#endif // TEAMTYPE_H
