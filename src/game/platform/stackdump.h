/**
 * @file
 *
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Functions for creating dumps of information on program crash.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef STACKDUMP_H
#define STACKDUMP_H

#include "always.h"
#include <captainslog.h>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <winver.h>

#ifdef COMPILER_MSVC
#include <dbghelp.h>
#endif

#ifdef COMPILER_WATCOM
#include <imagehlp.h>
#endif

#include <stdlib.h>


void __cdecl Dump_Exception_Info(unsigned int u, struct _EXCEPTION_POINTERS *e_info);
BOOL Init_Symbol_Info();
void Uninit_Symbol_Info();
void __cdecl Stack_Dump_Handler(const char *data);
void Make_Stack_Trace(
    uintptr_t myeip, uintptr_t myesp, uintptr_t myebp, int skipFrames, void(__cdecl *callback)(char const *));

extern char g_ExceptionFileBuffer[65536];
#endif

#endif // STACKDUMP_H
