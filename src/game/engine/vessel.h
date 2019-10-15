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

#ifndef VESSEL_H
#define VESSEL_H

#include "always.h"
#include "drive.h"
#include "vesseltype.h"

class VesselClass : public DriveClass
{
    public:
    VesselClass(VesselType type, int id, HousesType house);
    VesselClass(const VesselClass &that);
    VesselClass(const NoInitClass &noinit);
    virtual ~VesselClass();

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
    virtual const VesselTypeClass &Class_Of() const override { return *m_Class; }
    virtual coord_t Fire_Coord(WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual const int16_t *Overlap_List(BOOL a1 = false) const override;
    virtual void Draw_It(int x, int y, WindowNumberType window) const override;
    virtual void Active_Click_With(ActionType action, ObjectClass *object) override;
    virtual void Active_Click_With(ActionType action, cell_t cellnum) override;
    virtual DamageResultType Take_Damage(
        int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false) override;
    virtual void Per_Cell_Process(PCPType pcp) override;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) override;

    // MissionClass
    virtual int Mission_Retreat() override;
    virtual int Mission_Unload() override;

    // TechnoClass
    virtual DirType Turret_Facing() const override;
    virtual DirType Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const override;
    virtual BOOL Is_Allowed_To_Recloak() const override;
    virtual FireErrorType Can_Fire(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual target_t Greatest_Threat(ThreatType threat) override;
    virtual BulletClass *Fire_At(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) override;
    virtual void Assign_Destination(target_t dest) override;
    virtual void Enter_Idle_Mode(BOOL a1 = false) override;

    // FootClass
    virtual BOOL Start_Driver(coord_t &dest) override;

    // VesselClass
    virtual MZoneType Zone_Check_Type() const { return MZONE_AMPHIBIOUS_DESTROYER; }

    int Shape_Number() const;

    void Rotation_AI();
    void Combat_AI();
    void Repair_AI();
    BOOL Edge_Of_World_AI();
    void Transport_Open_Door();
    void Transport_Close_Door();

    static void Read_INI(GameINIClass &ini);
    static void Write_INI(GameINIClass &ini);

    VesselType What_Type() const { return m_Class->What_Type(); }

    static void Init();

private:
    GamePtr<VesselTypeClass> m_Class;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_ToSelfRepair : 1; // 1
    BOOL m_Repairing : 1; // 2
#else
    bool m_ToSelfRepair; // 
    bool m_Repairing; // Is this vessel currently receiving repairs?
#endif
    TCountDownTimerClass<FrameTimerClass> m_TransportDoorTimer;
    TCountDownTimerClass<FrameTimerClass> m_SubmergeTimer;
    FacingClass m_SecondaryTurretFacing;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<VesselClass> &g_Vessels;
#else
extern TFixedIHeapClass<VesselClass> g_Vessels;
#endif

#endif // VESSEL_H
