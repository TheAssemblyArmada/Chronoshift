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

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "always.h"
#include "aircrafttype.h"
#include "fly.h"
#include "foot.h"


class AircraftClass : public FootClass
{
public:
    AircraftClass(AircraftType type, HousesType house);
    AircraftClass(const AircraftClass &that);
    AircraftClass(const NoInitClass &noinit);
    virtual ~AircraftClass();

    // AbstractClass
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const final;
    virtual void AI() final;

    // ObjectClass
    virtual ActionType What_Action(ObjectClass *object) const final;
    virtual ActionType What_Action(cell_t cellnum) const final;
    virtual LayerType In_Which_Layer() const final;
    virtual const AircraftTypeClass &Class_Of() const final { return *m_Class; }
    virtual coord_t Sort_Y() const final;
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH) final;
    virtual int Exit_Object(TechnoClass *object) final;
    virtual const int16_t *Overlap_List(BOOL a1 = false) const final;
    virtual void Draw_It(int x, int y, WindowNumberType window) const final;
    virtual void Look(BOOL a1 = false) final;
    virtual void Active_Click_With(ActionType action, ObjectClass *object) final;
    virtual void Active_Click_With(ActionType action, cell_t cellnum) final;
    virtual DamageResultType Take_Damage(
        int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false) final;
    virtual void Scatter(coord_t coord = 0, BOOL a2 = false, BOOL a3 = false) final;
    virtual void Per_Cell_Process(PCPType pcp) final;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) final;

    // MissionClass
    virtual int Mission_Attack() final;
    virtual int Mission_Guard() final;
    virtual int Mission_Guard_Area() final;
    virtual int Mission_Hunt() final;
    virtual int Mission_Move() final;
    virtual int Mission_Retreat() final;
    virtual int Mission_Unload() final;
    virtual int Mission_Enter() final;

    // TechnoClass
    virtual DirType Turret_Facing() const final;
    virtual DirType Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const final;
    virtual DirType Fire_Direction() const final;
    virtual int Pip_Count() const final;
    virtual void Response_Select() final;
    virtual void Response_Move() final;
    virtual void Response_Attack() final;
    virtual void Player_Assign_Mission(MissionType mission, target_t target = 0, target_t dest = 0) final;
    virtual FireErrorType Can_Fire(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const final;
    virtual BulletClass *Fire_At(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) final;
    virtual void Assign_Destination(target_t dest) final;
    virtual void Enter_Idle_Mode(BOOL a1 = false) final;

    // FootClass
    virtual void Set_Speed(int speed) final;

    AircraftType What_Type() const { return m_Class->What_Type(); }

private:
    int Shape_Number() const;
    void Draw_Rotors(int x, int y, WindowNumberType window) const;
    void Rotation_AI();
    void Movement_AI();
    BOOL Landing_Takeoff_AI();
    BOOL Edge_Of_World_AI();
    BOOL Process_Take_Off();
    BOOL Is_LZ_Clear(target_t landing_zone);
    DirType Pose_Dir() const;
    int Paradrop_Cargo();
    BOOL Cell_Seems_Ok(cell_t cell, BOOL a3);

#ifdef GAME_DLL
    friend void Setup_Hooks();

public:
    void Hook_Draw_It(int x, int y, WindowNumberType window)
    {
        AircraftClass::Draw_It(x, y, window);
    }
    MoveType Hook_Can_Enter_Cell(cell_t cellnum, FacingType facing)
    {
        return AircraftClass::Can_Enter_Cell(cellnum, facing);
    }
    DirType Hook_Desired_Load_Dir(ObjectClass *object, cell_t &cellnum)
    {
        return AircraftClass::Desired_Load_Dir(object, cellnum);
    }
    int Hook_Pip_Count()
    {
        return AircraftClass::Pip_Count();
    }

    void Hook_Active_Click_With(ActionType action, ObjectClass *object)
    {
        AircraftClass::Active_Click_With(action, object);
    }

    void Hook_Player_Assign_Mission(MissionType mission, target_t target, target_t dest)
    {
        AircraftClass::Player_Assign_Mission(mission, target, dest);
    }


#endif

private:
    FlyClass m_FlyControl;
    GamePtr<AircraftTypeClass> m_Class;
    FacingClass m_BDir;
#ifdef GAME_DLL
    BOOL m_field_14C;
#else
    bool m_field_14C; // could be HasCargo?
#endif
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsLanding : 1; // 1
    BOOL m_IsTakingOff : 1; // 2
    BOOL m_Bit4 : 1; // 4
    BOOL m_Bit8 : 1; // 8
    BOOL m_Bit16 : 1; // 16
#else
    bool m_IsLanding;
    bool m_IsTakingOff;
    bool m_Bit4;
    bool m_Bit8;
    bool m_Bit16;
#endif
    uint8_t m_field_154; // Looks like jitter or jitter level used in Draw_It in TD.
    TCountDownTimerClass<FrameTimerClass> m_LookDelayTimer;
    uint8_t m_field_15E; // Attk in mono info.
};

#ifdef GAME_DLL
extern TFixedIHeapClass<AircraftClass> &g_Aircraft;
#else
extern TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

#endif // AIRCRAFT_H
