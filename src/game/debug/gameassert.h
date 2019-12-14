/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Debug assertion interface.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMEASSERT_H
#define GAMEASSERT_H

#include "always.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CHRONOSHIFT_ASSERTS

extern bool g_ExitOnAssert; // Exit application on assertion when break button is pressed?
extern bool g_IgnoreAllAsserts; // Ignore all assertionss.
extern int g_GlobalIgnoreCount; // The number of assertions to ignore on a global basis.
extern int g_TotalAssertions; // The total number of assertions.
extern bool g_BreakOnException; // Break to debugger when a throw assertion is triggered.

enum
{
    ASSERT_BUFFER_SIZE = 4096
};

void Debug_Assert(char const *expr, char const *file, int const line, char const *func, char const *msg, volatile bool &_ignore, volatile bool &_break);

#endif // CHRONOSHIFT_ASSERTS

#ifdef __cplusplus
} // extern "C"
#endif

#endif // GAMEASSERT_H
