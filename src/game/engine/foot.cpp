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
#include "foot.h"
#include "coord.h"
#include "iomap.h"
#include "globals.h"
#include "team.h"

static cell_t StartLocation;
static cell_t DestLocation;
static unsigned int MainOverlap[512];	// Is this perhaps some map size math?
static unsigned int LeftOverlap[512];	// Is this perhaps some map size math?
static unsigned int RightOverlap[512];	// Is this perhaps some map size math?

FootClass::FootClass(RTTIType type, int id, HousesType house) :
    TechnoClass(type, id, house),
    m_Bit1_1(false),
    m_Initiated(false),
    m_Bit1_4(false),
    m_ToLook(false),
    m_Deploying(false),
    m_Firing(false),
    m_Rotating(false),
    m_Moving(false),
    m_Bit2_1(false),
    m_InFormation(false),
    m_Bit2_4(false),
    m_ToScatter(false),
    m_Bit2_16(false),
    m_Speed(0),
    m_SpeedMult("1.0"),
    m_FormXCoefficient(0x80000000), // INT32_MIN?
    m_FormYCoefficient(0x80000000), // INT32_MIN?
    m_NavCom(0),
    m_SuspendedNavCom(0),
    m_Team(nullptr),
    m_field_113(TEAM_NUMBER_NONE),
    m_field_114(0),
    m_field_124((MoveType)1), // TODO: Confirm MoveTypes.
    m_field_125(),
    m_field_12E(10),
    m_field_132(),
    m_TeamSpeed(SPEED_FOOT),
    m_TeamMaxSpeed((MPHType)0),
    m_HeadTo(0)
{
    memset(&m_NavList, 0, sizeof(m_NavList));
    memset(&m_Paths, FACING_NONE, sizeof(m_Paths));
}

FootClass::FootClass(const FootClass &that) :
    TechnoClass(that),
    m_Bit1_1(that.m_Bit1_1),
    m_Initiated(that.m_Initiated),
    m_Bit1_4(that.m_Bit1_4),
    m_ToLook(that.m_ToLook),
    m_Deploying(that.m_Deploying),
    m_Firing(that.m_Firing),
    m_Rotating(that.m_Rotating),
    m_Moving(that.m_Moving),
    m_Bit2_1(that.m_Bit2_1),
    m_InFormation(that.m_InFormation),
    m_Bit2_4(that.m_Bit2_4),
    m_ToScatter(that.m_ToScatter),
    m_Bit2_16(that.m_Bit2_16),
    m_Speed(that.m_Speed),
    m_SpeedMult(that.m_SpeedMult),
    m_FormXCoefficient(that.m_FormXCoefficient),
    m_FormYCoefficient(that.m_FormYCoefficient),
    m_NavCom(that.m_NavCom),
    m_SuspendedNavCom(that.m_SuspendedNavCom),
    m_Team(that.m_Team),
    m_field_113(that.m_field_113),
    m_field_114(that.m_field_114),
    m_field_124(that.m_field_124),
    m_field_125(that.m_field_125),
    m_field_12E(that.m_field_12E),
    m_field_132(that.m_field_132),
    m_TeamSpeed(that.m_TeamSpeed),
    m_TeamMaxSpeed(that.m_TeamMaxSpeed),
    m_HeadTo(that.m_HeadTo)
{
    memcpy(&m_NavList, that.m_NavList, sizeof(m_NavList));
    memcpy(&m_Paths, that.m_Paths, sizeof(m_Paths));
}

FootClass::FootClass(const NoInitClass &noinit) :
    TechnoClass(noinit),
    m_field_125(noinit),
    m_field_132(noinit)
{
}

FootClass::~FootClass()
{
    if (GameActive) {
        m_Team->Remove(this);
    }
}

void FootClass::AI()
{
    TechnoClass::AI();
}

void FootClass::Death_Announcement(TechnoClass *killer) const
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const FootClass *, TechnoClass *) = reinterpret_cast<void (*)(const FootClass *, TechnoClass *)>(0x004C3150);
    func(this, killer);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

void FootClass::Assign_Destination(target_t dest)
{
    m_field_124 = (MoveType)1; // TODO: Confirm MoveTypes.
    m_NavCom = dest;
}

coord_t FootClass::Likely_Coord()
{
    return (m_HeadTo ? m_HeadTo : Target_Coord());
}

BOOL FootClass::Start_Driver(coord_t &dest)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(FootClass *, coord_t &) = reinterpret_cast<BOOL (*)(FootClass *, coord_t &)>(0x004C14C8);
    return func(this, dest);
#else
    Stop_Driver();
    if (dest) {
        m_HeadTo = dest;
        m_Moving = true;
        if (Map[Coord_To_Cell(dest)].Goodie_Check(this) ) {
            return true;
        }
        if (m_IsActive) {
            m_HeadTo = 0;
            m_Moving = false;
        }
    }
    return false;
#endif
}

BOOL FootClass::Stop_Driver()
{
    if (m_HeadTo) {
        m_HeadTo = 0;
        Set_Speed(0);
        m_Moving = false;
        return true;
    }
    return false;
}

BOOL FootClass::Offload_Ore_Bail()
{
    // empty in RA, but UnitClass overrides this.
    return false;
}

void FootClass::Approach_Target()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(FootClass *) = reinterpret_cast<void (*)(FootClass *)>(0x005808FC);
    func(this);
#else
#endif
}

void FootClass::Fixup_Path(PathType *path)
{
    // Empty
}
