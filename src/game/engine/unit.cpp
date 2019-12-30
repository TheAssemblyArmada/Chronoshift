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
#include "scenario.h"
#include "team.h"
#include "iomap.h"
#include "rules.h"
#include "target.h"

#ifndef GAME_DLL
TFixedIHeapClass<UnitClass> g_Units;
#endif

UnitClass::UnitClass(UnitType type, HousesType house) :
    DriveClass(RTTI_UNIT, g_Units.ID(this), house)
{
}

UnitClass::UnitClass(const UnitClass &that) :
    DriveClass(that)
{
}

UnitClass::UnitClass(const NoInitClass &noinit) :
    DriveClass(noinit)
{
}

UnitClass::~UnitClass()
{
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
    int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057AAE8, DamageResultType, UnitClass *, int &, int, WarheadType, TechnoClass *, BOOL);
    return func(this, damage, a2, warhead, object, a5);
#else
    return DAMAGE_NONE;
#endif
}

void UnitClass::Scatter(coord_t coord, BOOL a2, BOOL a3)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00581644, void, UnitClass *, coord_t, BOOL, BOOL);
    func(this, coord, a2, a3);
#endif
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

int UnitClass::Mission_Guard()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057FB98, int, UnitClass *);
    return func(this);
#else
    return 0;
#endif
}

int UnitClass::Mission_Guard_Area()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005819E8, int, UnitClass *);
    return func(this);
#else
    return 0;
#endif
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

DirType UnitClass::Turret_Facing() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004CDB40, DirType, const UnitClass *);
    return func(this);
#else
    return DIR_NONE;
#endif
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

DirType UnitClass::Fire_Direction() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005803C4, DirType, const UnitClass *);
    return func(this);
#else
    return DIR_NONE;
#endif
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

    if (g_Scen.Get_Random_Value(0, 99) < 50) {
        return INFANTRY_C1;
    }
    return INFANTRY_C7;
}

fixed_t UnitClass::Ore_Load() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00580874, fixed_t, const UnitClass *);
    return func(this);
#else
    return fixed_t(0);
#endif
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

FireErrorType UnitClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00580554, FireErrorType, const UnitClass *, target_t, WeaponSlotType);
    return func(this, target, weapon);
#else
    return FIRE_NONE;
#endif
}

target_t UnitClass::Greatest_Threat(ThreatType threat)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00580F14, target_t, UnitClass *, ThreatType);
    return func(this, threat);
#else
    return 0;
#endif
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

void UnitClass::Approach_Target()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005808FC, void, UnitClass *);
    func(this);
#endif
}

void UnitClass::Overrun_Cell(cell_t cell, int a2)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005809EC, void, UnitClass *, cell_t, int);
    func(this, cell, a2);
#endif
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
    if (dir - m_TurretFacing.Get_Current() != 0) {
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
