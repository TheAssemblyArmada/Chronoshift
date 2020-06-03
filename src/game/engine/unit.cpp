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
#include "unit.h"
#include "anim.h"
#include "fixedmult.h"
#include "gameoptions.h"
#include "infantry.h"
#include "iomap.h"
#include "missioncontrol.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "target.h"
#include "team.h"

#ifndef GAME_DLL
TFixedIHeapClass<UnitClass> g_Units;
#endif

UnitClass::UnitClass(UnitType type, HousesType house) :
    DriveClass(RTTI_UNIT, g_Units.ID(this), house),
    m_Class(g_UnitTypes.Ptr(type)),
    m_FlagOwner(HOUSES_NONE),
    m_IsDumping(false),
    m_Gold(0),
    m_Gems(0),
    m_ToScatter(false),
    m_BailCount(0),
    m_GapGenCellTracker(-1),
    m_GapGenCenterCell(0),
    m_V2RearmDelayTimer(0),
    m_TurretFacing()
{
    m_OwnerHouse->Tracking_Add(this);

    m_Ammo = Class_Of().Get_Ammo();
    m_Cloakable = Class_Of().Is_Cloakable();

    if (Class_Of().Is_Viceroid()) {
         m_AnimStage.Set_Rate(g_Options.Normalize_Delay(3));
    }

    m_Bit2_16 = !Class_Of().Is_Two_Shooter();
    m_Health = Class_Of().Get_Strength();

}

UnitClass::UnitClass(const UnitClass &that) :
    DriveClass(that),
    m_Class(that.m_Class),
    m_FlagOwner(that.m_FlagOwner),
    m_IsDumping(that.m_IsDumping),
    m_Gold(that.m_Gold),
    m_Gems(that.m_Gems),
    m_ToScatter(that.m_ToScatter),
    m_BailCount(that.m_BailCount),
    m_GapGenCellTracker(that.m_GapGenCellTracker),
    m_GapGenCenterCell(that.m_GapGenCenterCell),
    m_V2RearmDelayTimer(that.m_V2RearmDelayTimer),
    m_TurretFacing(that.m_TurretFacing)
{
}

UnitClass::UnitClass(const NoInitClass &noinit) :
    DriveClass(noinit)
{
}

UnitClass::~UnitClass()
{
    if (g_GameActive) {
        if (m_Team != nullptr) {
            m_Team->Remove(this);
            m_Team = nullptr;
        }

        m_OwnerHouse->Tracking_Remove(this);

        Destroy_Cargo();

        Limbo();
    }

    m_Class = nullptr;
    m_FlagOwner = HOUSES_NONE;
}

MoveType UnitClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057EB30, MoveType, const UnitClass *, cell_t, FacingType);
    return func(this, cellnum, facing);
#else
    return MOVE_NONE;
#endif
}

ActionType UnitClass::What_Action(ObjectClass *object) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057F3D0, ActionType, const UnitClass *, ObjectClass *);
    return func(this, object);
#else
    return ACTION_NONE;
#endif
}

ActionType UnitClass::What_Action(cell_t cellnum) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057FA00, ActionType, const UnitClass *, cell_t);
    return func(this, cellnum);
#else
    return ACTION_NONE;
#endif
}

/**
 *
 *
 */
coord_t UnitClass::Sort_Y() const
{
    return m_Coord + 0x800000;
}

/**
 *
 *
 */
BOOL UnitClass::Limbo()
{
    if (!DriveClass::Limbo()) {
        return false;
    }
    if (m_FlagOwner != HOUSES_NONE) {
        HouseClass::As_Pointer(m_FlagOwner)->Flag_Attach(Get_Cell());
    }
    return true;
}

BOOL UnitClass::Unlimbo(coord_t coord, DirType dir)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057A9A8, BOOL, UnitClass *, coord_t, DirType);
    return func(this, coord, dir);
#else
    return false;
#endif
}

const int16_t *UnitClass::Overlap_List(BOOL a1) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057E8AC, const int16_t *, const UnitClass *, BOOL);
    return func(this, a1);
#else
    return nullptr;
#endif
}

void UnitClass::Draw_It(int x_pos, int y_pos, WindowNumberType window) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057C9C4, void, const UnitClass *, int, int, WindowNumberType);
    func(this, x_pos, y_pos, window);
#endif
}

/**
 *
 *
 */
void UnitClass::Active_Click_With(ActionType action, ObjectClass *object)
{
    if (What_Action(object) != action) {
        action = What_Action(object);
        if (action == ACTION_CAPTURE || action == ACTION_SABOTAGE) {
            action = ACTION_ATTACK;
        }
        if (action == ACTION_ENTER) {
            action = ACTION_MOVE;
        }
    }
    if (object != this || action != ACTION_NO_MOVE) {
        if (What_Type() != UNIT_MAD_TANK || !m_IsDumping && !m_ToScatter) {
            DriveClass::Active_Click_With(action, object);
        }
    }
}

/**
 *
 *
 */
void UnitClass::Active_Click_With(ActionType action, cell_t cellnum)
{
    if (What_Type() != UNIT_MAD_TANK || !m_IsDumping && !m_ToScatter) {
        DriveClass::Active_Click_With(action, cellnum);
    }
}

DamageResultType UnitClass::Take_Damage(
    int &damage, int distance, WarheadType warhead, TechnoClass *object, BOOL force)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057AAE8, DamageResultType, UnitClass *, int &, int, WarheadType, TechnoClass *, BOOL);
    return func(this, damage, distance, warhead, object, force);
#else
    return DAMAGE_NONE;
#endif
}

/**
 *
 *
 */
void UnitClass::Scatter(coord_t coord, BOOL a2, BOOL a3)
{
    // Note this if prevents sleeping units from being worken up by a scatter order.
    // Only UnitClass has this, other object classes don't have such check in retail RA
    if (m_Mission != MISSION_SLEEP) {
        if (m_Mission != MISSION_STICKY && m_Mission != MISSION_UNLOAD
            && (Get_Mission_Control(m_Mission).Can_Scatter() || a2) && m_Facing.Has_Not_Changed()
            && (!Target_Legal(m_NavCom) || a3)) {
            if (coord == 0) {
                Assign_Destination(::As_Target(g_Map.Nearby_Location(Get_Cell(), Class_Of().Get_Speed())));
            } else {
                DriveClass::Scatter(coord, a2, a3);
            }
        }
    }
}

void UnitClass::Per_Cell_Process(PCPType pcp)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057BD10, void, UnitClass *, PCPType);
    func(this, pcp);
#endif
}

RadioMessageType UnitClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057A354, RadioMessageType, UnitClass *, RadioClass *, RadioMessageType, target_t &);
    return func(this, radio, message, target);
#else
    return RADIO_NONE;
#endif
}

/**
 *
 *
 */
int UnitClass::Mission_Guard()
{
    if (!m_OwnerHouse->Is_Human() && Class_Of().Is_Harvester() && m_OwnerHouse->Get_Quantity(BUILDING_PROC) > 0
        && !m_OwnerHouse->Short_On_Ore()) {
        Assign_Mission(MISSION_HARVEST);
        return 1;
    }

    if (What_Type() == UNIT_MCV) {
        if (m_OwnerHouse->Auto_Base_AI()) {
            Assign_Mission(MISSION_UNLOAD);
            return (900 * Get_Mission_Control(m_Mission).Get_Rate()) + g_Scen.Get_Random_Value(0, 2);
        }
    }

    return DriveClass::Mission_Guard();
}

/**
 *
 *
 */
int UnitClass::Mission_Guard_Area()
{
    if (g_Session.Game_To_Play() == GAME_CAMPAIGN) {
        return DriveClass::Mission_Guard_Area();
    }

    if (What_Type() != UNIT_APC && What_Type() != UNIT_PHASE) {
        return DriveClass::Mission_Guard_Area();
    }

    if (Target_Legal(m_TarCom) || m_Radio != nullptr) {
        return DriveClass::Mission_Guard_Area();
    }

    if (m_OwnerHouse->Which_Zone(this) == ZONE_NONE) {
        return DriveClass::Mission_Guard_Area();
    }

    if (!m_OwnerHouse->Is_Human()) {
        return DriveClass::Mission_Guard_Area();
    }

    int passengers = Class_Of().Max_Passengers() - m_Cargo.Cargo_Count();

    for (int i = 0; i < g_Infantry.Count() && passengers != 0; ++i)  {
        InfantryClass *iptr = &g_Infantry[i];

        if (iptr != nullptr && iptr->Is_Active() && !iptr->In_Limbo() && iptr->Get_Health() > 0) {
            if (iptr->Get_Owner_House() == m_OwnerHouse && !Target_Legal(m_TarCom) && !Target_Legal(m_NavCom)) {
                if (Direction_To_Coord(iptr->Target_Coord()) < (CELL_LEPTONS * 7)) {
                    if (iptr->Get_Mission() == MISSION_GUARD || iptr->Get_Mission() == MISSION_AREA_GUARD) {
                        iptr->Assign_Mission(MISSION_ENTER);
                        --passengers;
                        iptr->Set_Archive(As_Target());
                    }
                }
            }
        }
    }

    return DriveClass::Mission_Guard_Area();
}

int UnitClass::Mission_Harvest()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057E2D0, int, UnitClass *);
    return func(this);
#else
    return 0;
#endif
}

int UnitClass::Mission_Hunt()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057E7D0, int, UnitClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 *
 *
 */
int UnitClass::Mission_Move()
{
    m_IsHarvesting = false;
    if (m_Door.Is_Closed()) {
        APC_Close_Door();
    }
    return DriveClass::Mission_Move();
}

int UnitClass::Mission_Unload()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057D38C, int, UnitClass *);
    return func(this);
#else
    return 0;
#endif
}

int UnitClass::Mission_Repair()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005802F4, int, UnitClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 *
 *
 */
DirType UnitClass::Turret_Facing() const
{
    if (Class_Of().Is_Turret_Equipped()) {
        return m_TurretFacing.Get_Current();
    }

    return m_Facing.Get_Current();
}

DirType UnitClass::Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057FD8C, DirType, const UnitClass *, ObjectClass *, cell_t &);
    return func(this, object, cellnum);
#else
    return DIR_NONE;
#endif
}

/**
 *
 *
 */
DirType UnitClass::Fire_Direction() const
{
    if (Class_Of().Is_Turret_Equipped()) {
        // Special logics for the V2 facing.
        if (What_Type() == UNIT_V2RL) {
            // TODO: figure out what exactly is going on here and document it.
            int dif1 = std::abs(m_TurretFacing.Difference_From_Current(DIR_EAST));
            int dif2 = std::abs(m_TurretFacing.Difference_From_Current(DIR_WEST));

            // TODO: Old TD era fixed maths, need to be replaced with fixed_t if possible.
            int adj = fixed_mult(DIR_EAST - std::min(dif1, dif2), std::abs(m_TurretFacing.Difference_From_Current(DIR_NORTH)));

            if (m_TurretFacing.Difference_From_Current(DIR_NORTH) < 0) {
                return m_TurretFacing.Get_Current() - adj;
            } else {
                return m_TurretFacing.Get_Current() + adj;
            }
        }

        // Not a special unit so get its turret facing.
        return m_TurretFacing.Get_Current();
    }

    // Got no turret so get the unit facing.
    return DriveClass::Fire_Direction();
}

/**
 *
 *
 */
InfantryType UnitClass::Crew_Type() const
{
    if (Class_Of().Get_Weapon(WEAPON_SLOT_PRIMARY) != nullptr) {
        return DriveClass::Crew_Type();
    }

    if (g_Scen.Check_Random_Chance(50)) {
        return INFANTRY_C1;
    }
    return INFANTRY_C7;
}

/**
 *
 *
 */
fixed_t UnitClass::Ore_Load() const
{
    if (What_Type() == UNIT_HARVESTER) {
        return fixed_t(m_BailCount, g_Rule.Bail_Count());   
    }

    return fixed_t(0);
}

int UnitClass::Pip_Count() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00580004, int, const UnitClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 *
 *
 */
FireErrorType UnitClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    FireErrorType fireerror = DriveClass::Can_Fire(target, weapon);

    if (fireerror != FIRE_OK) {
        return fireerror;
    }

    WeaponTypeClass *wptr = Class_Of().Get_Weapon(weapon);

    if (Class_Of().No_Moving_Fire() && Target_Legal(m_NavCom)) {
        return FIRE_MOVING;
    }

    if (!m_Firing && m_Rotating && wptr->Get_Projectile()->Rate_Of_Turn() == 0) {
        return FIRE_ROTATING;
    }

    // TODO figure out what exactly is going on here and document it.
    DirType dir = Direction_To_Target(target);

    int absdir = std::abs(Class_Of().Is_Turret_Equipped() ? m_TurretFacing.Difference_From_Current(dir) : m_Facing.Difference_From_Current(dir));
    
    if (wptr->Get_Projectile()->Rate_Of_Turn()) {
        absdir /= 4;
    }

    if (absdir >= 8) {
        return FIRE_FACING;
    }

    return DriveClass::Can_Fire(target, weapon);
}

/**
 *
 *
 */
target_t UnitClass::Greatest_Threat(ThreatType threat)
{
    WeaponTypeClass *wptr = Class_Of().Get_Weapon(WEAPON_SLOT_PRIMARY);

    if (wptr != nullptr) {
        threat |= wptr->Allowed_Threats();
    }

    wptr = Class_Of().Get_Weapon(WEAPON_SLOT_SECONDARY);

    if (wptr != nullptr) {
        threat |= wptr->Allowed_Threats();
    }

    return DriveClass::Greatest_Threat(threat);
}

BulletClass *UnitClass::Fire_At(target_t target, WeaponSlotType weapon)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00580554, BulletClass *, UnitClass *, target_t, WeaponSlotType);
    return func(this, target, weapon);
#else
    return nullptr;
#endif
}

void UnitClass::Assign_Destination(target_t dest)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00580B94, void, UnitClass *, target_t);
    func(this, dest);
#endif
}

void UnitClass::Enter_Idle_Mode(BOOL a1)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057B508, void, UnitClass *, BOOL);
    func(this, a1);
#endif
}

/**
 *
 *
 */
BOOL UnitClass::Start_Driver(coord_t &dest)
{
    if (!DriveClass::Start_Driver(dest)) {
        return false;
    }
    if (m_IsActive) {
        Mark_Track(dest, MARK_PUT);
        return true;
    }
    return false;
}

/**
 *
 *
 */
BOOL UnitClass::Offload_Ore_Bail()
{
    if (m_BailCount != 0) {
        --m_BailCount;
    }
    return false;
}

/**
 *
 *
 */
void UnitClass::Approach_Target()
{
    // Is the owner of the current object not human, is its target legal and the current object has nowhere to go?
    if (!m_OwnerHouse->Is_Human() && Target_Legal(m_TarCom) && !Target_Legal(m_NavCom)) {
        // Is the current unit a crusher and within crushing distance?
        if (Class_Of().Crusher() && Distance_To_Target(m_TarCom) < g_Rule.Crush_Distance()) {
            UnitClass *uptr = (UnitClass *)As_Techno(m_TarCom);

            // Do we have a target and is it crushable?
            if (uptr != nullptr && uptr->Class_Of().Is_Crushable()) {
                // Assign target's location as navigation destination so we can crush it.
                Assign_Destination(m_TarCom);
                return;
            }
        }
    }

    // just handle target approach as we normally do
    DriveClass::Approach_Target();
}

/**
 *
 *
 */
void UnitClass::Overrun_Cell(cell_t cellnum, BOOL a2)
{
    CellClass &cell = g_Map[cellnum];

    if (Class_Of().Crusher()) {
        if (a2) {
            if (cell.Check_Occupants(OCCUPANT_INFANTRY)) {
                cell.Incoming(0, true);
            }
        } else {
            bool uncloak = false;

            ObjectClass *optr = cell.Get_Occupier();

            while (optr != nullptr) {
                // Is the object crushable, is it a enemy and is it less than a half cell away?
                if (optr->Class_Of().Is_Crushable() && !m_OwnerHouse->Is_Ally(optr)
                    && Distance_To_Object_Center(optr) < 128) {
                    ObjectClass *next = optr->Get_Next();

                    uncloak = true;

                    Sound_Effect(VOC_SQUISHY2, m_Coord);

                    if (m_Height == 0) {
                        AnimClass *aptr = new AnimClass(ANIM_CORPSE1, optr->Center_Coord());
                        captainslog_assert(aptr != nullptr);
                    }

                    optr->Record_The_Kill(this);
                    optr->Mark(MARK_REMOVE);
                    optr->Limbo();

                    delete optr;

                    optr = next;
                } else {
                    optr = optr->Get_Next();
                }
            }

            if (uncloak) {
                Do_Uncloak();
            }
        }
    }
}

/**
 *
 *
 */
BOOL UnitClass::Ok_To_Move(DirType dir)
{
    if (!Class_Of().Is_Bit32()) {
        return true;
    }

    if (m_Rotating) {
        return false;
    }

    if (m_TurretFacing.Difference_From_Current(dir) != 0) {
        m_TurretFacing.Set_Desired(dir);
        return false;
    }

    return true;
}

/**
 *
 *
 */
BOOL UnitClass::Edge_Of_World_AI()
{
    if (m_Mission == MISSION_GUARD && !g_Map.In_Radar(Get_Cell()) && m_LockedOnMap) {
        if (m_Team != nullptr) {
            m_Team->Set_Bit2_4(true);
        }
        Stun();
        delete this;
        return true;
    }
    return false;
}

/**
 *
 *
 */
BOOL UnitClass::Flag_Attach(HousesType house)
{
    if (house != HOUSES_NONE && m_FlagOwner == HOUSES_NONE) {
        m_FlagOwner = house;
        Mark(MARK_REDRAW);
        return true;
    }
    return false;
}

/**
 *
 *
 */
BOOL UnitClass::Flag_Remove()
{
    if (m_FlagOwner != HOUSES_NONE) {
        m_FlagOwner = HOUSES_NONE;
        Mark(MARK_REDRAW);
        return true;
    }
    return false;
}

/**
 *
 *
 */
void UnitClass::APC_Close_Door()
{
    m_Door.Close_Door(10, 2);
}

/**
 *
 *
 */
void UnitClass::APC_Open_Door()
{
    int delay;

    if (!m_Moving && !m_Rotating) {
        DirType dir = m_Facing.Get_Current();
        if (dir == DIR_NORTH_WEST || dir == DIR_NORTH_EAST) {
            delay = 10;
        } else {
            delay = 1;
        }
        m_Door.Open_Door(delay, 2);
    }
}

/**
 *
 *
 */
int UnitClass::Credit_Load()
{
    return g_Rule.Get_Gold_Value() * m_Gold + g_Rule.Get_Gem_Value() * m_Gems;
}

BOOL UnitClass::Ore_Check(short &cellnum, int a2, int a3)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057CD9C, BOOL, UnitClass *, short &, int, int);
    return func(this, cellnum, a2, a3);
#else
    /*
    cell_t check_cell;

    if (g_Scen.Game_To_Play() == GAME_CAMPAIGN && m_PlayerOwned && !Map[check_cell].Is_Visible()) {
        return false;
    }

    cell_t cell = Get_Cell();
    CellClass *cptr = &Map[cell];
    if (cptr->Get_Zone(MZONE_NORMAL) == Class_Of().Get_Movement_Zone() && !Cell_Techno(0, 0) && Map[check_cell].Get_Land() ==
    LAND_ORE) { return true;
    }
    */
    return false;
#endif
}

/**
 * Finds nearest ore location and directs the unit to it, return value is if its on ore already.
 *
 */
BOOL UnitClass::Goto_Ore(int scan_radius)
{
    if (Target_Legal(m_NavCom)) {
        return false;
    }

    // is the cell im under ore?
    cell_t cell = Get_Cell();
    if (g_Map[cell].Get_Land() == LAND_ORE) {
        return true;
    }

    cell_t adj_cell = -1;
    for (int i = 1; i < scan_radius; ++i) {
        for (int j = -i; j <= i; ++j) {
            adj_cell = cell;
            if (Ore_Check(adj_cell, j, -i)) {
                Assign_Destination(::As_Target(adj_cell));
                return false;
            }

            adj_cell = cell;
            if (Ore_Check(adj_cell, j, i)) {
                Assign_Destination(::As_Target(adj_cell));
                return false;
            }

            adj_cell = cell;
            if (Ore_Check(adj_cell, -i, j)) {
                Assign_Destination(::As_Target(adj_cell));
                return false;
            }

            adj_cell = cell;
            if (Ore_Check(adj_cell, i, j)) {
                Assign_Destination(::As_Target(adj_cell));
                return false;
            }
        }
    }
    return false;
}
