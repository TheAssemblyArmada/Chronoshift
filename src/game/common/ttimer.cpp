/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Templated timers intended to tick on either the system timer or a frame.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "ttimer.h"
#include "globals.h"
#include "ostimer.h"

#ifdef GAME_DLL
TTimerClass<SystemTimerClass> &TickCountTimer = *reinterpret_cast<TTimerClass<SystemTimerClass> *>(0x00680870);
TCountDownTimerClass<SystemTimerClass> &FrameTimer = *reinterpret_cast<TCountDownTimerClass<SystemTimerClass> *>(0x006807F8);
#else
TTimerClass<SystemTimerClass> TickCountTimer;
TCountDownTimerClass<SystemTimerClass> FrameTimer;
#endif

uint32_t SystemTimerClass::operator()() const
{
    if (PlatformTimer != nullptr) {
        return PlatformTimer->Get_System_Tick_Count();
    }

    return 0;
}

uint32_t FrameTimerClass::operator()() const
{
    return g_GameFrame;
}
