/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Classes providing critical section behaviour.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "critsection.h"
#include <cstdlib>

#ifndef PLATFORM_WINDOWS
#include <unistd.h>
#endif

using std::atoi;

CriticalSectionClass::CriticalSectionClass() : Handle(), Locked(0)
{
#ifdef PLATFORM_WINDOWS
    InitializeCriticalSection(&Handle);
#else
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&Handle, &attr);
#endif // PLATFORM_WINDOWS
}

CriticalSectionClass::~CriticalSectionClass()
{
#ifdef PLATFORM_WINDOWS
    DeleteCriticalSection(&Handle);
#else
    pthread_mutex_destroy(&Handle);
#endif // PLATFORM_WINDOWS
}

/**
 * Performs the lock when entering a critical section of code. Private and is only called from the Lock object.
 */
void CriticalSectionClass::Lock()
{
#ifdef PLATFORM_WINDOWS
    EnterCriticalSection(&Handle);
#else
    pthread_mutex_lock(&Handle);
#endif // PLATFORM_WINDOWS

    ++Locked;
}

/**
 * Removes the lock when leaving a critical section of code. Private and is only called from the Lock object.
 */
void CriticalSectionClass::Unlock()
{
#ifdef PLATFORM_WINDOWS
    LeaveCriticalSection(&Handle);
#else
    pthread_mutex_unlock(&Handle);
#endif // PLATFORM_WINDOWS

    --Locked;
}

/**
 * Performs the lock when entering a critical section of code. Private and is only called from the Lock object.
 */
void FastCriticalSectionClass::Thread_Safe_Set_Flag()
{
#if defined CHRONOSHIFT_STANDALONE && !defined COMPILER_WATCOM
    while (Flag.test_and_set(std::memory_order_seq_cst)) {
#else
    // Should work for both x86_32 and x86_64 plus no assembly.
    while (_interlockedbittestandset(&Flag, 0)) {
#endif
        // Yield the thread if no lock aquired.
#ifdef PLATFORM_WINDOWS
        Sleep(1);
#else
        usleep(1); // TODO test for usleep in build system?
#endif
    }
}

/**
 * Removes the lock when leaving a critical section of code. Private and is only called from the Lock object.
 */
void FastCriticalSectionClass::Thread_Safe_Clear_Flag()
{
    Flag = 0;
}
