/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles things with stages.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
#include "noinit.h"
#include "ttimer.h"

class StageClass
{
public:
    StageClass() : Stage(0), Timer(0), Delay(0) {}
    StageClass(const NoInitClass &noinit) : Timer(noinit) {}
    StageClass &operator=(StageClass &that);

    int Get_Stage() const { return Stage; }
    void Set_Stage(int stage) { Stage = stage; }
    void Set_Delay(int delay);
    BOOL Stage_Changed();

private:
    int Stage; // Current stage we are at.
    TCountDownTimerClass<FrameTimerClass> Timer; // Countdown timer, 0 indicates time to go to next stage.
    int Delay; // Delay in frames to the next stage transition.
};

/**
 * @brief Sets the delay between stage transitions in game frames.
 */
inline void StageClass::Set_Delay(int delay)
{
    Timer = delay;
    Delay = delay;
}

/**
 * @brief Checks if a stage change has occured.
 */
inline BOOL StageClass::Stage_Changed()
{
    if (Timer.Time() <= 0 && Delay > 0) {
        ++Stage;
        Timer = Delay;

        return true;
    }

    return false;
}

inline StageClass &StageClass::operator=(StageClass &that)
{
    if (this != &that) {
        Stage = that.Stage;
        Timer = that.Timer;
        Delay = that.Delay;
    }

    return *this;
}

#endif // STAGE_H
