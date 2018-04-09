/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Mission layer of the object class hierachy.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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

class NoInitClass;

class MissionClass : public ObjectClass
{
    friend class MissionControlClass;

public:
    MissionClass(RTTIType type, int id);
    MissionClass(NoInitClass const &noinit) : ObjectClass(noinit) {}
    virtual ~MissionClass() {}

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
#ifdef RAPP_STANDALONE
    virtual int Mission_Find_Crate() { return TICKS_HALF_MINUTE; } // From Sole, can't add until standalone
    virtual int Mission_Timed_Hunt() { return TICKS_HALF_MINUTE; }  // From C&C, can't add until standalone
#endif
    virtual void Set_Mission(MissionType mission);
    virtual void Override_Mission(MissionType mission, int target1 = -1, int target2 = -1);
    virtual BOOL Restore_Mission(); 
    
    MissionType Get_Suspended_Mission() const { return SuspendedMission; };
    MissionType Get_Queued_Mission() const { return MissionQueue; }
    void Set_Mission_Status(MissionStatusType status) { Status = status; }
    int Get_Mission_Status() const { return Status; }

    static BOOL Is_Recruitable_Mission(MissionType mission);

    static MissionType From_Name(char const *name);
    static const char *Name_From(MissionType mission);

protected:
    MissionType Mission; // The current mission of this object (def = MISSION_NONE).
    MissionType SuspendedMission;
    MissionType MissionQueue;
    int Status; //see MissionStatusType
    BasicTimerClass<FrameTimerClass> MissionTimer; // C&CDOS calls this "Timer" might not be BTC, but TCountDownTimerClass<FrameTimerClass>

    static const char *Missions[MISSION_COUNT];
};

#endif // MISSION_H
