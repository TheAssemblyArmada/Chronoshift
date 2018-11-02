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
#include "ostimer.h"
#include "gamedebug.h"
#include "timer.h"

#ifndef PLATFORM_WINDOWS
#include <signal.h>
#include <time.h>
#endif // PLATFORM_WINDOWS

#ifndef CHRONOSHIFT_STANDALONE
PlatformTimerClass *&PlatformTimer = Make_Global<PlatformTimerClass *>(0x00665EB0);
BOOL &PlatformTimerClass::s_timerSystemOn = Make_Global<BOOL>(0x006ABF68);
BOOL &PlatformTimerClass::s_inCallback = Make_Global<BOOL>(0x006ABF8C);
void *&PlatformTimerClass::s_threadHandle = Make_Global<void *>(0x006ABF88);
#else
PlatformTimerClass *PlatformTimer = nullptr;
BOOL PlatformTimerClass::s_timerSystemOn = false;
BOOL PlatformTimerClass::s_inCallback = false;
void *PlatformTimerClass::s_threadHandle = nullptr;
#endif

#ifdef PLATFORM_WINDOWS
void __stdcall PlatformTimerClass::Timer_Callback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    PlatformTimer->s_inCallback = true;

    if (PlatformTimerClass::s_threadHandle == nullptr) {
        DuplicateHandle(GetCurrentProcess(),
            GetCurrentThread(),
            GetCurrentProcess(),
            static_cast<LPHANDLE>(&PlatformTimerClass::s_threadHandle),
            0,
            true,
            DUPLICATE_SAME_ACCESS);
    }

    if (PlatformTimer) {
        PlatformTimer->Update_Tick_Count();
    }

    PlatformTimerClass::s_inCallback = false;
}
#elif defined PLATFORM_OSX
void PlatformTimerClass::Timer_Callback(void *arg)
{
    PlatformTimerClass::s_inCallback = true;

    if (PlatformTimer) {
        PlatformTimer->Update_Tick_Count();
    }

    PlatformTimerClass::s_inCallback = false;
}
#else // !PLATFORM_WINDOWS || !PLATFORM_OSX
void PlatformTimerClass::Timer_Callback(union sigval arg)
{
    PlatformTimerClass::s_inCallback = true;

    if (PlatformTimer) {
        PlatformTimer->Update_Tick_Count();
    }

    PlatformTimerClass::s_inCallback = false;
}
#endif // PLATFORM_WINDOWS

PlatformTimerClass::PlatformTimerClass(unsigned freq, BOOL partial) :
    m_timerHandle(0),
    m_frequency(0),
    m_sysTicks(0),
    m_userTicks(0)
{
#if defined(PLATFORM_WINDOWS)
    // Set the system timer accuracy to be at least as accurate as our callbacks
    // need on WINAPI.
    timeBeginPeriod(1000 / freq);
    m_frequency = freq; // This is the timer accuracy?

    // timeSetEvent is deprecated but is what was used in original game.
    m_timerHandle = timeSetEvent(1000 / freq, 1, Timer_Callback, (DWORD_PTR) nullptr, TIME_PERIODIC);
    s_timerSystemOn = m_timerHandle != 0;
#elif defined(PLATFORM_OSX)
    m_frequency = freq;
    TimerQueue = dispatch_queue_create("platform_timer", nullptr);
    m_timerHandle = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, nullptr, 0, TimerQueue);
    dispatch_source_set_timer(m_timerHandle, DISPATCH_TIME_NOW, 1000000000 / m_frequency, 0);
    dispatch_source_set_event_handler_f(m_timerHandle, Timer_Callback);
    dispatch_resume(m_timerHandle);

// dispatch_time_t dt = dispatch_time(DISPATCH_TIME_NOW, 1000000000 / m_frequency);

// m_timerHandle = dispatch_queue_create("timer queue", nullptr);
// dispatch_after_f(dt, m_timerHandle, nullptr, Timer_Callback);
#else // !PLATFORM_WINDOWS
    m_frequency = freq;

    struct sigevent se;
    struct itimerspec ts;

    // Set up our event callback info
    se.sigev_notify = SIGEV_THREAD; // Type of event is threaded callback
    se.sigev_value.sival_ptr = &m_timerHandle; // Handle for the timer
    se.sigev_notify_function = Timer_Callback; // callback to call
    se.sigev_notify_attributes = nullptr;

    // Set up time interval to call back with
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 1000000000 / m_frequency;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 1000000000 / m_frequency;

    if (timer_create(CLOCK_REALTIME, &se, &m_timerHandle) == 0) {
        timer_settime(m_timerHandle, 0, &ts, nullptr);
        s_timerSystemOn = true;
    }
#endif // PLATFORM_WINDOWS

    if (m_timerHandle != 0 && !partial) {
        PlatformTimer = this;
        TickCount.Start();
    } else {
        DEBUG_LOG("Error - timer system failed to start.\n");
    }
}

PlatformTimerClass::~PlatformTimerClass()
{
#if defined(PLATFORM_WINDOWS)
    if (m_timerHandle) {
        timeKillEvent(m_timerHandle);
    }
    s_timerSystemOn = false;
    timeEndPeriod(1000 / m_frequency);
#elif defined(PLATFORM_OSX)
    if (m_timerHandle) {
        dispatch_source_cancel(m_timerHandle);
    }

    if (TimerQueue) {
        dispatch_release(TimerQueue);
    }
    s_timerSystemOn = false;
#else // !PLATFORM_WINDOWS
    if (m_timerHandle) {
        timer_delete(m_timerHandle);
    }
    s_timerSystemOn = false;
#endif // PLATFORM_WINDOWS
}

void PlatformTimerClass::Update_Tick_Count()
{
    Update_System_Tick_Count();
    Update_User_Tick_Count();
}
