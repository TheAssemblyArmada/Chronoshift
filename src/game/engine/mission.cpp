/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Mission layer of the object class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "mission.h"
#include "missioncontrol.h"

const char *MissionClass::Missions[MISSION_COUNT] = { 
    "Sleep",
    "Attack",
    "Move",
    "QMove",
    "Retreat",
    "Guard",
    "Sticky",
    "Enter",
    "Capture",
    "Harvest",
    "Area Guard",
    "Return",
    "Stop",
    "Ambush",
    "Hunt",
    "Unload",
    "Sabotage",
    "Construction",
    "Selling",
    "Repair",
    "Rescue",
    "Missile",
    "Harmless"
};

/**
 * 0x00502680
 */
MissionClass::MissionClass(RTTIType type, int id) :
    ObjectClass(type, id),
    Mission(MISSION_NONE),
    SuspendedMission(MISSION_NONE),
    MissionQueue(MISSION_NONE),
    Status(STATUS_NONE),
    MissionTimer()
{
}

/**
 * @brief Gets the current mission this object has.
 *
 * 0x005027A8
 */
MissionType MissionClass::Get_Mission() const
{
    DEBUG_ASSERT(IsActive);

    if (Mission != MISSION_NONE) {
        return Mission;
    }

    return MissionQueue;
}

/**
 * @brief Assigns a mission to this object for after it's current mission is complete.
 *
 * 0x00502C70
 */
void MissionClass::Assign_Mission(MissionType mission)
{
    DEBUG_ASSERT(IsActive);

    if (mission == MISSION_QMOVE) {
        mission = MISSION_MOVE;
    }

    if (mission != MISSION_NONE && mission != Mission) {
        MissionQueue = mission;
    }
}

/**
 * @brief Commence a queued mission.
 *
 * 0x00502C2C
 */
BOOL MissionClass::Commence()
{
    DEBUG_ASSERT(IsActive);

    if (MissionQueue != MISSION_NONE) {
        MissionTimer.Reset();
        Mission = MissionQueue;
        MissionQueue = MISSION_NONE;
        Status = STATUS_0;

        return true;
    }

    return false;
}

/**
 * @brief Set current mission.
 *
 * 0x00502798
 */
void MissionClass::Set_Mission(MissionType mission)
{
    DEBUG_ASSERT(IsActive);

    MissionQueue = MISSION_NONE;
    Mission = mission;
}

/**
 * @brief Override current mission.
 *
 * 0x00502CFC
 */
void MissionClass::Override_Mission(MissionType mission, int target1, int target2)
{
    DEBUG_ASSERT(IsActive);

    if (MissionQueue == MISSION_NONE) {
        SuspendedMission = MissionQueue;
    } else {
        SuspendedMission = Mission;
    }

    Assign_Mission(SuspendedMission);
}

/**
 * @brief Restore a suspended mission.
 *
 * 0x00502D20
 */
BOOL MissionClass::Restore_Mission()
{
    DEBUG_ASSERT(IsActive);

    if (SuspendedMission != MISSION_NONE) {
        Assign_Mission(SuspendedMission);
        SuspendedMission = MISSION_NONE;

        return true;
    }

    return false;
}

/**
 * @brief Process logic tick for this object.
 *
 * 0x00502824
 */
void MissionClass::AI()
{
    DEBUG_ASSERT(IsActive);

    ObjectClass::AI();

    if ((What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_VESSEL) && Get_Height() > 0) {
        return;
    }

    if (Is_Active() && MissionTimer == 0 && Health > 0) {
        switch (Get_Mission()) {
            case MISSION_SLEEP:
            case MISSION_HARMLESS:
                MissionTimer = Mission_Sleep();
                break;

            case MISSION_GUARD:
            case MISSION_STICKY:
                MissionTimer = Mission_Guard();
                break;

            case MISSION_ENTER:
                MissionTimer = Mission_Enter();
                break;

            case MISSION_CONSTRUCTION:
                MissionTimer = Mission_Construction();
                break;

            case MISSION_DECONSTRUCTION:
                MissionTimer = Mission_Deconstruction();
                break;

            case MISSION_CAPTURE:
            case MISSION_SABOTAGE:
                MissionTimer = Mission_Capture();
                break;

            case MISSION_MOVE:
            case MISSION_QMOVE:
                MissionTimer = Mission_Move();
                break;

            case MISSION_ATTACK:
                MissionTimer = Mission_Attack();
                break;

            case MISSION_RETREAT:
                MissionTimer = Mission_Retreat();
                break;

            case MISSION_HARVEST:
                MissionTimer = Mission_Harvest();
                break;

            case MISSION_AREA_GUARD:
                MissionTimer = Mission_Guard_Area();
                break;

            case MISSION_RETURN:
                MissionTimer = Mission_Return();
                break;

            case MISSION_STOP:
                MissionTimer = Mission_Stop();
                break;

            case MISSION_AMBUSH:
                MissionTimer = Mission_Ambush();
                break;

            case MISSION_HUNT:
            case MISSION_RESCUE:
                MissionTimer = Mission_Hunt();
                break;

            case MISSION_UNLOAD:
                MissionTimer = Mission_Unload();
                break;

            case MISSION_REPAIR:
                MissionTimer = Mission_Repair();
                break;

            case MISSION_MISSILE:
                MissionTimer = Mission_Missile();
                break;
            default:
                DEBUG_LOG("Unhandled MissionType in MissionClass::AI()! (%d)", Mission);
                break;
        }
    }
}

/**
 * @brief Check if aa mission allows an object to be recruited or not.
 *
 * 0x00502D54
 */
BOOL MissionClass::Is_Recruitable_Mission(MissionType mission)
{
    DEBUG_ASSERT(mission < MISSION_COUNT);

    if (mission == MISSION_NONE) {
        return true;
    }

    return MissionControlClass::MissionControl[mission].Recruitable;
}

/**
 * @brief Retrieves a mission name.
 *
 * 0x00502C8C
 */
MissionType MissionClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return MISSION_NONE;
    }

    if (name != nullptr) {
        for (MissionType mission = MISSION_FIRST; mission < MISSION_COUNT; ++mission) {
            if (strcasecmp(name, Missions[mission]) == 0) {
                return mission;
            }
        }
    }

    return MISSION_NONE;
}

/**
 * @brief Retrieves a mission name.
 *
 * 0x00502CDC
 */
const char *MissionClass::Name_From(MissionType mission)
{
    if (mission != MISSION_NONE && mission < MISSION_COUNT) {
        return Missions[mission];
    }

    return "None";
}
