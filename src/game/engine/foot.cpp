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
#include "bench.h"
#include "building.h"
#include "coord.h"
#include "globals.h"
#include "iomap.h"
#include "missioncontrol.h"
#include "rules.h"
#include "session.h"
#include "target.h"
#include "team.h"
#include <algorithm>
#include <captainslog.h>
#include <cstdlib>

// Size in bits of each element of the path flags array.
#define PATH_FLAG_BITSIZE 32

#ifdef GAME_DLL
extern cell_t &g_StartLocation;
extern cell_t &g_DestLocation;
#else
static cell_t g_StartLocation;
static cell_t g_DestLocation;
#endif

static unsigned g_MainOverlap[512]; // Is this perhaps some map size math?
static unsigned g_LeftOverlap[512]; // Is this perhaps some map size math?
static unsigned g_RightOverlap[512]; // Is this perhaps some map size math?
static int g_PathCount;

FootClass::FootClass(RTTIType type, int id, HousesType house) :
    TechnoClass(type, id, house),
    m_LimitedScan(false),
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
    m_Group(GROUP_NONE),
    m_Member(nullptr),
    m_PathBreak(MOVE_CLOAK),
    m_PathDelay(),
    m_Try(MAX_TRY),
    m_BaseDefenseDelay(),
    m_TeamSpeed(SPEED_FOOT),
    m_TeamMaxSpeed(MPH_MIN),
    m_HeadTo(0)
{
    memset(m_NavList, 0, sizeof(m_NavList));
    memset(m_Paths, FACING_NONE, sizeof(m_Paths));
}

FootClass::FootClass(const FootClass &that) :
    TechnoClass(that),
    m_LimitedScan(that.m_LimitedScan),
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
    m_Group(that.m_Group),
    m_Member(that.m_Member),
    m_PathBreak(that.m_PathBreak),
    m_PathDelay(that.m_PathDelay),
    m_Try(that.m_Try),
    m_BaseDefenseDelay(that.m_BaseDefenseDelay),
    m_TeamSpeed(that.m_TeamSpeed),
    m_TeamMaxSpeed(that.m_TeamMaxSpeed),
    m_HeadTo(that.m_HeadTo)
{
    memcpy(m_NavList, that.m_NavList, sizeof(m_NavList));
    memcpy(m_Paths, that.m_Paths, sizeof(m_Paths));
}

FootClass::FootClass(const NoInitClass &noinit) : TechnoClass(noinit), m_PathDelay(noinit), m_BaseDefenseDelay(noinit) {}

#ifdef CHRONOSHIFT_DEBUG
void FootClass::Debug_Dump(MonoClass *mono) const
{
    TechnoClass::Debug_Dump(mono);
}
#endif

FootClass::~FootClass()
{
    if (g_GameActive) {
        m_Team->Remove(this);
    }
}

/**
 *
 *
 */
MoveType FootClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
    return MOVE_OK;
}

/**
 *
 *
 */
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
            // Handle if we are heading to a building and that building is the repair depot.
            if (m_Radio != nullptr && m_Radio->What_Am_I() == RTTI_BUILDING) {
                if (reinterpret_cast<BuildingClass *>(m_Radio)->What_Type() == BUILDING_FIX) {
                    if (Coord_Distance(Center_Coord(), m_Radio->Target_Coord()) < 128) {
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

    if (m_Radio != nullptr && m_Radio->What_Am_I() == RTTI_UNIT && m_Tethered) {
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

void FootClass::Detach(target_t target, BOOL a2)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C3210, void, FootClass *, target_t, BOOL);
    func(this, target, a2);
#endif
}

void FootClass::Detach_All(BOOL a1)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C2F80, void, FootClass *, BOOL);
    func(this, a1);
#endif
}

/**
 *
 *
 * 0x004C08D0
 */
BOOL FootClass::Mark(MarkType mark)
{
    if (!TechnoClass::Mark(mark)) {
        return false;
    }
    cell_t cell = Get_Cell();
    switch (mark) {
        case MARK_REMOVE:
            g_Map.Pick_Up(cell, this);
            return true;
        case MARK_PUT:
            g_Map.Place_Down(cell, this);
            return true;
        case MARK_REDRAW:
            g_Map.Refresh_Cells(cell, Overlap_List(true));
            g_Map.Refresh_Cells(cell, Occupy_List());
            return true;
        default:
            g_Map.Refresh_Cells(cell, Overlap_List());
            g_Map.Refresh_Cells(cell, Occupy_List());
            return true;
    }
}

void FootClass::Active_Click_With(ActionType action, ObjectClass *object)
{
#ifdef GAME_DLL
    void (*func)(FootClass *, ActionType, ObjectClass *) =
        reinterpret_cast<void (*)(FootClass *, ActionType, ObjectClass *)>(0x004C1E30);
    func(this, action, object);
#endif
}

void FootClass::Active_Click_With(ActionType action, cell_t cellnum)
{
#ifdef GAME_DLL
    void (*func)(FootClass *, ActionType, cell_t) = reinterpret_cast<void (*)(FootClass *, ActionType, cell_t)>(0x004C2280);
    func(this, action, cellnum);
#endif
}

DamageResultType FootClass::Take_Damage(int &damage, int distance, WarheadType warhead, TechnoClass *object, BOOL force)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C1BC8, DamageResultType, FootClass *, int &, int, WarheadType, TechnoClass *, BOOL)
    return func(this, damage, distance, warhead, object, force);
#else
    return DamageResultType();
#endif
}

void FootClass::Per_Cell_Process(PCPType pcp)
{
#ifdef GAME_DLL
    void (*func)(FootClass *, PCPType) = reinterpret_cast<void (*)(FootClass *, PCPType)>(0x004C2568);
    func(this, pcp);
#endif
}

/**
 *
 *
 */
RadioMessageType FootClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    switch (message) {
        case RADIO_NEED_A_LIFT:
            if (m_Mission == MISSION_ENTER || m_MissionQueue == MISSION_ENTER) {
                TechnoClass::Receive_Message(radio, message, target);
                return RADIO_ROGER;
            }
            break;

        case RADIO_MOVE_TO_LOCATION:
            if (m_NavCom == target) {
                return RADIO_ROGER;
            }

            if (::As_Target(Get_Cell()) == target) {
                return RADIO_NOW_WHAT;
            }

            if (m_Mission == MISSION_GUARD && m_MissionQueue == MISSION_NONE) {
                Assign_Mission(MISSION_MOVE);
            }

            Assign_Destination(target);
            m_MissionTimer = 0;
            return RADIO_ROGER;

        case RADIO_NEED_TO_MOVE:
            target = m_NavCom;

            if (!Target_Legal(m_NavCom)) {
                return RADIO_ROGER;
            }

            return RADIO_UNABLE_TO_COMPLY;

        case RADIO_RUN_AWAY:
            if (m_Radio != nullptr && m_Radio->As_Target() == m_NavCom) {
                Assign_Destination(0);
            }

            if (m_Mission == MISSION_SLEEP) {
                Assign_Mission(MISSION_GUARD);
                Commence();
            }

            if (m_Mission == MISSION_ENTER) {
                Assign_Mission(MISSION_GUARD);
            }

            if (!m_Rotating && !Target_Legal(m_NavCom)) {
                Scatter(0, true, true);
            }
            break;

        case RADIO_REPAIR_ONE_STEP:
            if (Target_Legal(m_NavCom)) {
                return RADIO_UNABLE_TO_COMPLY;
            }
            break;

        case RADIO_ON_SERVICE:
            BuildingClass *bptr = g_Map[Center_Cell()].Cell_Building();
            if (bptr == nullptr) {
                return RADIO_UNABLE_TO_COMPLY;
            }

            if (bptr->What_Type() != BUILDING_FIX) {
                return RADIO_UNABLE_TO_COMPLY;
            }

            return RADIO_ROGER;
    };

    return TechnoClass::Receive_Message(radio, message, target);
}

/**
 *
 *
 * @address 0x004C33C4
 */
void FootClass::Sell_Back(int a1)
{
    if (a1) {
        if (m_OwnerHouse == g_PlayerPtr) {
            Speak(VOX_UNIT_SOLD);
            Sound_Effect(VOC_CASHTURN);
        }

        m_OwnerHouse->Refund_Money(Refund_Amount());
        Stun();
        Limbo();
        delete this;
    }
}

/**
 * @brief
 *
 * @address 0x004F9760
 */
void FootClass::Code_Pointers()
{
    if (m_Member != nullptr && m_Member->m_IsActive) {
        m_Member = reinterpret_cast<FootClass *>(m_Member->As_Target());
    } else {
        m_Member = nullptr;
    }
    TechnoClass::Code_Pointers();
}

/**
 * @brief
 *
 * @address 0x004F97B8
 */
void FootClass::Decode_Pointers()
{
    if (m_Member != nullptr) {
        m_Member = reinterpret_cast<FootClass *>(As_Techno((uintptr_t)m_Member));
        captainslog_assert(m_Member != nullptr);
    }
    TechnoClass::Decode_Pointers();
}

/**
 *
 *
 * @address 0x004F97B8
 */
int FootClass::Mission_Attack()
{
    if (m_TarCom != 0) {
        Approach_Target();
    } else {
        Enter_Idle_Mode();
    }
    return (900 * MissionControlClass::s_MissionControl[m_Mission].m_Rate) + g_Scen.Get_Random_Value(0, 2);
}

int FootClass::Mission_Capture()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C0F50, int, FootClass *);
    return func(this);
#else
    return 0;
#endif
}

int FootClass::Mission_Guard()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C1104, int, FootClass *);
    return func(this);
#else
    return 0;
#endif
}

int FootClass::Mission_Guard_Area()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C191C, int, FootClass *);
    return func(this);
#else
    return 0;
#endif
}

int FootClass::Mission_Hunt()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C1340, int, FootClass *);
    return func(this);
#else
    return 0;
#endif
}

int FootClass::Mission_Move()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C0DE4, int, FootClass *);
    return func(this);
#else
    return 0;
#endif
}

int FootClass::Mission_Retreat()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C37B8, int, FootClass *);
    return func(this);
#else
    return 0;
#endif
}

int FootClass::Mission_Enter()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004C2EB0, int, FootClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 *
 *
 * @address 0x004C2B98
 */
void FootClass::Override_Mission(MissionType mission, int target1, int target2)
{
    m_SuspendedNavCom = m_NavCom;
    TechnoClass::Override_Mission(mission, target1, target2);
    Assign_Destination(target2);
}

/**
 *
 *
 * @address 0x004C2BCC
 */
BOOL FootClass::Restore_Mission()
{
    if (!TechnoClass::Restore_Mission()) {
        return false;
    }
    Assign_Destination(m_SuspendedNavCom);
    return true;
}

/**
 *
 *
 * @address 0x004C160C
 */
void FootClass::Stun()
{
    Assign_Destination(0);
    m_Paths[0] = FACING_NONE;
    Stop_Driver();
    TechnoClass::Stun();
}

void FootClass::Death_Announcement(TechnoClass *killer) const
{
#ifdef GAME_DLL
    void (*func)(const FootClass *, TechnoClass *) =
        reinterpret_cast<void (*)(const FootClass *, TechnoClass *)>(0x004C3150);
    func(this, killer);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

/**
 *
 *
 * @address
 */
target_t FootClass::Greatest_Threat(ThreatType threat)
{
    captainslog_assert(m_IsActive);

    if (m_LimitedScan) {
        // Threat Scan is limited to the object's range only.
        threat &= ~THREAT_AREA | THREAT_RANGE;
    }

    if (Get_Owner_House()->Is_Human() && Is_Cloakable() && m_Mission == MISSION_GUARD) {
        return 0;
    }

    if (!(threat & THREAT_3D40 | THREAT_GROUND | THREAT_VESSELS)) {
        if (What_Am_I() == RTTI_VESSEL) {
            threat |= THREAT_GROUND | THREAT_VESSELS;
        } else {
            threat |= THREAT_GROUND;
        }
    }

    target_t target = TechnoClass::Greatest_Threat(threat);

    if (m_LimitedScan) {
        if (!Target_Legal(target)) {
            // Couldn't find a target using a limited scan.
            m_LimitedScan = false;
            return 0;
        }
    }

    return target;
}

void FootClass::Assign_Destination(target_t dest)
{
    m_PathBreak = MOVE_CLOAK;
    m_NavCom = dest;
}

coord_t FootClass::Likely_Coord()
{
    return (m_HeadTo ? m_HeadTo : Target_Coord());
}

/**
 *
 *
 */
BOOL FootClass::Start_Driver(coord_t &dest)
{
    Stop_Driver();

    if (dest) {
        m_HeadTo = dest;
        m_Moving = true;

        if (g_Map[Coord_To_Cell(dest)].Goodie_Check(this)) {
            return true;
        }

        if (m_IsActive) {
            m_HeadTo = 0;
            m_Moving = false;
        }
    }

    return false;
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
#ifdef GAME_DLL
    void (*func)(FootClass *) = reinterpret_cast<void (*)(FootClass *)>(0x004C1640);
    func(this);
#endif
}

void FootClass::Fixup_Path(PathType *path)
{
    // Empty
}

/**
 * Constructs a path if possible.
 *
 * 0x004BF77C
 */
PathType *FootClass::Find_Path(cell_t dest, FacingType *buffer, int length, MoveType move)
{
    // captainslog_debug("Running Find_Path...");
    // This is Script_Unit_Pathfinder in OpenDUNE which returns the struct as
    // an object instead of using a static instance. Might be useful to study
    // for rewriting this without the gotos.
    static PathType _path;

    // If the buffer pointer is nullptr, we can't proceed so return nullptr.
    if (buffer == nullptr) {
        return nullptr;
    }

    int threat;
    int risk;
    int threat_state = 0;
    ++g_PathCount;

    BENCHMARK_START(BENCH_FIND_PATH);

    if (!m_Team.Is_Valid() || !m_Team->Is_Roundabout()) {
        // captainslog_debug(" Threat procesing will not be performed for final cell.");
        threat = -1;
        risk = -1;
    } else {
        // captainslog_debug(" Threat procesing will be performed if needed for final cell.");
        if (!m_Team.Is_Valid()) {
            risk = Risk();
        } else {
            risk = m_Team->Field35();
        }

        threat = 0;
    }

    // Set up initial state of the path
    cell_t current_cell = Coord_To_Cell(Get_Coord());

    _path.StartCell = g_StartLocation = current_cell;
    _path.Score = 0;
    _path.Length = 0;
    _path.Moves = buffer;
    _path.Overlap = g_MainOverlap;
    _path.PreviousCell = -1;
    _path.UnravelCheckpoint = -1;
    _path.Moves[0] = FACING_NONE;
    --length;
    g_DestLocation = dest;
    memset(g_MainOverlap, 0, sizeof(g_MainOverlap));
    _path.Overlap[(uint16_t)current_cell / 32] |= 1 << ((uint16_t)current_cell % 32);

    while (_path.Length < length) {
        if (current_cell == dest) {
            break;
        }

        FacingType direction = Direction_To_Facing(Cell_Direction8(current_cell, dest));
        cell_t adj_cell = Cell_Get_Adjacent(current_cell, direction);
        int cell_score = Passable_Cell(adj_cell, direction, threat, move);

        if (cell_score != 0) { // Great, we have a direct move, do the next round.
            // captainslog_debug("  Cell passable and on direct route, registering cell.");
            Register_Cell(&_path, adj_cell, direction, cell_score, move);
            current_cell = adj_cell;
        } else { // Oops, we bumped into something, find a way around.
            // captainslog_debug("  No direct route, doing further searching.");
            FacingType right_moves[304];
            PathType right_path;
            int right_score = 0;
            FacingType left_moves[304];
            PathType left_path;
            int left_score = 0;
            PathType *chosen_path = nullptr;

            if (adj_cell == dest) { // Close enough.
                break;
            }

            int i = 0;

            while (i < 5) {
                FacingType next_dir = Direction_To_Facing(Cell_Direction8(adj_cell, dest));
                adj_cell = Cell_Get_Adjacent(adj_cell, next_dir);

                // If we still don't have passable, see if we are close.
                if (Passable_Cell(adj_cell, FACING_NONE, threat, move) == 0) {
                    // If we are close, adjust the threat and try again.
                    if (adj_cell == dest) {
                        if (threat == -1) {
                            // Set current a break to finish pathfinding.
                            current_cell = adj_cell;

                            break;
                        }

                        // captainslog_debug("  Adjusting threat state for final cell early check.");
                        switch (threat_state++) {
                            case 0:
                                threat = risk / 2;
                                break;
                            case 1:
                                threat += risk;
                                break;
                            case 2:
                                threat = -1;
                                break;
                            default:
                                break;
                        }

                        break;
                    } else {
                        continue;
                    }
                }

                memcpy(&left_path, &_path, sizeof(left_path));
                left_path.Moves = left_moves;
                left_path.Overlap = g_LeftOverlap;
                memcpy(left_moves, _path.Moves, _path.Length);
                memcpy(left_path.Overlap, _path.Overlap, sizeof(g_LeftOverlap));

                left_score = Follow_Edge(current_cell,
                    adj_cell,
                    &left_path,
                    FACING_EDGE_LEFT,
                    direction,
                    threat,
                    threat_state,
                    sizeof(left_moves) - 2,
                    move);

                memcpy(&right_path, &_path, sizeof(right_path));
                right_path.Moves = right_moves;
                right_path.Overlap = g_RightOverlap;
                memcpy(right_moves, _path.Moves, _path.Length);
                memcpy(right_path.Overlap, _path.Overlap, sizeof(g_RightOverlap));

                right_score = Follow_Edge(current_cell,
                    adj_cell,
                    &right_path,
                    FACING_EDGE_RIGHT,
                    direction,
                    threat,
                    threat_state,
                    sizeof(right_moves) - 2,
                    move);

                if (left_score != 0 || right_score != 0) {
                    // captainslog_debug("  Found a path following an edge.");
                    break;
                }

                // captainslog_debug("  Follow_Edge failed to find path, checking adjacents.");
                while (adj_cell != dest) {
                    next_dir = Direction_To_Facing(Cell_Direction8(adj_cell, dest));
                    adj_cell = Cell_Get_Adjacent(adj_cell, next_dir);

                    if (Passable_Cell(adj_cell, next_dir, threat, move) == 0) {
                        // captainslog_debug("  Adjacent cell not passable, incrementing exit checker.");
                        ++i;

                        break;
                    }
                }

                if (adj_cell != dest) {
                    continue;
                }

                if (threat == -1) {
                    // captainslog_debug("  Find_Path bailing as failed Follow_Edge and adj_cell == dest.");
                    // Set current a break to finish pathfinding.
                    current_cell = adj_cell;

                    break;
                }

                // captainslog_debug("  Adjusting threat for final cell check.");
                switch (threat_state++) {
                    case 0:
                        threat = risk >> 1;
                        break;
                    case 1:
                        threat += risk;
                        break;
                    case 2:
                        threat = -1;
                        break;
                    default:
                        break;
                }

                break;
            }

            if (left_score != 0 || right_score != 0) {
                chosen_path = &left_path;

                if (right_score == 0) {
                    // captainslog_debug("  Find_Path found a left path.");
                    chosen_path = &left_path;
                } else if (left_score == 0) {
                    // captainslog_debug("  Find_Path found a right path.");
                    chosen_path = &right_path;
                } else {
                    // captainslog_debug("  Find_Path chose path based on length, left %d vs right %d.", left_path.Length,
                    // right_path.Length);
                    chosen_path = left_path.Length >= right_path.Length ? &right_path : &left_path;
                }

                int move_count = std::min(chosen_path->Length, length);

                if (move_count > 0) {
                    // captainslog_debug("  Find_Path is copying .");
                    memcpy(_path.Overlap, chosen_path->Overlap, sizeof(g_MainOverlap));
                    memcpy(_path.Moves, chosen_path->Moves, move_count);
                    _path.Length = move_count;
                    _path.Score = chosen_path->Score;
                    _path.PreviousCell = -1;
                    _path.UnravelCheckpoint = -1;

                    current_cell = adj_cell;
                } else {
                    // captainslog_debug("  Find_Path bailing, mount cound 0 after successful follow edge.");
                    break;
                }
            } else {
                break;
            }
        }
    }

    if (_path.Length < length) {
        _path.Moves[_path.Length++] = FACING_NONE;
    }

    Optimize_Moves(&_path, move);

    BENCHMARK_END(BENCH_FIND_PATH);

    // captainslog_debug("Completed Find_Path...");
    return &_path;
}

/**
 * Generates a basic path for this object to follow.
 *
 * 0x004C09E4
 */
BOOL FootClass::Basic_Path()
{
    m_Paths[0] = FACING_NONE;
    bool havepath = false;

    if (Target_Legal(m_NavCom)) {
        cell_t navcell = As_Cell(m_NavCom);
        int navdist = Distance_To_Target(m_NavCom);
        int straydist = m_Team.Has_Valid_ID() ? g_Rule.Stray_Distance() : g_Rule.Close_Enough_Distance();

        if (Can_Enter_Cell(navcell) > MOVE_CLOAK && navdist > straydist) {
            MZoneType mzone = reinterpret_cast<TechnoTypeClass const *>(&Class_Of())->Get_Movement_Zone();
            SpeedType speed = reinterpret_cast<TechnoTypeClass const *>(&Class_Of())->Get_Speed();
            cell_t thiscell = Coord_To_Cell(Get_Coord());
            cell_t nearcell = g_Map.Nearby_Location(navcell, speed, g_Map[thiscell].Get_Zone(mzone), mzone);

            // If we have a nearby cell and its closer than navdist, set that to
            // the nav cell instead.
            if (nearcell != 0) {
                if (Coord_Distance(Cell_To_Coord(navcell), Cell_To_Coord(nearcell)) < navdist) {
                    navcell = nearcell;
                }
            }
        }

        if (What_Am_I() == RTTI_INFANTRY) {
            FootClass *occupier = reinterpret_cast<FootClass *>(g_Map[Coord_To_Cell(Center_Coord())].Get_Occupier());

            // Find a chained object from the cell occupier that has a valid
            // path that isn't us if we are Infantry.
            while (occupier != nullptr) {
                if (occupier != this && occupier->What_Am_I() == RTTI_INFANTRY && occupier->m_NavCom == m_NavCom
                    && occupier->m_Paths[0] != FACING_NONE) {
                    if (Coord_To_Cell(occupier->m_HeadTo) == Coord_To_Cell(occupier->Get_Coord())) {
                        memcpy(m_Paths, &occupier->m_Paths[1], sizeof(m_Paths) - sizeof(m_Paths[0]));
                    } else {
                        memcpy(m_Paths, occupier->m_Paths, sizeof(m_Paths));
                    }

                    if (m_Paths[0] != FACING_NONE) {
                        havepath = true;
                    }

                    break;
                }

                occupier = reinterpret_cast<FootClass *>(occupier->m_Next);
            }
        }

        // If we didn't find a path on a chained object.
        if (!havepath) {
            Mark(MARK_REMOVE);
            MoveType move = MOVE_TEMP;
            m_Paths[0] = FACING_NONE;

            // If we are human owned and aren't close enough, downgrade max
            // move types we will evaluate the path for.
            if (m_OwnerHouse->Is_Human() && Get_Mission() == MISSION_MOVE) {
                if (Distance_To_Target(m_NavCom) < g_Rule.Close_Enough_Distance()) {
                    move = MOVE_DESTROYABLE;
                }
            }

            PathType *path;
            PathType pathobj;
            FacingType facings[GEN_PATH_LENGTH];
            bool do_fixup = false;

            // Do loop at least once to try and get a path, then check our moves.
            do {
                path = Find_Path(navcell, facings, GEN_PATH_LENGTH, m_PathBreak);

                if (path != nullptr && path->Score != 0) {
                    // memcpy(&pathobj, path, sizeof(pathobj));
                    pathobj = *path;
                    do_fixup = true;

                    break;
                }

                ++m_PathBreak;
            } while (m_PathBreak <= move);

            // If we found a path within the allowed move types.
            if (do_fixup) {
                Fixup_Path(&pathobj);
                memcpy(m_Paths, facings, std::min(path->Length * sizeof(m_Paths[0]), PATH_LENGTH * sizeof(m_Paths[0])));
            }

            Mark(MARK_PUT);
        }

        m_PathDelay.Reset(900 * g_Rule.Path_Delay());

        // Do we have a valid path?
        if (m_Paths[0] != FACING_NONE) {
            return true;
        }

        // Stop if path isn't valid.
        Stop_Driver();
    }

    return false;
}

/**
 * Attempts to remove loops from the path.
 *
 * 0x004BF49C
 */
BOOL FootClass::Unravel_Loop(PathType *path, cell_t &cell, FacingType &facing, int x1, int y1, int x2, int y2, MoveType move)
{
    BOOL even = false;
    cell_t cellnum = cell + AdjacentCell[Opposite_Facing(facing)];
    FacingType *facing_ptr = &path->Moves[path->Length - 1];
    FacingType face = facing;

    for (int i = path->Length; i > 0; --i) {
        if (Point_Relative_To_Line(Cell_Get_X(cellnum), Cell_Get_Y(cellnum), x1, y1, x2, y2) == 0 || even) {
            if ((face % 2) != 0 && cellnum != path->UnravelCheckpoint) {
                cell = cellnum;
                facing = *(facing_ptr - 1); // field_A a pointer to facing types?
                path->UnravelCheckpoint = cellnum;
                path->Length = i;

                return true;
            }

            even = even == false; // flip even bool
        }

        path->Score -= Passable_Cell(cellnum, *facing_ptr, -1, move);
        path->Overlap[cellnum / PATH_FLAG_BITSIZE] &= ~(1 << (cellnum % PATH_FLAG_BITSIZE));
        face = *facing_ptr--;
        cellnum += AdjacentCell[Opposite_Facing(face)];
    }

    return false;
}

/**
 * Registers a cell in the path and check if we visit the cell already.
 *
 * 0x004BF5E0
 */
BOOL FootClass::Register_Cell(PathType *path, cell_t cell, FacingType facing, int cost, MoveType move)
{
    captainslog_assert(cell >= 0);

    // Check the flagging for the passed in cell, if its not flagged, then add
    // facing to what appears to be the move list.
    if (!(path->Overlap[cell / PATH_FLAG_BITSIZE] & (1 << (cell % PATH_FLAG_BITSIZE)))) {
        path->Moves[path->Length++] = facing;
        path->Score += cost;
        path->Overlap[cell / PATH_FLAG_BITSIZE] |= 1 << (cell % PATH_FLAG_BITSIZE);

        return true;
    }

    // If the last facing in the list matches our facing, then do some unflagging
    // vodoo
    if (Opposite_Facing(facing) == path->Moves[path->Length - 1]) {
        cell_t cellnum = Cell_Get_Adjacent(cell, Opposite_Facing(facing));
        path->Overlap[cellnum / PATH_FLAG_BITSIZE] &= ~(1 << (cellnum % PATH_FLAG_BITSIZE));
        --path->Length;

        return true;
    }

    // If cell doesn't equal the previous cell perhaps?
    if (cell != path->PreviousCell) {
        int count = 0;
        int final_count = 0;
        cell_t test_cell = path->StartCell;
        FacingType *face_ptr = path->Moves;
        path->PreviousCell = cell;

        // If the cell isn't the start cell, go through the list and see if any
        // of the directions we currently have lead to the cell.
        if (cell != path->StartCell) {
            while (count < path->Length) {
                test_cell = Cell_Get_Adjacent(test_cell, *face_ptr);
                ++count;
                ++face_ptr;

                if (test_cell == cell) {
                    break;
                }
            }

            final_count = count;
        }

        // For any remaining count do more stuff.
        while (count < path->Length) {
            test_cell = Cell_Get_Adjacent(test_cell, *face_ptr);
            path->Score -= Passable_Cell(test_cell, *face_ptr, -1, move);
            // Sole has a -1 before the 1 << while RA doesn't here...
            path->Overlap[test_cell / PATH_FLAG_BITSIZE] &= ~(1 << (test_cell % PATH_FLAG_BITSIZE));
            ++count;
            ++face_ptr;
        }

        path->Length = final_count;

        return true;
    }

    return false;
}

/**
 * Calculates a path around an obstacle after "crashing" into it during path calc.
 *
 * 0x004BFDE4
 */
BOOL FootClass::Follow_Edge(cell_t start, cell_t destination, PathType *path, FacingType chirality, FacingType facing,
    int threat, int threat_state, int length, MoveType move)
{
    // OpenDUNE calls this function Script_Unit_Pathfinder_Connect. Again the
    // C&C version is more complicated, checking the relative position of
    // the cell being checked against a straight line to the target.

    // If we don't have a path stuct to record the found path to, don't bother.
    if (path == nullptr) {
        return false;
    }

    int start_x = Cell_Get_X(start);
    int start_y = Cell_Get_Y(start);
    int dest_x = Cell_Get_X(destination);
    int dest_y = Cell_Get_Y(destination);
    int last_relative = 0;
    int path_count = 0;
    bool no_last_relative = true;
    bool loop_finished = false;

    path->PreviousCell = -1;
    path->UnravelCheckpoint = -1;

    FacingType edge_face = Facing_Adjust(facing, chirality);
    FacingType last_face = FACING_NONE;

    cell_t edge_cell = Cell_Get_Adjacent(start, edge_face);
    cell_t curr_cell = start;
    cell_t last_cell = -1;

    // Keep adding cells to the path so long as we have space in our buffer.
    while (path->Length < length) {
        int cell_cost;
        edge_face = facing;

        do {
            bool either_side_of_line = false;
            edge_face = Facing_Adjust(edge_face, chirality);

            // Handle Cardinal and Ordinal facings differently as ordinals are
            // a diagonal move.
            if ((edge_face & FACING_ORDINAL_TEST) != 0) {
                cell_t next_adj = Cell_Get_Adjacent(curr_cell, Facing_Adjust(edge_face, chirality));

                if (next_adj == destination) {
                    cell_cost = Passable_Cell(next_adj, Facing_Adjust(edge_face, chirality), threat, move);

                    if (cell_cost != 0) {
                        edge_face = Facing_Adjust(edge_face, chirality);
                        edge_cell = Cell_Get_Adjacent(curr_cell, edge_face);

                        break;
                    }
                }

                cell_t chk_cell = Cell_Get_Adjacent(curr_cell, edge_face);

                int relative =
                    Point_Relative_To_Line(Cell_Get_X(chk_cell), Cell_Get_Y(chk_cell), start_x, start_y, dest_x, dest_y);

                // In theory, Point_Relative_To_Line returns -ve for below and
                // +ve for above the line, so the bool shows if the last two
                // checked points are on opposite sides of the line.
                if (relative != 0 && !no_last_relative) {
                    either_side_of_line = (last_relative ^ relative) < 0;
                } else {
                    either_side_of_line = false;
                }

                if (either_side_of_line && path->Length > 0 && Opposite_Facing(edge_face) == path->Moves[path->Length - 1]) {
                    either_side_of_line = false;
                }
            }

            // We are facing the way we started, so we didn't find a route.
            if (edge_face == facing) {
                return false;
            }

            edge_cell = Cell_Get_Adjacent(curr_cell, edge_face);

            if (!either_side_of_line) {
                cell_cost = Passable_Cell(edge_cell, edge_face, threat, move);

                if (cell_cost != 0) {
                    break;
                }
            }

            if (edge_cell == destination) {
                loop_finished = true;
                break;
            }
        } while (true);

        if (!loop_finished) {
            if (!Register_Cell(path, edge_cell, edge_face, cell_cost, move)) {
                if (!Unravel_Loop(path, edge_cell, edge_face, start_x, start_y, dest_x, dest_y, move)) {
                    return false;
                }

                edge_face = Facing_Adjust(edge_face, chirality * 2);
            }

            int tmp_rel =
                Point_Relative_To_Line(Cell_Get_X(edge_cell), Cell_Get_Y(edge_cell), start_x, start_y, dest_x, dest_y);

            if (tmp_rel != 0) {
                last_relative = tmp_rel;
                no_last_relative = false;
            } else {
                no_last_relative = true;
            }

            // If we can't get round the obstruction in 400 moves, give up?
            if (++path_count == 400) {
                return false;
            }
        }

        // Success! We found a path around the obstacle.
        if (edge_cell == destination) {
            path->Moves[path->Length] = FACING_NONE;
            return true;
        }

        // No progress, we failed.
        if (edge_cell == last_cell && edge_face == last_face) {
            return false;
        }

        if (last_cell == -1) {
            last_cell = edge_cell;
            last_face = edge_face;
        }

        facing = Facing_Adjust(edge_face, chirality * -3);
        curr_cell = edge_cell;
    }

    return false;
}

/**
 * Cleans the path of none optimal moves.
 *
 * 0x004C0130
 */
int FootClass::Optimize_Moves(PathType *path, MoveType move)
{
    // This is Script_Unit_Pathfinder_Smoothen in OpenDUNE, looks like it does
    // exactly the same thing but cell pass check doesn't use move types.
    captainslog_assert(m_IsActive);
    captainslog_assert(move != MOVE_NONE);
    // captainslog_dbgassert(move < MOVE_COUNT, "move value is %d which exceed expected %d.\n", move, MOVE_COUNT);

    static FacingType _trans[] = { FACING_NORTH,
        FACING_NORTH,
        FACING_NORTH_EAST,
        FACING_EAST,
        FACING_SOUTH_EAST,
        FACING_FIXUP_MARK,
        FACING_NONE,
        FACING_NORTH };

    if (path == nullptr || path->Moves == nullptr) {
        return 0;
    }

    path->Moves[path->Length] = FACING_NONE;

    if (path->Length == 0) {
        return 0;
    }

    cell_t cell = path->StartCell;

    // If we have more than 1 move in our move queue, attempt to optimise
    if (path->Length > 1) {
        FacingType *next_move = &path->Moves[1];

        while (*next_move != FACING_NONE) {
            FacingType *last_move = next_move - 1;

            // Adjust last_move back to the last valid move.
            while (*last_move == FACING_FIXUP_MARK && last_move != path->Moves) {
                --last_move;
            }

            if (*last_move == FACING_FIXUP_MARK) {
                ++next_move;
                continue;
            }

            FacingType facing_diff = Reverse_Adjust(*next_move, *last_move);

            // Don't think this bit is possible given the math, original was
            // probably % 8 rather than & 7 for the adjusts.
            if ((int8_t)facing_diff < 0) {
                facing_diff = Facing_Adjust(facing_diff, FACING_COUNT);
            }

            FacingType trans_move = _trans[facing_diff];

            // Opposite directions so we can remove them.
            if (trans_move == FACING_SOUTH_EAST) {
                *last_move = FACING_FIXUP_MARK;
                *next_move++ = FACING_FIXUP_MARK;
                continue;
            }

            if (trans_move == FACING_NORTH) {
                cell = Cell_Get_Adjacent(cell, *last_move);
                ++next_move;
                continue;
            }

            FacingType dir;

            if ((*last_move & FACING_ORDINAL_TEST) != 0) { // == -1 or 1
                dir = Facing_Adjust(*last_move, (int8_t)trans_move >= FACING_NORTH ? FACING_NORTH_EAST : FACING_NONE);

                if (std::abs((int8_t)trans_move) == 1) {
                    if (Passable_Cell(Cell_Get_Adjacent(cell, dir), dir, -1, move) != 0) {
                        *next_move = dir;
                        *last_move = dir;
                    }

                    cell = Cell_Get_Adjacent(cell, *last_move);
                    ++next_move;
                    continue;
                }
            } else { // == 2
                dir = Facing_Adjust(*last_move, trans_move);
            }

            *next_move = dir;
            *last_move = FACING_FIXUP_MARK;

            // Adjust last_move back to the last valid move.
            while (*last_move == FACING_FIXUP_MARK && last_move != path->Moves) {
                --last_move;
            }

            if (*last_move == FACING_FIXUP_MARK) {
                cell = path->StartCell;
            } else {
                cell = Cell_Get_Adjacent(cell, Facing_Adjust(*last_move, FACING_SOUTH));
            }
        }
    }

    path->Score = 0;
    path->Length = 0;
    FacingType *moves = path->Moves;
    cell = path->StartCell;

    // Build the optimised path.
    for (FacingType *m = path->Moves; *m != FACING_NONE; ++m) {
        if (*m == FACING_FIXUP_MARK) {
            continue;
        }

        cell = Cell_Get_Adjacent(cell, *m);
        path->Score += Passable_Cell(cell, *m, -1, move);
        ++path->Length;
        *moves++ = *m;
    }

    ++path->Length;
    *moves = FACING_NONE;

    return path->Length;
}

/**
 * Looks like it finds the safest cell within a range?
 *
 * 0x004C0570
 */
cell_t FootClass::Safety_Point(cell_t start_cell, cell_t end_cell, int start, int end)
{
#ifdef GAME_DLL
    cell_t (*func)(FootClass *, cell_t, cell_t, int, int) =
        reinterpret_cast<cell_t (*)(FootClass *, cell_t, cell_t, int, int)>(0x004C037C);
    return func(this, start_cell, end_cell, start, end);
#else
    FacingType adj_facing = SS_41B710(Opposite_Facing(Direction_To_Facing(Cell_Direction8(start_cell, end_cell))), 1);

    for (int i = start; i < end; ++i) {
        cell_t cell = end_cell;

        for (int j = 0; j < i; ++j) {
            cell = Cell_Get_Adjacent(cell, adj_facing);
        }

        // If adj_facing is ordinal
        if ((adj_facing & 1) != 0) {
            for (int j = 0; j <= 2 * i; ++j) {
                cell = Cell_Get_Adjacent(cell, SS_41B73C(adj_facing, 3));

                if (Can_Enter_Cell(cell)) {
                    return cell;
                }
            }
        } else {
            for (int j = 0; j <= i; ++j) {
                cell = Cell_Get_Adjacent(cell, SS_41B73C(adj_facing, 4));

                if (Can_Enter_Cell(cell)) {
                    return cell;
                }
            }

            for (int j = 0; j <= i; ++j) {
                cell = Cell_Get_Adjacent(cell, SS_41B73C(adj_facing, 2));

                if (Can_Enter_Cell(cell)) {
                    return cell;
                }
            }
        }
    }

    return -1;
#endif
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
        if (Distance_To_Cell(cell) > 256) {
            move = MOVE_MOVING_BLOCK;
        }
    }

    if (canmove <= move) {
        if (g_Session.Game_To_Play() != GAME_CAMPAIGN || threat == -1
            || Coord_Distance(Cell_To_Coord(cell), Cell_To_Coord(g_DestLocation)) <= 1280
            || g_Map.Cell_Threat(cell, Owner()) <= threat) {
            return _value[canmove];
        }
    }

    return 0;
}

/**
 * Helper function to determine how far off a line a point lies?
 *
 * 0x004BF470
 */
int FootClass::Point_Relative_To_Line(int px, int py, int sx, int sy, int ex, int ey)
{
    return (px - ex) * (sy - ey) - (sx - ex) * (py - ey);
}

/**
 * Currently not used, but can be hooked over Find_Path to check our code matches original.
 */
PathType *FootClass::Find_Path_Wrapper(cell_t dest, FacingType *buffer, int length, MoveType move)
{
    captainslog_debug("***Find_Path_Wrapper entered***");
    PathType *real;
    PathType *test;
    FacingType *buffer2 = new FacingType[length];
    unsigned test_overlap[512];

    // call our implamentation
    test = Find_Path(dest, buffer2, length, move);
    memcpy(test_overlap, test->Overlap, sizeof(test_overlap));

    // call the original Find_Path
    PathType *(*func)(FootClass *, cell_t, FacingType *, int, MoveType) =
        reinterpret_cast<PathType *(*)(FootClass *, cell_t, FacingType *, int, MoveType)>(0x004BF77C);
    real = func(this, dest, buffer, length, move);

    bool fail = false;
    if (real->StartCell != test->StartCell) {
        captainslog_debug(
            "ERROR: Find_Path_Wrapper StartCells don't match real %hd test %hd", real->StartCell, test->StartCell);
        fail = true;
    }
    if (real->Score != test->Score) {
        captainslog_debug("ERROR: Find_Path_Wrapper Scores don't match real %d test %d", real->Score, test->Score);
        fail = true;
    }
    if (real->Length != test->Length) {
        captainslog_debug("ERROR: Find_Path_Wrapper Lengths don't match real %d test %d", real->Length, test->Length);
        fail = true;
    }
    if (real->PreviousCell != test->PreviousCell) {
        captainslog_debug(
            "ERROR: Find_Path_Wrapper PreviousCells don't match real %hd test %hd", real->PreviousCell, test->PreviousCell);
        fail = true;
    }
    if (real->UnravelCheckpoint != test->UnravelCheckpoint) {
        captainslog_debug("ERROR: Find_Path_Wrapper UnravelCheckpoints don't match real %hd test %hd",
            real->UnravelCheckpoint,
            test->UnravelCheckpoint);
        fail = true;
    }

    // if all previous tests went fine check the moves and overlap for missmatches
    if (fail == false) {
        int num1 = memcmp(real->Moves, test->Moves, test->Length > real->Length ? test->Length : real->Length);
        if (num1 != 0) {
            captainslog_debug("ERROR: Find_Path_Wrapper Moves don't match");
        }
        int num2 = memcmp(real->Overlap, test_overlap, 2048);
        if (num2 != 0) {
            captainslog_debug("ERROR: Find_Path_Wrapper Overlaps don't match");
        }
    }

    // clear test's buffer
    delete[] buffer2;
    captainslog_debug("***Find_Path_Wrapper exit***");
    return real;
}

BOOL FootClass::Is_Allowed_To_Leave_Map() const
{
#ifdef GAME_DLL
    BOOL (*func)(const FootClass *) = reinterpret_cast<BOOL (*)(const FootClass *)>(0x004C36A4);
    return func(this);
#else
    if (!m_LockedOnMap) {
        return false;
    }
    if (!m_IsALoner && m_Mission != MISSION_RETREAT) {
        /*
        if(!m_Team->Is_Leaving_Map()) {
            return false;
        }
        */
    }
    return true;
#endif
}

/**
 *
 *
 * 0x004C37A0
 */
BOOL FootClass::Is_On_Priority_Mission()
{
    return m_Mission == MISSION_ENTER;
}
