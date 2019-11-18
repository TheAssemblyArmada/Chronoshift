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

#ifndef BUILDING_H
#define BUILDING_H

#include "always.h"
#include "buildingtype.h"
#include "factory.h"
#include "techno.h"

class BuildingClass : public TechnoClass
{
public:
    BuildingClass(BuildingType type, HousesType house);
    BuildingClass(const BuildingClass &that);
    BuildingClass(const NoInitClass &noinit);
    virtual ~BuildingClass();

    // ObjectClass
    virtual void *Get_Image_Data() const final;
    virtual const BuildingTypeClass &Class_Of() const final;
    virtual BOOL Can_Player_Move() const final;
    virtual void Detach(target_t target, int a2) final;
    virtual void Fire_Out() final {}
    virtual int Value() const final;

    // TechnoClass
    virtual void Death_Announcement(TechnoClass *killer) const final;

    BuildingType What_Type() const { return m_Class->What_Type(); }


#ifdef GAME_DLL
    friend void Setup_Hooks();

public:
    void Hook_Death_Announcement(TechnoClass *killer) { BuildingClass::Death_Announcement(killer); }
    int Hook_Value() { return BuildingClass::Value(); }
    void *Hook_Get_Image_Data() { return BuildingClass::Get_Image_Data(); }
#endif


private:
    GamePtr<BuildingTypeClass> m_Class;
    GamePtr<FactoryClass> m_Factory;
    HousesType m_field_D5;
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
    TCountDownTimerClass<FrameTimerClass> m_C4FuseTimer;
    BStateType m_CurrentState;
    BStateType m_NextState; // name is subject to change after researching
    HousesType m_LastAttackedBy; // house that last attacked this building, this is kept track for scoring purposes
    target_t m_SabotagedBy; // what sabotaged this building
    int m_field_EA;
    target_t m_AttachedAnim; // currently attached animation, seems to be only used for SPUTDOOR
    TCountDownTimerClass<FrameTimerClass> m_PlacementDelayTimer;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<BuildingClass> &g_Buildings;
#else
extern TFixedIHeapClass<BuildingClass> g_Buildings;
#endif

#endif // BUILDING_H
