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
#include "aircraft.h"

#ifndef GAME_DLL
TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

AircraftClass::AircraftClass(AircraftType type, HousesType house) :
    FootClass(RTTI_AIRCRAFT, g_Aircraft.ID(this), house)
{
}

AircraftClass::AircraftClass(const AircraftClass &that) :
    FootClass(that)
{
}

AircraftClass::AircraftClass(const NoInitClass &noinit) :
    FootClass(noinit)
{
}

AircraftClass::~AircraftClass()
{
}

MoveType AircraftClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00421918, MoveType, const AircraftClass *, cell_t, FacingType);
    return func(this, cellnum, facing);
#else
    return MOVE_NONE;
#endif
}

void AircraftClass::AI()
{}

ActionType AircraftClass::What_Action(ObjectClass * object) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004202F0, ActionType, const AircraftClass *, ObjectClass *);
    return func(this, object);
#else
    return ACTION_NONE;
#endif
}

ActionType AircraftClass::What_Action(cell_t cellnum) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00420588, ActionType, const AircraftClass *, cell_t);
    return func(this, cellnum);
#else
    return ACTION_NONE;
#endif
}

LayerType AircraftClass::In_Which_Layer() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00423BEC, LayerType, const AircraftClass *);
    return func(this);
#else
    return LAYER_NONE;
#endif
}

/**
 * @brief
 *
 * @address 0x0041EA98
 */
coord_t AircraftClass::Sort_Y() const
{
    //TODO figure out if we can represent the hex val as a from pixels or coord conversion
    return m_Coord + 0x800000;
}

BOOL AircraftClass::Unlimbo(coord_t coord, DirType dir)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041CEF0, BOOL, AircraftClass *, coord_t, DirType);
    return func(this, coord, dir);
#else
    return false;
#endif
}

int AircraftClass::Exit_Object(TechnoClass * object)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041EC50, int, const AircraftClass *, ObjectClass *);
    return func(this, object);
#else
    return 0;
#endif
}

const int16_t *AircraftClass::Overlap_List(BOOL a1) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041E238, const int16_t *, const AircraftClass *, BOOL);
    return func(this, a1);
#else
    return nullptr;
#endif
}

void AircraftClass::Draw_It(int x, int y, WindowNumberType window) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041D25C, void, const AircraftClass *, int, int, WindowNumberType);
    func(this, x, y, window);
#endif
}

void AircraftClass::Look(BOOL a1)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00423C38, void, AircraftClass *, BOOL);
    func(this, a1);
#endif
}

void AircraftClass::Active_Click_With(ActionType action, ObjectClass * object) 
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00420208, void, AircraftClass *, ActionType, ObjectClass *);
    func(this, action, object);
#endif
}

/**
 * @brief
 *
 * @address 0x00420294
 */
void AircraftClass::Active_Click_With(ActionType action, cell_t cellnum)
{
    FootClass::Active_Click_With(action, cellnum);
}

DamageResultType AircraftClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041F01C, DamageResultType, AircraftClass *, int &, int, WarheadType, TechnoClass *, BOOL);
    return func(this, damage, a2, warhead, object, a5);
#else
    return DAMAGE_NONE;
#endif
}

void AircraftClass::Scatter(coord_t coord, int a2, BOOL a3)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00422980, void, AircraftClass *, coord_t, int, BOOL);
    func(this, coord, a2, a3);
#endif
}

/**
 * @brief
 *
 * @address 0x00423B50
 */
void AircraftClass::Per_Cell_Process(PCPType pcp)
{
    FootClass::Per_Cell_Process(pcp);
}

RadioMessageType AircraftClass::Receive_Message(RadioClass * radio, RadioMessageType message, target_t & target) 
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00420F5C, RadioMessageType, AircraftClass *, RadioClass *, RadioMessageType, target_t &);
    return func(this, radio, message, target);
#else
    return RADIO_NONE;
#endif
}

int AircraftClass::Mission_Attack()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004206DC, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Guard()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004229F8, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Guard_Area()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00422FC8, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Hunt()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041D85C, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Move()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041F1F4, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Retreat()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041EAD0, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Unload()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041E588, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Enter()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00421E00, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 * @brief
 *
 * @address 0x00423E60
 */
DirType AircraftClass::Turret_Facing() const
{
    return m_BDir.Get_Current();
}

DirType AircraftClass::Desired_Load_Dir(ObjectClass * object, cell_t & cellnum) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00421464, DirType, const AircraftClass *, ObjectClass *, cell_t &);
    return func(this, object, cellnum);
#else
    return DIR_NONE;
#endif
}

/**
 * @brief
 *
 * @address 0x00422780
 */
DirType AircraftClass::Fire_Direction() const
{
    return m_BDir.Get_Current();
}

int AircraftClass::Pip_Count() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00421D04, int, const AircraftClass *);
    return func(this);
#else
    //TODO
    if (Class_Of().Max_Passengers() > 0) {
        return m_Cargo.Cargo_Count();
    }
    return 0;
#endif
}

void AircraftClass::Response_Select()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00423154, void, AircraftClass *);
    func(this);
#endif
}

void AircraftClass::Response_Move()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004230FC, void, AircraftClass *);
    func(this);
#endif
}

void AircraftClass::Response_Attack()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004230A4, void, AircraftClass *);
    func(this);
#endif
}

FireErrorType AircraftClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004231AC, FireErrorType, const AircraftClass *, target_t, WeaponSlotType);
    return func(this, target, weapon);
#else
    return FIRE_NONE;
#endif
}

BulletClass *AircraftClass::Fire_At(target_t target, WeaponSlotType weapon)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041EE90, BulletClass *, AircraftClass *, target_t, WeaponSlotType);
    return func(this, target, weapon);
#else
    return nullptr;
#endif
}

void AircraftClass::Assign_Destination(target_t dest)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00423B60, void, AircraftClass *, target_t);
    func(this, dest);
#endif
}

void AircraftClass::Enter_Idle_Mode(BOOL a1)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041FA08, void, AircraftClass *, BOOL);
    func(this, a1);
#endif
}

void AircraftClass::Set_Speed(int speed)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0042263C, void, AircraftClass *, int);
    func(this, speed);
#endif
/*
    FootClass::Set_Speed(speed);
    //FlyControl.Fly_Speed();
*/
}
