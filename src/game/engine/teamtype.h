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
#include "gametypes.h"
#include "triggertype.h"

class HouseClass;
class TeamClass;
class ObjectTypeClass;

enum TeamMissionType
{
    TMISSION_NONE = -1,
    TMISSION_FIRST = 0,
    TMISSION_ATTACK = 0, // "Attack..."
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
    TMISSION_COUNT
};

DEFINE_ENUMERATION_OPERATORS(TeamMissionType);

class TeamMissionClass
{
public:
    TeamMissionClass(void) : m_TMission(TMISSION_NONE) {}
    TeamMissionClass(TeamMissionType mission, int value = 0) : m_TMission(mission) {}
    ~TeamMissionClass(void) {}

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

    void Build_INI_Entry(char const *);
    void Fill_In(char const *, char const *);
    TeamClass *const Create_One_Of() const;
    void const Destroy_All_Of() const;
    NeedType TeamMission_Needs(TeamMissionType tmission);
    bool Validate() { return false; }

    void Detach(target_t target, int a2);

    void Code_Pointers();
    void Decode_Pointers();

    BOOL Avoid_Threats() const { return m_AvoidThreats; }

    static void Read_Scenario_INI(GameINIClass &ini);
    static void Write_Scenario_INI(GameINIClass &ini);

    static void Init();

    static TeamMissionType Mission_From_Name(char const *name);
    static char const *Name_From_Mission(TeamMissionType tmission);
    static TeamTypeClass &As_Reference(char const *name);
    static TeamTypeClass *As_Pointer(char const *name);
    static TeamTypeClass *From_Name(char *name);
    static char const *Name_From(TeamTypeClass *team);

    static TeamTypeClass *Suggested_New_Team(HouseClass *house, long, long, long, long, int);

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    /*
    //map bit, >> 1, as it goes past IsActive.
    Always take the safest route, even if it’s a detour  1
    Charge at target ignoring enemy units/enemy fire  	2
    Team is only used by autocreate AI (produced taskforces) 	4
    Prebuild teammembers before creating team   	8
    Automatically reinforce     	16
    */

    BOOL m_IsActive : 1; // 1
    BOOL m_AvoidThreats : 1; // 2
    BOOL m_Suicide : 1; // 4
    BOOL m_Autocreate : 1; // 8
    BOOL m_Prebuild : 1; // 16
    BOOL m_Reinforce : 1; // 32
#else
    bool m_IsActive;
    bool m_AvoidThreats;
    bool m_Suicide;
    bool m_Autocreate;
    bool m_Prebuild;
    bool m_Reinforce;
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
        char const *Name;
        char const *Description;
        char const *ExpectedValue; // TODO: Rename this.
    };

    static TMissionInfoStruct TMissions[TMISSION_COUNT];
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<TeamTypeClass> &g_TeamTypes;
#else
extern TFixedIHeapClass<TeamTypeClass> g_TeamTypes;
#endif

#endif // TEAMTYPE_H
