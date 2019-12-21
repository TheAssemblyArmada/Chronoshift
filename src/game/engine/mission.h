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
#pragma once

#ifndef MISSION_H
#define MISSION_H

#include "always.h"
#include "object.h"
#include "ttimer.h"

class MissionControlClass;

class MissionClass : public ObjectClass
{
    friend class MissionControlClass;

public:
    MissionClass(RTTIType type, int id);
    MissionClass(const NoInitClass &noinit) : ObjectClass(noinit), m_MissionTimer(noinit) {}
    virtual ~MissionClass() {}

#ifdef CHRONOSHIFT_DEBUG
#ifdef CHRONOSHIFT_STANDALONE
    virtual void Debug_Dump(MonoClass *mono) const override;
#else
    void Debug_Dump(MonoClass *mono) const;
#endif
#endif

    // ObjectClass virtual functions
    virtual void AI() override;
    virtual MissionType Get_Mission() const override;

    // MissionClass virtual functions
    virtual void Assign_Mission(MissionType mission);
    virtual BOOL Commence();
    virtual int Mission_Sleep() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Ambush() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Attack() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Capture() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Guard() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Guard_Area() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Harvest() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Hunt() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Move() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Retreat() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Return() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Stop() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Unload() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Enter() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Construction() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Deconstruction() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Repair() { return TICKS_HALF_MINUTE; }
    virtual int Mission_Missile() { return TICKS_HALF_MINUTE; }
    virtual void Set_Mission(MissionType mission);
    virtual void Override_Mission(MissionType mission, int target1 = -1, int target2 = -1);
    virtual BOOL Restore_Mission(); 

    MissionType Get_Suspended_Mission() const { return m_SuspendedMission; };
    MissionType Get_Queued_Mission() const { return m_MissionQueue; }
    void Set_Mission_Status(MissionStatusType status) { m_Status = status; }
    int Get_Mission_Status() const { return m_Status; }
    BOOL Not_On_A_Mission() const { return Get_Mission() == MISSION_NONE && Get_Queued_Mission() == MISSION_NONE; }
    BOOL Has_A_Suspended_Mission() { return m_SuspendedMission != MISSION_NONE; }
    int Mission_Time() { return m_MissionTimer.Time(); }

    static BOOL Is_Recruitable_Mission(MissionType mission);

    static MissionType From_Name(const char *name);
    static const char *Name_From(MissionType mission);
    static const MissionControlClass &Get_Mission_Control(MissionType mission);

protected:
    MissionType m_Mission; // The current mission of this object (def = MISSION_NONE).
    MissionType m_SuspendedMission;
    MissionType m_MissionQueue;
    int m_Status; //see MissionStatusType
    TCountDownTimerClass<FrameTimerClass> m_MissionTimer;

    static const char *s_Missions[MISSION_COUNT];
};

#endif // MISSION_H
