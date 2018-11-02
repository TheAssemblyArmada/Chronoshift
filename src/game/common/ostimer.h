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

#if defined(PLATFORM_WINDOWS)
#include <mmsystem.h>
#else
#include <unistd.h>
#if defined(PLATFORM_OSX)
#include <dispatch/dispatch.h>
#elif defined(PLATFORM_UNIX)
#include <sys/time.h>
#endif
#endif // PLATFORM_WINDOWS

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
#endif

class PlatformTimerClass
{
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
#if defined(PLATFORM_WINDOWS)
        ::Sleep(interval);
#elif defined(PLATFORM_UNIX) || defined(PLATFORM_OSX)
        usleep(1000 * interval);
#endif // PLATFORM_WINDOWS || PLATFORM_UNIX || PLATFORM_OSX
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
    
#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif
private:
#ifdef PLATFORM_WINDOWS
    static void __stdcall Timer_Callback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
#elif defined PLATFORM_OSX
    static void Timer_Callback(void *arg);
#else // !PLATFORM_WINDOWS || !PLATFORM_OSX
    static void Timer_Callback(union sigval arg);
#endif // PLATFORM_WINDOWS
    void Update_Tick_Count();
    void Update_System_Tick_Count() { ++m_sysTicks; }
    void Update_User_Tick_Count() { ++m_userTicks; }

private:
#if defined(PLATFORM_WINDOWS)
    MMRESULT m_timerHandle;
#elif defined(PLATFORM_OSX)
    dispatch_source_t m_timerHandle;
    dispatch_queue_t TimerQueue;
#elif defined(PLATFORM_UNIX)
    timer_t m_timerHandle;
#endif // PLATFORM_WINDOWS || PLATFORM_OSX || PLATFORM_UNIX

    unsigned m_frequency;
    unsigned m_trueRate;
    unsigned m_sysTicks;
    unsigned m_userTicks;
    unsigned m_userRate;

public:
#ifndef CHRONOSHIFT_STANDALONE
    static void *&s_threadHandle;
    static BOOL &s_inCallback;
    static BOOL &s_timerSystemOn;
#else
    static void *s_threadHandle;
    static BOOL s_inCallback;
    static BOOL s_timerSystemOn;
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
inline void PlatformTimerClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005BBEB0, &Timer_Callback);
#endif
}

extern PlatformTimerClass *&PlatformTimer;
#else
extern PlatformTimerClass *PlatformTimer;
#endif

#endif // OSTIMER_H
