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

coord_t UnitClass::Sort_Y() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005799E0, coord_t, const UnitClass *);
    return func(this);
#else
    return 0;
#endif
}

BOOL UnitClass::Limbo()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00581730, BOOL, UnitClass *);
    return func(this);
#else
    return false;
#endif
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

void UnitClass::Active_Click_With(ActionType action, ObjectClass *object)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057B40C, void, UnitClass *, ActionType, ObjectClass *);
    func(this, action, object);
#endif
}

void UnitClass::Active_Click_With(ActionType action, cell_t cellnum)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057B4B0, void, UnitClass *, ActionType, cell_t);
    func(this, action, cellnum);
#endif
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

void UnitClass::Scatter(coord_t coord, int a2, int a3)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00581644, void, UnitClass *, coord_t, int, int);
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

int UnitClass::Mission_Move()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057FD4C, int, UnitClass *);
    return func(this);
#else
    return 0;
#endif
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

InfantryType UnitClass::Crew_Type() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00580290, InfantryType, const UnitClass *);
    return func(this);
#else
    return INFANTRY_NONE;
#endif
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

BOOL UnitClass::Start_Driver(coord_t &dest)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0057F398, BOOL, UnitClass *, coord_t &);
    return func(this, dest);
#else
    return false;
#endif
}

BOOL UnitClass::Offload_Ore_Bail()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005808D8, BOOL, UnitClass *);
    return func(this);
#else
    return false;
#endif
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

BOOL UnitClass::Ok_To_Move(DirType dir)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005804E8, BOOL, UnitClass *, DirType);
    return func(this, dir);
#else
    return false;
#endif
}
