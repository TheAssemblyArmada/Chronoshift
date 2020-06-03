/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles things with stages.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef STAGE_H
#define STAGE_H

#include "always.h"
#include "ttimer.h"

class StageClass
{
public:
    StageClass() : m_Stage(0), m_Timer(0), m_Rate(0) {}
    StageClass(const NoInitClass &noinit) : m_Timer(noinit) {}
    StageClass &operator=(StageClass &that);

    int Get_Stage() const { return m_Stage; }
    void Set_Stage(int stage) { m_Stage = stage; }
    int Get_Rate() const { return m_Rate; }
    void Set_Rate(int rate);
    BOOL To_Change();
    BOOL Stage_AI();
    void Reset();

private:
    int m_Stage; // Current stage we are at.
    TCountDownTimerClass<FrameTimerClass> m_Timer; // Countdown timer, 0 indicates time to go to next stage.
    int m_Rate; // Rate at which the stage transition occurs.
};

/**
 * @brief Sets the delay between stage transitions in game frames.
 */
inline void StageClass::Set_Rate(int rate)
{
    m_Timer = rate;
    m_Rate = rate;
}

/**
 * @brief Is a stage change going to occur.
 */
inline BOOL StageClass::To_Change()
{
    return m_Timer.Expired() && m_Rate > 0;
}

/**
 * @brief Handles a stage change should it be needed.
 * Returns if a stage change has occured.
 */
inline BOOL StageClass::Stage_AI()
{
    if (To_Change()) {
        ++m_Stage;
        m_Timer = m_Rate;

        return true;
    }

    return false;
}

/**
 * @brief Reset all members.
 */
inline void StageClass::Reset()
{
    m_Stage = 0;
    m_Timer = 0;
    m_Rate = 0;
}

inline StageClass &StageClass::operator=(StageClass &that)
{
    if (this != &that) {
        m_Stage = that.m_Stage;
        m_Timer = that.m_Timer;
        m_Rate = that.m_Rate;
    }

    return *this;
}

#endif // STAGE_H
