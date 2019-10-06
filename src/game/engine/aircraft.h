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
    AircraftClass(const NoInitClass& noinit) : FootClass(noinit) {}
    virtual ~AircraftClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    // AbstractClass
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const override;
    virtual void AI() override;

    // ObjectClass
    virtual ActionType What_Action(ObjectClass *object) const override;
    virtual ActionType What_Action(cell_t cellnum) const override;
    virtual LayerType In_Which_Layer() const override;
    virtual const AircraftTypeClass &Class_Of() const override { return *m_Type; }
    virtual coord_t Sort_Y() const override;
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH) override;
    virtual int Exit_Object(TechnoClass *object);
    virtual const int16_t *Overlap_List(BOOL a1 = false) const override;
    virtual void Draw_It(int x, int y, WindowNumberType window) const override;
    virtual void Look(BOOL a1 = false) override;
    virtual void Active_Click_With(ActionType action, ObjectClass *object) override;
    virtual void Active_Click_With(ActionType action, cell_t cellnum) override;
    virtual DamageResultType Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false) override;
    virtual void Scatter(coord_t coord = 0, int a2 = 0, BOOL a3 = false) override;
    virtual void Per_Cell_Process(PCPType pcp);
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) override;

    // MissionClass
    virtual int Mission_Attack() override;
    virtual int Mission_Guard() override;
    virtual int Mission_Guard_Area() override;
    virtual int Mission_Hunt() override;
    virtual int Mission_Move() override;
    virtual int Mission_Retreat() override;
    virtual int Mission_Unload() override;
    virtual int Mission_Enter() override;

    // TechnoClass
    virtual DirType Turret_Facing() const override;
    virtual DirType Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const override;
    virtual DirType Fire_Direction() const override;
    virtual int Pip_Count() const override;
    virtual void Response_Select() override;
    virtual void Response_Move() override;
    virtual void Response_Attack() override;
    virtual void Player_Assign_Mission(MissionType mission, target_t target = 0, target_t dest = 0) override;
    virtual FireErrorType Can_Fire(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual BulletClass *Fire_At(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) override;
    virtual void Assign_Destination(target_t dest) override;
    virtual void Enter_Idle_Mode(BOOL a1 = false) override;
    virtual void Set_Speed(int speed) override;

    DirType Pose_Dir() const;

    AircraftType What_Type() const { return m_Type->What_Type(); }

    static void Init();
    static void Read_INI(GameINIClass &ini);
    static void Write_INI(GameINIClass &ini);

private:
    int Shape_Number() const;
    void Draw_Rotors(int x, int y, WindowNumberType window) const;
    int Paradrop_Cargo();
    target_t New_LZ(target_t target) const;
    BOOL Is_LZ_Clear(target_t target) const;
    int Process_Fly_To(int a1, target_t a2);
    BOOL Process_Take_Off();
    BOOL Process_Landing();
    target_t Good_Fire_Location(target_t target) const;
    BOOL Cell_Seems_Ok(cell_t cell, int a2) const;
    target_t Good_LZ() const;
    BOOL Landing_Takeoff_AI();
    BOOL Edge_Of_World_AI();
    void Movement_AI();
    void Rotation_AI();

    static BOOL Counts_As_Civ_Evac(ObjectClass *object);

private:
    FlyClass m_Flyer;
    GamePtr<AircraftTypeClass> m_Type;
    FacingClass m_field_14A;
    void *m_field_14C;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Bit1 : 1; // 1
    BOOL m_Bit2 : 1; // 2
    BOOL m_Bit4 : 1; // 4
    BOOL m_Bit8 : 1; // 8
    BOOL m_Bit16 : 1; // 16
#else
    bool m_Bit1;
    bool m_Bit2;
    bool m_Bit4;
    bool m_Bit8;
    bool m_Bit16;
#endif
    uint8_t m_field_154;
    BasicTimerClass<FrameTimerClass> m_field_155;
    uint8_t m_field_15E;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<AircraftClass> &g_Aircraft;
#else
extern TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

#endif // AIRCRAFT_H
