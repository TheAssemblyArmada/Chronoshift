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
#include "super.h"
#include "fixed.h"
#include "audio.h"

#define MAX_CLOCK_STAGES 54

SuperClass::SuperClass(int recharge_time, BOOL is_powered, VoxType charging, VoxType recharge, VoxType impatient, VoxType suspend) : 
    m_ChargeTime(),
    m_ClockAnimStage(-1),
    m_IsPowered(is_powered),
    m_IsEnabled(false),
    m_OneTime(false),
    m_FullyCharged(false),
    m_ChargingVoice(charging),
    m_RechargeVoice(recharge),
    m_ImpatientVoice(impatient),
    m_SuspendVoice(suspend),
    m_RechargeTime(recharge_time)
{
}

SuperClass::SuperClass(const SuperClass &that) :
    m_ChargeTime(that.m_ChargeTime),
    m_ClockAnimStage(that.m_ClockAnimStage),
    m_IsPowered(that.m_IsPowered),
    m_IsEnabled(that.m_IsEnabled),
    m_OneTime(that.m_OneTime),
    m_FullyCharged(that.m_FullyCharged),
    m_ChargingVoice(that.m_ChargingVoice),
    m_RechargeVoice(that.m_RechargeVoice),
    m_ImpatientVoice(that.m_ImpatientVoice),
    m_SuspendVoice(that.m_SuspendVoice),
    m_RechargeTime(that.m_RechargeTime)
{
}

BOOL SuperClass::Suspend(BOOL suspend)
{
/*#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(SuperClass *, BOOL) = reinterpret_cast<BOOL (*)(SuperClass *, BOOL)>(0x00552FE8);
    return func(this, suspend);
#else*/
    if (!m_IsEnabled || m_OneTime || m_FullyCharged || suspend != m_ChargeTime.Started()) {
        return false;
    }
    if (m_ChargeTime.Started()) {
        m_ChargeTime.Stop();
    } else {
        m_ChargeTime.Start();
    }
    return true;
//#endif
}

BOOL SuperClass::Enable(BOOL one_time, BOOL feedback, BOOL suspend)
{
/*#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(SuperClass *, BOOL, BOOL, BOOL) = reinterpret_cast<BOOL (*)(SuperClass *, BOOL, BOOL, BOOL)>(0x00553034);
    return func(this, one_time, feedback, suspend);
#else*/
    if (m_IsEnabled) {
        return false;
    }

    m_IsEnabled = true;
    m_OneTime = one_time;

    bool recharging = Recharge(feedback && !suspend);

    if (suspend) {
        Suspend(true);
    }

    return recharging;
//#endif
}

BOOL SuperClass::Remove()
{
/*#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(SuperClass *) = reinterpret_cast<BOOL (*)(SuperClass *)>(0x0055308C);
    return func(this);
#else*/
    if (!m_IsEnabled || m_OneTime) {
        return false;
    }
    m_IsEnabled = false;
    m_FullyCharged = false;
    return true;
//#endif
}

BOOL SuperClass::Recharge(BOOL feedback)
{
/*#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(SuperClass *, BOOL) = reinterpret_cast<BOOL (*)(SuperClass *, BOOL)>(0x005530B4);
    return func(this, feedback);
#else*/
    if (m_IsEnabled && !m_FullyCharged) {
        m_ClockAnimStage = -1;
        m_ChargeTime.Start();
        m_ChargeTime.Set(m_RechargeTime);
        if (feedback) {
            if (m_ChargingVoice != VOX_NONE) {
                Speak(m_ChargingVoice);
            }
        }
        return true;
    }
    return false;
//#endif
}

BOOL SuperClass::Discharged(BOOL feedback)
{
/*#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(SuperClass *, BOOL) = reinterpret_cast<int (*)(SuperClass *, BOOL)>(0x00553110);
    return func(this, feedback);
#else*/
    if (m_ChargeTime.Stopped() || !m_IsEnabled || !m_FullyCharged) {
        return false;
    }
    m_FullyCharged = false;
    if (m_OneTime) {
        m_OneTime = false;
        return Remove();
    }
    Recharge(feedback);
    return false;
//#endif
}

BOOL SuperClass::AI(BOOL feedback)
{
/*#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(SuperClass *, BOOL) = reinterpret_cast<BOOL (*)(SuperClass *, BOOL)>(0x0055315C);
    return func(this, feedback);
#else*/
    if (!m_IsEnabled || m_FullyCharged) {
        return false;
    }
    if (m_ChargeTime.Stopped()) {
        if (m_ClockAnimStage != -1) {
            m_ClockAnimStage = -1;
            return true;
        }
        return false;
    }
    if (!m_ChargeTime.Expired()) {
        if (Anim_Stage() != m_ClockAnimStage) {
            m_ClockAnimStage = Anim_Stage();
            return true;
        }
        return false;
    }
    m_FullyCharged = true;
    if (feedback) {
        if (m_RechargeVoice != VOX_NONE) {
            Speak(m_RechargeVoice);
        }
    }
    return true;
//#endif
}

int SuperClass::Anim_Stage() const
{
/*#ifndef CHRONOSHIFT_STANDALONE
    int (*func)(const SuperClass *) = reinterpret_cast<int (*)(const SuperClass *)>(0x00553224);
    return func(this);
#else*/
    int anim_stage = 0;
    if (m_IsEnabled) {
        if (m_FullyCharged) {
            anim_stage = MAX_CLOCK_STAGES;
        } else {
            anim_stage = (MAX_CLOCK_STAGES * fixed(m_RechargeTime - m_ChargeTime.Time(), m_RechargeTime));
            if (anim_stage >= (MAX_CLOCK_STAGES - 1)) {
                anim_stage = (MAX_CLOCK_STAGES - 1);
            }
        }
    }
    return anim_stage;
//#endif
}

void SuperClass::Impatient_Click() const
{
/*#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const SuperClass *) = reinterpret_cast<void (*)(const SuperClass *)>(0x005532BC);
    func(this);
#else*/
    if (m_ChargeTime.Stopped()) {
        if (m_SuspendVoice != VOX_NONE) {
            Speak(m_SuspendVoice);
        }
    } else {
        if (m_ImpatientVoice != VOX_NONE) {
            Speak(m_ImpatientVoice);
        }
    }
//#endif
}

void SuperClass::Forced_Charge(BOOL feedback)
{
/*#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(SuperClass *, BOOL) = reinterpret_cast<void (*)(SuperClass *, BOOL)>(0x005532E8);
    func(this, feedback);
#else*/
    if (m_IsEnabled) {
        m_FullyCharged = true;
        m_ChargeTime.Start();
        m_ChargeTime.Reset();
        if (feedback) {
            if (m_RechargeVoice != VOX_NONE) {
                Speak(m_RechargeVoice);
            }
        }
    }
//#endif
}
