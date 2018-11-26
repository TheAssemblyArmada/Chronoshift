/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
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
#pragma once

#ifndef FOOT_H
#define FOOT_H

#include "always.h"
#include "techno.h"
#include "fixed.h"

class TeamClass;

struct PathType
{
};

class FootClass : public TechnoClass
{
public:
    FootClass(RTTIType type, int id, HousesType house);
    FootClass(const FootClass &that);
    FootClass(const NoInitClass &noinit);
    virtual ~FootClass();

    // AbstractClass
    virtual void AI() override;

    // TechnoClass
    virtual void Death_Announcement(TechnoClass *killer) const override;
    virtual void Assign_Destination(target_t dest) override;

	// FootClass
    virtual coord_t Likely_Coord();
    virtual BOOL Start_Driver(coord_t &dest);
    virtual BOOL Stop_Driver();
    virtual BOOL Offload_Ore_Bail();
    virtual void Approach_Target();
    virtual void Fixup_Path(PathType *path);
    virtual void Set_Speed(signed int speed) { m_Speed = speed; }

//protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_Bit1_1 : 1; // 1
            bool m_Initiated : 1; // 2
            bool m_Bit1_4 : 1; // 4
            bool m_ToLook : 1; // 8
            bool m_Deploying : 1; // 16
            bool m_Firing : 1; // 32
            bool m_Rotating : 1; // 64
            bool m_Moving : 1; // 128

            bool m_Bit2_1 : 1; // 1
            bool m_InFormation : 1; // 2
            bool m_Bit2_4 : 1; // 4
            bool m_ToScatter : 1; // 8
            bool m_Bit2_16 : 1; // 16
        };
        int m_Bitfield;
    };
#else
    bool m_Bit1_1;
    bool m_Initiated;
    bool m_Bit1_4;
    bool m_ToLook;
    bool m_Deploying;
    bool m_Firing;
    bool m_Rotating;
    bool m_Moving;

    bool m_Bit2_1;
    bool m_InFormation;
    bool m_Bit2_4;
    bool m_ToScatter;
    bool m_Bit2_16;
#endif

    signed int m_Speed;
    fixed_t m_SpeedMult;
    signed int m_FormXCoefficient;
    signed int m_FormYCoefficient;
    target_t m_NavCom;
    target_t m_SuspendedNavCom;
    target_t m_NavList[10];
    TeamClass *m_Team;
    TeamNumberType m_field_113;
    FootClass *m_field_114; // Next team member?
    FacingType m_Paths[12];
    MoveType m_field_124;
    TCountDownTimerClass<FrameTimerClass> m_field_125; // path delay timer?
    unsigned int m_field_12E;
    TCountDownTimerClass<FrameTimerClass> m_field_132;
    SpeedType m_TeamSpeed;
    MPHType m_TeamMaxSpeed;
    coord_t m_HeadTo;

};

#endif // FOOT_H
