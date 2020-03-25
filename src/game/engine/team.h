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

#ifndef TEAM_H
#define TEAM_H

#include "always.h"
#include "abstract.h"
#include "gameptr.h"
#include "gametypes.h"
#include "house.h"
#include "trigger.h"
#include "ttimer.h"

class FootClass;
class TeamTypeClass;

class TeamClass : public AbstractClass
{
public:
    TeamClass(TeamTypeClass *teamtype, HouseClass *house);
    TeamClass(const NoInitClass &noinit) {}
    virtual ~TeamClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual void AI() override;

    BOOL Remove(FootClass *object, int a2 = -1);

    void Code_Pointers();
    void Decode_Pointers();

    void Force_Active() { m_ForcedActive = true; }
    BOOL Is_Roundabout() const { return m_Class->Roundabout(); }
    int Field35() const { return m_field_35; }
    BOOL Get_Bit2_4() const { return m_Bit2_4; }
    void Set_Bit2_4(BOOL state) { m_Bit2_4 = state; }
    const GamePtr<TeamTypeClass> &Get_Class() const { return m_Class; }
    FootClass *Get_field_54() { return m_field_54; }

private:
    GamePtr<TeamTypeClass> m_Class;
    GamePtr<HouseClass> m_Owner;

#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_ForcedActive : 1; // 1
    BOOL m_HasBeen : 1; // 2
    BOOL m_FullStrength : 1; // 4
    BOOL m_Understrength : 1; // 8
    BOOL m_Reforming : 1; // 16
    BOOL m_Bit1_32 : 1; // 32
    BOOL m_Bit1_64 : 1; // 64
    BOOL m_Bit1_128 : 1; // 128

    BOOL m_Moving : 1; // 1
    BOOL m_Bit2_2 : 1; // 2
    BOOL m_Bit2_4 : 1; // 4
    BOOL m_Bit2_8 : 1; // 8
    BOOL m_Bit2_16 : 1; // 16
    BOOL m_Bit2_32 : 1; // 32
    BOOL m_Bit2_64 : 1; // 64
    BOOL m_Bit2_128 : 1; // 128
#else
    bool m_ForcedActive;
    bool m_HasBeen;
    bool m_FullStrength;
    bool m_Understrength;
    bool m_Reforming;
    bool m_Bit1_32;
    bool m_Bit1_64;
    bool m_Bit1_128;
    bool m_Moving;
    bool m_Bit2_2;
    bool m_Bit2_4;
    bool m_Bit2_8;
    bool m_Bit2_16;
    bool m_Bit2_32;
    bool m_Bit2_64;
    bool m_Bit2_128;
#endif

    target_t m_Center;
    target_t m_field_25;
    target_t m_field_29;
    target_t m_Target;
    int m_Members;
    int m_field_35;
    FormationType m_Formation;
    TCountDownTimerClass<FrameTimerClass> m_field_3A;
    GamePtr<TriggerClass> m_Trigger;
    int m_Mission;
    TCountDownTimerClass<FrameTimerClass> m_field_4B;
    FootClass *m_field_54;
    uint8_t m_Quantity[5];
};

#ifdef GAME_DLL
extern TFixedIHeapClass<TeamClass> &g_Teams;
#else
extern TFixedIHeapClass<TeamClass> g_Teams;
#endif

#endif // TEAM_H
