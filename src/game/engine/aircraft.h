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
    virtual void Scatter(coord_t coord = 0, int a2 = 0, BOOL a3 = false) final;
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
    virtual FireErrorType Can_Fire(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const final;
    virtual BulletClass *Fire_At(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) final;
    virtual void Assign_Destination(target_t dest) final;
    virtual void Enter_Idle_Mode(BOOL a1 = false) final;

    // FootClass
    virtual void Set_Speed(int speed) final;

    AircraftType What_Type() const { return m_Class->What_Type(); }

private:
    FlyClass FlyControl;
    GamePtr<AircraftTypeClass> m_Class;
    FacingClass m_BDir;
    void *field_14C;
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
    uint8_t field_154;
    BasicTimerClass<FrameTimerClass> field_155;
    uint8_t field_15E;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<AircraftClass> &g_Aircraft;
#else
extern TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

#endif // AIRCRAFT_H
