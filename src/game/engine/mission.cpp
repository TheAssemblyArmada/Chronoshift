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

const char *MissionClass::s_Missions[MISSION_COUNT] = { 
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
    m_Mission(MISSION_NONE),
    m_SuspendedMission(MISSION_NONE),
    m_MissionQueue(MISSION_NONE),
    m_Status(STATUS_NONE),
    m_MissionTimer()
{
}

#ifdef CHRONOSHIFT_DEBUG
void MissionClass::Debug_Dump(MonoClass *mono) const
{
    ObjectClass::Debug_Dump(mono);
}
#endif

/**
 * @brief Gets the current mission this object has.
 *
 * 0x005027A8
 */
MissionType MissionClass::Get_Mission() const
{
    DEBUG_ASSERT(m_IsActive);

    if (m_Mission != MISSION_NONE) {
        return m_Mission;
    }

    return m_MissionQueue;
}

/**
 * @brief Assigns a mission to this object for after it's current mission is complete.
 *
 * 0x00502C70
 */
void MissionClass::Assign_Mission(MissionType mission)
{
    DEBUG_ASSERT(m_IsActive);

    if (mission == MISSION_QMOVE) {
        mission = MISSION_MOVE;
    }

    if (mission != MISSION_NONE && mission != m_Mission) {
        m_MissionQueue = mission;
    }
}

/**
 * @brief Commence a queued mission.
 *
 * 0x00502C2C
 */
BOOL MissionClass::Commence()
{
    DEBUG_ASSERT(m_IsActive);

    if (m_MissionQueue != MISSION_NONE) {
        m_MissionTimer.Reset();
        m_Mission = m_MissionQueue;
        m_MissionQueue = MISSION_NONE;
        m_Status = STATUS_0;

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
    DEBUG_ASSERT(m_IsActive);

    m_MissionQueue = MISSION_NONE;
    m_Mission = mission;
}

/**
 * @brief Override current mission.
 *
 * 0x00502CFC
 */
void MissionClass::Override_Mission(MissionType mission, int target1, int target2)
{
    DEBUG_ASSERT(m_IsActive);

    if (m_MissionQueue == MISSION_NONE) {
        m_SuspendedMission = m_MissionQueue;
    } else {
        m_SuspendedMission = m_Mission;
    }

    Assign_Mission(m_SuspendedMission);
}

/**
 * @brief Restore a suspended mission.
 *
 * 0x00502D20
 */
BOOL MissionClass::Restore_Mission()
{
    DEBUG_ASSERT(m_IsActive);

    if (m_SuspendedMission != MISSION_NONE) {
        Assign_Mission(m_SuspendedMission);
        m_SuspendedMission = MISSION_NONE;

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
    DEBUG_ASSERT(m_IsActive);

    ObjectClass::AI();

    if ((What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_VESSEL) && Get_Height() > 0) {
        return;
    }

    if (m_IsActive && m_MissionTimer == 0 && Get_Health() > 0) {
        switch (Get_Mission()) {
            case MISSION_SLEEP:
            case MISSION_HARMLESS:
                m_MissionTimer = Mission_Sleep();
                break;

            case MISSION_GUARD:
            case MISSION_STICKY:
                m_MissionTimer = Mission_Guard();
                break;

            case MISSION_ENTER:
                m_MissionTimer = Mission_Enter();
                break;

            case MISSION_CONSTRUCTION:
                m_MissionTimer = Mission_Construction();
                break;

            case MISSION_DECONSTRUCTION:
                m_MissionTimer = Mission_Deconstruction();
                break;

            case MISSION_CAPTURE:
            case MISSION_SABOTAGE:
                m_MissionTimer = Mission_Capture();
                break;

            case MISSION_MOVE:
            case MISSION_QMOVE:
                m_MissionTimer = Mission_Move();
                break;

            case MISSION_ATTACK:
                m_MissionTimer = Mission_Attack();
                break;

            case MISSION_RETREAT:
                m_MissionTimer = Mission_Retreat();
                break;

            case MISSION_HARVEST:
                m_MissionTimer = Mission_Harvest();
                break;

            case MISSION_AREA_GUARD:
                m_MissionTimer = Mission_Guard_Area();
                break;

            case MISSION_RETURN:
                m_MissionTimer = Mission_Return();
                break;

            case MISSION_STOP:
                m_MissionTimer = Mission_Stop();
                break;

            case MISSION_AMBUSH:
                m_MissionTimer = Mission_Ambush();
                break;

            case MISSION_HUNT:
            case MISSION_RESCUE:
                m_MissionTimer = Mission_Hunt();
                break;

            case MISSION_UNLOAD:
                m_MissionTimer = Mission_Unload();
                break;

            case MISSION_REPAIR:
                m_MissionTimer = Mission_Repair();
                break;

            case MISSION_MISSILE:
                m_MissionTimer = Mission_Missile();
                break;
            default:
                DEBUG_LOG("Unhandled MissionType in MissionClass::AI()! (%d)", m_Mission);
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

    return MissionControlClass::s_MissionControl[mission].m_Recruitable;
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
            if (strcasecmp(name, s_Missions[mission]) == 0) {
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
        return s_Missions[mission];
    }

    return "None";
}
