/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class to wrap the Operating System's timer API to provide constant ticks.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef OSTIMER_H
#define OSTIMER_H

#include "always.h"
#include "timer.h"
#include <signal.h>
#include <time.h>

#ifdef PLATFORM_WINDOWS
#ifdef __WATCOMC__
#include <windows.h>
#else
#include <synchapi.h>
#endif
#include <mmsystem.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_SELECT
#include <sys/select.h>
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef HAVE_MACOS_GDC
#include <dispatch/dispatch.h>
#endif

class PlatformTimerClass
{
    friend void Setup_Hooks();
public:
    PlatformTimerClass(unsigned freq, BOOL partial = false);
    ~PlatformTimerClass();

    void Reset_System_Tick_Count() { m_sysTicks = 0; }
    void Reset_User_Tick_Count() { m_userTicks = 0; }
    unsigned Get_System_Tick_Count() { return m_sysTicks; }
    unsigned Get_User_Tick_Count() { return m_userTicks; }

    // Calls OS sleep function in micro seconds
    static void Sleep(int interval)
    {
#if defined PLATFORM_WINDOWS
        ::Sleep(interval);
#elif defined HAVE_NANOSLEEP
        struct timespec ts;
        ts.tv_sec = interval / 1000;
        ts.tv_nsec = (interval % 1000) * 1000000;
        nanosleep(&ts, nullptr);
#elif defined HAVE_USLEEP
        usleep(1000 * interval);
#elif defined HAVE_SYS_SELECT_H
        struct timeval tv;
        tv.tv_sec = interval / 1000;
        tv.tv_usec = (interval % 1000) * 1000;
        select(0, nullptr, nullptr, nullptr, &tv);
#else
#error Add sleep function in ostimer.h
#endif
    }

    static unsigned Get_Time()
    {
#ifdef PLATFORM_WINDOWS
        return timeGetTime();
#else
        struct timeval now;
        gettimeofday(&now, nullptr);
        return now.tv_usec / 1000;
#endif
    }

private:
#ifdef PLATFORM_WINDOWS
    static void __stdcall Timer_Callback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
#elif defined HAVE_MACOS_GCD
    static void Timer_Callback(void *arg);
#elif defined HAVE_POSIX_TIMERS
    static void Timer_Callback(union sigval arg);
#else
#error Implement a threaded timer callback in ostimer.h/cpp
#endif
    void Update_Tick_Count();
    void Update_System_Tick_Count() { ++m_sysTicks; }
    void Update_User_Tick_Count() { ++m_userTicks; }

private:
#if defined PLATFORM_WINDOWS
    MMRESULT m_timerHandle;
#elif defined HAVE_MACOS_GCD
    dispatch_source_t m_timerHandle;
    dispatch_queue_t TimerQueue;
#elif defined HAVE_POSIX_TIMERS
    timer_t m_timerHandle;
#endif

    unsigned m_frequency;
    unsigned m_trueRate;
    unsigned m_sysTicks;
    unsigned m_userTicks;
    unsigned m_userRate;

public:
#ifdef GAME_DLL
    static void *&s_threadHandle;
    static BOOL &s_inCallback;
    static BOOL &s_timerSystemOn;
#else
    static void *s_threadHandle;
    static BOOL s_inCallback;
    static BOOL s_timerSystemOn;
#endif
};

#ifdef GAME_DLL
extern PlatformTimerClass *&g_PlatformTimer;
#else
extern PlatformTimerClass *g_PlatformTimer;
#endif

#endif // OSTIMER_H
