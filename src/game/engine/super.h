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

#ifndef SUPER_H
#define SUPER_H

#include "always.h"
#include "ttimer.h"
#include "vox.h"

class NoInitClass;

class SuperClass
{
public:
    SuperClass(int recharge_time = 0, BOOL is_powered = false, VoxType charging = VOX_NONE, VoxType recharge = VOX_NONE, VoxType impatient = VOX_NONE, VoxType suspend = VOX_NONE);
    ~SuperClass() {}
    SuperClass(const SuperClass &that);
    SuperClass(const NoInitClass &noinit) {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    BOOL Suspend(BOOL suspend);
    BOOL Enable(BOOL one_time = false, BOOL feedback = false, BOOL suspend = false);
    BOOL Remove();
    BOOL Recharge(BOOL feedback = false);
    BOOL Discharged(BOOL feedback = false);
    BOOL AI(BOOL feedback = false);
    int Anim_Stage() const;
    void Impatient_Click() const;
    void Forced_Charge(BOOL feedback);

    bool Is_Powered() const { return m_IsPowered; }
    bool Is_Enabled() const { return m_IsEnabled; }
    bool Is_One_Time() const { return m_OneTime; }
    bool Is_Fully_Charged() const { return m_FullyCharged; }
    bool Ready_To_Fire() const { return m_IsEnabled && m_FullyCharged; }

private:

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_IsPowered : 1; // 1
            bool m_IsEnabled : 1; // 2
            bool m_OneTime : 1; // 4
            bool m_FullyCharged : 1; // 8
        };
        int m_Bitfield;
    };
#else
    bool m_IsPowered; // Does this super weapon become inoperative in a low power situation?
    bool m_IsEnabled; // 
    bool m_OneTime; // Is this a 'one shot' super weapon [removed after it has been fired]?
    bool m_FullyCharged; // Is this super weapon fully charged and ready to fire?
#endif
    TCountDownTimerClass<FrameTimerClass> m_ChargeTime;
    int m_ClockAnimStage;
    VoxType m_RechargeVoice; // Voice to use when weapon is fully recharged and ready.
    VoxType m_ChargingVoice; // Voice to use when weapon begins charging.
    VoxType m_ImpatientVoice; // Voice to use when user clicks on weapon that isn't finished charging.
    VoxType m_SuspendVoice; // Voice to use when special weapon charging is suspended.
    int m_RechargeTime; // Time in minutes to recharge this special.

#ifndef CHRONOSHIFT_STANDALONE
public:
    static void Hook_Me();

private:
    int Hook_Anim_Stage() { return SuperClass::Anim_Stage(); }
    void Hook_Impatient_Click() { return SuperClass::Impatient_Click(); }
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void SuperClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x00552FE8, *SuperClass::Suspend);
    Hook_Function(0x00553034, *SuperClass::Enable);
    Hook_Function(0x0055308C, *SuperClass::Remove);
    Hook_Function(0x005530B4, *SuperClass::Recharge);
    Hook_Function(0x00553110, *SuperClass::Discharged);
    Hook_Function(0x0055315C, *SuperClass::AI);
    Hook_Function(0x00553224, *SuperClass::Hook_Anim_Stage);
    Hook_Function(0x005532BC, *SuperClass::Hook_Impatient_Click);
    Hook_Function(0x005532E8, *SuperClass::Forced_Charge);
#endif
}
#endif

#endif // SUPER_H
