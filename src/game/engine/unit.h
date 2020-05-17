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

#ifndef UNIT_H
#define UNIT_H

#include "always.h"
#include "drive.h"
#include "unittype.h"

class UnitClass : public DriveClass
{
public:
    UnitClass(UnitType type, HousesType house);
    UnitClass(const UnitClass &that);
    UnitClass(const NoInitClass &noinit);
    virtual ~UnitClass();

    // AbstractClass
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const final;

    // ObjectClass
    virtual ActionType What_Action(ObjectClass *object) const final;
    virtual ActionType What_Action(cell_t cellnum) const final;
    virtual const UnitTypeClass &Class_Of() const final { return *m_Class; }
    virtual coord_t Sort_Y() const final;
    virtual BOOL Limbo() final;
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH) final;
    virtual const int16_t *Overlap_List(BOOL a1 = false) const final;
    virtual void Draw_It(int x_pos, int y_pos, WindowNumberType window) const final;
    virtual void Active_Click_With(ActionType action, ObjectClass *object) final;
    virtual void Active_Click_With(ActionType action, cell_t cellnum) final;
    virtual DamageResultType Take_Damage(
        int &damage, int distance, WarheadType warhead, TechnoClass *object = nullptr, BOOL force = false) final;
    virtual void Scatter(coord_t coord = 0, BOOL a2 = false, BOOL a3 = false) final;
    virtual void Per_Cell_Process(PCPType pcp) final;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) final;

    // MissionClass
    virtual int Mission_Guard() final;
    virtual int Mission_Guard_Area() final;
    virtual int Mission_Harvest() final;
    virtual int Mission_Hunt() final;
    virtual int Mission_Move() final;
    virtual int Mission_Unload() final;
    virtual int Mission_Repair() final;

    // TechnoClass
    virtual DirType Turret_Facing() const final;
    virtual DirType Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const final;
    virtual DirType Fire_Direction() const final;
    virtual InfantryType Crew_Type() const final;
    virtual fixed_t Ore_Load() const final;
    virtual int Pip_Count() const final;
    virtual FireErrorType Can_Fire(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const final;
    virtual target_t Greatest_Threat(ThreatType threat) final;
    virtual BulletClass *Fire_At(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) final;
    virtual void Assign_Destination(target_t dest) final;
    virtual void Enter_Idle_Mode(BOOL a1 = false) final;

    // FootClass
    virtual BOOL Start_Driver(coord_t &dest) final;
    virtual BOOL Offload_Ore_Bail() final;
    virtual void Approach_Target() final;

    // DriveClass
    virtual void Overrun_Cell(cell_t cellnum, BOOL a2) final;
    virtual BOOL Ok_To_Move(DirType dir) final;

    BOOL Edge_Of_World_AI();
    BOOL Flag_Attach(HousesType house);
    BOOL Flag_Remove();
    void APC_Close_Door();
    void APC_Open_Door();
    int Credit_Load();
    BOOL Ore_Check(short &cellnum, int a2, int a3);
    BOOL Goto_Ore(int scan_radius);

    UnitType What_Type() const { return m_Class->What_Type(); }

    HousesType Flag_Owner() const { return m_FlagOwner; }

#ifdef GAME_DLL
    friend void Setup_Hooks();

public:
    void Hook_Active_Click_With_Obj(ActionType action, ObjectClass *object)
    {
        UnitClass::Active_Click_With(action, object);
    }
    void Hook_Active_Click_With_Cell(ActionType action, cell_t cellnum)
    {
        UnitClass::Active_Click_With(action, cellnum);
    }
    InfantryType Hook_Crew_Type()
    {
        return UnitClass::Crew_Type();
    }
    FireErrorType Hook_Can_Fire(target_t target, WeaponSlotType weapon)
    {
        return UnitClass::Can_Fire(target, weapon);
    }
    DirType Hook_Fire_Direction()
    {
        return UnitClass::Fire_Direction();
    }
#endif

private:
    GamePtr<UnitTypeClass> m_Class;
    HousesType m_FlagOwner;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsDumping : 1; // 1
    int m_Gold : 5; // 2
    int m_Gems : 5;
    BOOL m_ToScatter : 1;
#else
    bool m_IsDumping;
    int m_Gold;
    int m_Gems;
    bool m_ToScatter;
#endif
    unsigned int m_BailCount;
    unsigned int m_GapGenCellTracker;
    cell_t m_GapGenCenterCell;
    TCountDownTimerClass<FrameTimerClass> m_V2RearmDelayTimer;
    FacingClass m_TurretFacing;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<UnitClass> &g_Units;
#else
extern TFixedIHeapClass<UnitClass> g_Units;
#endif

#endif // UNIT_H
