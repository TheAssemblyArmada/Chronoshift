/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pathfinding layer of game unit class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "foot.h"
#include "building.h"
#include "coord.h"
#include "globals.h"
#include "iomap.h"
#include "session.h"
#include "team.h"

static cell_t StartLocation;
static cell_t DestLocation;
static unsigned int MainOverlap[512]; // Is this perhaps some map size math?
static unsigned int LeftOverlap[512]; // Is this perhaps some map size math?
static unsigned int RightOverlap[512]; // Is this perhaps some map size math?

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
    m_FormXCoefficient(INT32_MIN),
    m_FormYCoefficient(INT32_MIN),
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

FootClass::FootClass(const NoInitClass &noinit) : TechnoClass(noinit), m_field_125(noinit), m_field_132(noinit) {}

FootClass::~FootClass()
{
    if (GameActive) {
        m_Team->Remove(this);
    }
}

MoveType FootClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
    return MoveType();
}

void FootClass::AI()
{
    TechnoClass::AI();
}

/**
 * Can we demolish this object? (can sell check?)
 *
 * 0x004C3328
 */
BOOL FootClass::Can_Demolish() const
{
    switch (What_Am_I()) {
        case RTTI_VESSEL: // Fall through
        case RTTI_UNIT: // Fall through
        case RTTI_AIRCRAFT:
            // Handle if we are heading to a building and that building is the
            // repair depot.
            if (Radio != nullptr && Radio->What_Am_I() == RTTI_BUILDING) {
                if (reinterpret_cast<BuildingClass *>(Radio)->What_Type() == BUILDING_FIX) {
                    if (Distance(Center_Coord(), Radio->Target_Coord()) < 128) {
                        return true;
                    }
                }
            }

        default: // All fall through if return not reached above.
            break;
    }

    return TechnoClass::Can_Demolish();
}

/**
 * Determine this units Y sort?
 *
 * 0x004C154C
 */
coord_t FootClass::Sort_Y() const
{
    if (m_Bit2_1) {
        return Get_Coord() + 0x1000000;
    }

    if (Radio != nullptr && Radio->What_Am_I() == RTTI_UNIT && m_Tethered) {
        return Get_Coord() + 0x1000000;
    }

    return Get_Coord() + 0x300000;
}

/**
 * Unlimbo this object.
 *
 * 0x004C1B6C
 */
BOOL FootClass::Unlimbo(coord_t coord, DirType dir)
{
    if (TechnoClass::Unlimbo(coord, dir)) {
        Revealed(m_OwnerHouse);
        m_Paths[0] = FACING_NONE;

        return true;
    }

    return false;
}

void FootClass::Detach(target_t target, int a2) {}

void FootClass::Detach_All(int a1) {}

BOOL FootClass::Mark(MarkType mark)
{
    return 0;
}

void FootClass::Active_Click_With(ActionType action, ObjectClass *object) {}

void FootClass::Active_Click_With(ActionType action, cell_t cellnum) {}

DamageResultType FootClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
    return DamageResultType();
}

void FootClass::Per_Cell_Process(PCPType pcp) {}

RadioMessageType FootClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    return RadioMessageType();
}

void FootClass::Sell_Back(int a1) {}

void FootClass::Code_Pointers() {}

void FootClass::Decode_Pointers() {}

int FootClass::Mission_Attack()
{
    return 0;
}

int FootClass::Mission_Capture()
{
    return 0;
}

int FootClass::Mission_Guard()
{
    return 0;
}

int FootClass::Mission_Guard_Area()
{
    return 0;
}

int FootClass::Mission_Hunt()
{
    return 0;
}

int FootClass::Mission_Move()
{
    return 0;
}

int FootClass::Mission_Retreat()
{
    return 0;
}

int FootClass::Mission_Enter()
{
    return 0;
}

void FootClass::Override_Mission(MissionType mission, int target1, int target2) {}

BOOL FootClass::Restore_Mission()
{
    return 0;
}

void FootClass::Stun() {}

void FootClass::Death_Announcement(TechnoClass *killer) const
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const FootClass *, TechnoClass *) =
        reinterpret_cast<void (*)(const FootClass *, TechnoClass *)>(0x004C3150);
    func(this, killer);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

target_t FootClass::Greatest_Threat(ThreatType threat)
{
    return target_t();
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
        if (Map[Coord_To_Cell(dest)].Goodie_Check(this)) {
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

PathType *FootClass::Find_Path(cell_t dest, FacingType *facing, int length, MoveType move)
{
    return nullptr;
}

BOOL FootClass::Basic_Path()
{
    return 0;
}

BOOL FootClass::Unravel_Loop(PathType *path, cell_t &cell, FacingType &facing, int x1, int y1, int x2, int y2, MoveType move)
{
    return 0;
}

BOOL FootClass::Register_Cell(PathType *path, cell_t cell, FacingType facing, int cost, MoveType move)
{
    return 0;
}

BOOL FootClass::Follow_Edge(cell_t start, cell_t destination, PathType *path, FacingType chirality, FacingType facing,
    int threat, int threat_state, int length, MoveType move)
{
    return 0;
}

int FootClass::Optimize_Moves(PathType *path, MoveType move)
{
    return 0;
}

cell_t FootClass::Safety_Point(cell_t start_cell, cell_t end_cell, int start, int end)
{
    return cell_t();
}

/**
 * Determine the cost of a move to a cell.
 *
 * 0x004C0570
 */
int FootClass::Passable_Cell(cell_t cell, FacingType facing, int threat, MoveType move)
{
    static int const _value[MOVE_COUNT] = { 1, 1, 3, 8, 10, 0 };

    MoveType canmove = Can_Enter_Cell(cell, facing);

    if (canmove < MOVE_MOVING_BLOCK) {
        if (Distance(Center_Coord(), Cell_To_Coord(cell)) > 256) {
            move = MOVE_MOVING_BLOCK;
        }
    }

    if (canmove <= move) {
        if (Session.Game_To_Play() != GAME_CAMPAIGN || threat == -1
            || Distance(Cell_To_Coord(cell), Cell_To_Coord(DestLocation)) <= 1280
            || Map.Cell_Threat(cell, Owner()) <= threat) {
            return _value[canmove];
        }
    }

    return 0;
}
