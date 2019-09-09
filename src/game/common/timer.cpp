/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief A "tick" timer.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "timer.h"
#include "ostimer.h"

#ifdef GAME_DLL
TimerClass &TickCount = Make_Global<TimerClass>(0x006ABF6C);
CountDownTimerClass &CountDown = Make_Global<CountDownTimerClass>(0x006ABF78);
#else
TimerClass TickCount;
CountDownTimerClass CountDown;
#endif

TimerClass::TimerClass(BaseTimerEnum base_timer, BOOL start) : m_started(0), m_accumulated(0), m_tickType(base_timer)
{
    if (start && PlatformTimer->s_timerSystemOn) {
        Start();
    }
}

TimerClass::TimerClass(TimerClass const &that) :
    m_started(that.m_started),
    m_accumulated(that.m_accumulated),
    m_tickType(that.m_tickType)
{
}

TimerClass::~TimerClass()
{
    Reset();
}

int TimerClass::Get_Ticks()
{
    if (PlatformTimer != nullptr) {
        switch (m_tickType) {
            case BT_USER:
                return PlatformTimer->Get_User_Tick_Count();

            case BT_SYSTEM:
            default:
                return PlatformTimer->Get_System_Tick_Count();
        }
    }

    return 0;
}

TimerClass &TimerClass::operator=(TimerClass &that)
{
    if (this != &that) {
        m_tickType = that.m_tickType;
        m_started = that.m_started;
        m_accumulated = that.m_accumulated;
    }

    return *this;
}

int TimerClass::Stop()
{
    int time = Time();
    m_started = 0;

    return time;
}

int TimerClass::Start()
{
    if (!m_started) {
        m_started = Get_Ticks() + 1;
    }

    return Time();
}

int TimerClass::Set(int ticks, BOOL start)
{
    m_started = 0;
    m_accumulated = ticks;

    if (start) {
        return Start();
    }

    return Time();
}

int TimerClass::Reset(BOOL start)
{
    return Set(0, start);
}

int TimerClass::Time()
{
    if (m_started) {
        int ticks = Get_Ticks();
        m_started += ticks - (m_started - 1);
        m_accumulated = ++ticks;
    }

    return m_accumulated;
}

CountDownTimerClass::CountDownTimerClass(BaseTimerEnum timer, BOOL on) :
    TimerClass(timer, on)
{
    m_delayTime = 0;

    if (on) {
        Start();
    }
}


CountDownTimerClass::CountDownTimerClass(BaseTimerEnum timer, int set, BOOL on) :
    TimerClass(timer, on)
{
    //
    // 
    //
    Set(set, on);
}

CountDownTimerClass &CountDownTimerClass::operator=(CountDownTimerClass &that)
{
    if (this != &that) {
        TimerClass::operator=(that);
        m_delayTime = that.m_delayTime;
    }

    return *this;
}

CountDownTimerClass &CountDownTimerClass::operator=(int value)
{
    Set(value, true);

    return *this;
}

int CountDownTimerClass::Set(int value, BOOL start)
{
    m_delayTime = value;

    TimerClass::Reset(start);

    return Time();
}

int CountDownTimerClass::Start()
{
    TimerClass::Start();

    return Time();
}

int CountDownTimerClass::Stop()
{

    TimerClass::Stop();

    return Time();
}

void CountDownTimerClass::Reset()
{
    m_delayTime = 0;
}

int CountDownTimerClass::Time()
{
    int ticks = m_delayTime - TimerClass::Time();

    if (ticks < 0) {
        return 0;
    }

    return ticks;
}