/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief A "tick" timer and its count down equivalent.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TIMER_H
#define TIMER_H

#include "always.h"

#ifndef RAPP_STANDALONE
#include "hooker.h"
#endif

enum BaseTimerEnum
{
    BT_SYSTEM = 0,
    BT_USER = 1
};

class TimerClass
{
public:
    TimerClass(BaseTimerEnum timer = BT_SYSTEM, BOOL on = false);
    TimerClass(TimerClass const &that);
    ~TimerClass();

    TimerClass &operator=(TimerClass &that);
    TimerClass &operator=(int value);

    int Stop();
    int Start();

    int Set(int ticks, BOOL start = false);
    int Reset(BOOL start = false);

    int Time();

private:
    int Get_Ticks();

protected:
    int m_started;
    int m_accumulated;

private:
    BaseTimerEnum m_tickType;
};

class CountDownTimerClass : public TimerClass
{
public:
    CountDownTimerClass(BaseTimerEnum timer = BT_SYSTEM, BOOL on = false);
    CountDownTimerClass(BaseTimerEnum timer, int set, BOOL on = false);
    ~CountDownTimerClass() {}

    CountDownTimerClass &operator=(CountDownTimerClass &that);
    CountDownTimerClass &operator=(int value);

    operator int() { return Time(); }

    int Set(int value, BOOL start = true);

    int Start();
    int Stop();

    void Reset();

    int Time();

private:
    int m_delayTime;
};


#ifndef RAPP_STANDALONE
extern TimerClass &TickCount;
extern CountDownTimerClass &CountDown;
#else
extern TimerClass TickCount;
extern CountDownTimerClass CountDown;
#endif

#endif // TIMER_H
