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
    StageClass() : m_Stage(0), m_Timer(0), m_Delay(0) {}
    StageClass(const NoInitClass &noinit) : m_Timer(noinit) {}
    StageClass &operator=(StageClass &that);

    int Get_Stage() const { return m_Stage; }
    void Set_Stage(int stage) { m_Stage = stage; }
    int Get_Delay() const { return m_Delay; }
    void Set_Delay(int delay);
    BOOL Stage_Changed();
    void Reset();

private:
    int m_Stage; // Current stage we are at.
    TCountDownTimerClass<FrameTimerClass> m_Timer; // Countdown timer, 0 indicates time to go to next stage.
    int m_Delay; // Delay in frames to the next stage transition.
};

/**
 * @brief Sets the delay between stage transitions in game frames.
 */
inline void StageClass::Set_Delay(int delay)
{
    m_Timer = delay;
    m_Delay = delay;
}

/**
 * @brief Checks if a stage change has occured.
 */
inline BOOL StageClass::Stage_Changed()
{
    if (m_Timer.Time() <= 0 && m_Delay > 0) {
        ++m_Stage;
        m_Timer = m_Delay;

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
    m_Delay = 0;
}

inline StageClass &StageClass::operator=(StageClass &that)
{
    if (this != &that) {
        m_Stage = that.m_Stage;
        m_Timer = that.m_Timer;
        m_Delay = that.m_Delay;
    }

    return *this;
}

#endif // STAGE_H
