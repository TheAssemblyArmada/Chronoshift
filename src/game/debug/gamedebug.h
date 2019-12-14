/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Macros for making use of the various debugging functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMEDEBUG_H
#define GAMEDEBUG_H

#include "always.h"
#include "gameassert.h"
#include "gamelogging.h"

#ifdef CHRONOSHIFT_LOGGING

#define DEBUG_INIT(flags) Debug_Init(flags)
#define DEBUG_STOP() Debug_Shutdown()

#define DEBUG_LOG(msg, ...) Debug_Log(msg, ##__VA_ARGS__)
#define DEBUG_LINE_LOG(msg, ...) Debug_Log("%s %d " msg, __FILE__, __LINE__, ##__VA_ARGS__)

#else // !CHRONOSHIFT_LOGGING

#define DEBUG_INIT(flags) ((void)0)
#define DEBUG_STOP() ((void)0)

#define DEBUG_LOG(msg, ...) ((void)0)
#define DEBUG_LINE_LOG(msg, ...) ((void)0)

#endif // CHRONOSHIFT_LOGGING

#ifdef CHRONOSHIFT_ASSERTS
#define DEBUG_ASSERT(exp) \
    if (!(exp)) { \
        static volatile bool _ignore_assert = false; \
        static volatile bool _break = false; \
        if (!_ignore_assert) { \
            DEBUG_LOG( \
                "ASSERTION FAILED!\n" \
                "  File:%s\n  Line:%d\n  Function:%s\n  Expression:%s\n\n", \
                __FILE__, \
                __LINE__, \
                __CURRENT_FUNCTION__, \
                #exp); \
            Debug_Assert(#exp, __FILE__, __LINE__, __CURRENT_FUNCTION__, nullptr, _ignore_assert, _break); \
        } \
        if (_break) { \
            __debugbreak(); \
        } \
    }

#define DEBUG_ASSERT_PRINT(exp, msg, ...) \
    if (!(exp)) { \
        DEBUG_LOG( \
            "ASSERTION FAILED!\n" \
            "  File:%s\n  Line:%d\n  Function:%s\n  Expression:%s\n  Message:" msg "\n\n", \
            __FILE__, \
            __LINE__, \
            __CURRENT_FUNCTION__, \
            #exp, \
            ##__VA_ARGS__); \
    }

#define DEBUG_ASSERT_THROW(exp, msg, ...) \
    if (!(exp)) { \
        DEBUG_LOG( \
            "ASSERTION FAILED!\n" \
            "  File:%s\n  Line:%d\n  Function:%s\n  Expression:%s\n  Message:" msg "\n\n", \
            __FILE__, \
            __LINE__, \
            __CURRENT_FUNCTION__, \
            #exp, \
            ##__VA_ARGS__); \
        if (g_BreakOnException) { \
            __debugbreak(); \
        } \
        throw except; \
    }

#else // !CHRONOSHIFT_ASSERTS

#define DEBUG_ASSERT(exp) if (!(exp)) {}
#define DEBUG_ASSERT_PRINT(exp, msg, ...) if (!(exp)) {}
#define DEBUG_ASSERT_THROW(exp, msg, ...) if (!(exp)) {}

#endif // CHRONOSHIFT_ASSERTS

#endif // GAMEDEBUG_H
