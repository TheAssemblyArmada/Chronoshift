/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Basic header files and defines that are always needed.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ALWAYS_H
#define ALWAYS_H

#include "config.h"

#include "bittype.h"
#include "compiler.h"
#include "intrinsics.h"
#include "macros.h"
#include "platform.h"
#include "targetver.h"
#include <sys/stat.h>
#include <sys/types.h>

#ifdef COMPILER_WATCOM
#include <direct.h>
#include <stdio.h> // For PATH_MAX
#undef PATH_MAX // Undefine these as this causes a define error through the codebase.
#undef NAME_MAX
#endif // COMPILER_WATCOM

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#define NAME_MAX FILENAME_MAX
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#endif // PLATFORM_WINDOWS

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

//  Define nullptr when standard is less than C++11, mainly for watcom support
#if __cplusplus <= 199711L && !defined COMPILER_MSVC
#define nullptr NULL
#define override
#define final
#define static_assert(x, ...)
#endif

#ifndef BOOL
typedef int BOOL;
#endif

// Allow inline recursive functions within inline recursive functions in msvc.
#ifdef COMPILER_MSVC
#pragma inline_recursion(on)
#endif

// This section defines some keywords defining calling conventions
// where the keywords needed differ between compilers.
#if !defined COMPILER_MSVC && !defined COMPILER_WATCOM
#if !defined(__fastcall)
#if __has_attribute(fastcall)
#define __fastcall __attribute__((fastcall))
#else
#define __fastcall
#endif
#endif

#if !defined(__cdecl)
#if __has_attribute(cdecl)
#define __cdecl __attribute__((cdecl))
#else
#define __cdecl
#endif
#endif

#if !defined(__stdcall)
#if __has_attribute(stdcall)
#define __stdcall __attribute__((stdcall))
#else
#define __stdcall
#endif
#endif
#endif // !defined COMPILER_MSVC || !defined COMPILER_WATCOM

// This section defines some keywords controlling inlining and unused variables
// where the keywords needed differ between compilers.
#ifdef COMPILER_MSVC
#define __noinline __declspec(noinline)
#define __unused __pragma(warning(suppress : 4100 4101))
#else
#if !defined(__forceinline)
#if __has_attribute(__always_inline__)
#define __forceinline inline __attribute__((__always_inline__))
#else
#define __forceinline inline
#endif
#endif

#if !defined(__unused)
#if __has_attribute(unused)
#define __unused __attribute__((unused))
#else
#define __unused
#endif
#endif

#if !defined(__noinline)
#if __has_attribute(noinline)
#define __noinline __attribute__((noinline))
#else
#define __noinline
#endif
#endif
#endif // COMPILER_MSVC

// Based on the build system generated config.h information we define some stuff here
// for cross platform consistency.
#if defined HAVE__STRICMP && !defined HAVE_STRCASECMP
#define strcasecmp _stricmp
#endif

#if defined HAVE__STRNICMP && !defined HAVE_STRNCASECMP
#define strncasecmp _strnicmp
#endif

// TODO Have cmake build system detect requirements for 64bit stat.
typedef struct stat stat_t;

// This section is for setting various pragmas for compilers for warnings we don't care about
// when they can't be disabled from the command line in the build system.
#if defined(COMPILER_WATCOM)
// Turns off stack checking.
#pragma off(check_stack)

// Turn all warnings into errors.
//#pragma warning * 0

// No reference to symbol.
#pragma warning 14 9

// Disables warning when "sizeof" is used on an object with virtual functions.
#pragma warning 549 9

// Disable the "Integral value may be truncated during assignment or initialization".
//#pragma warning 389 9

// Allow constructing a temporary to be used as a parameter.
//#pragma warning 604 9

// Disable the construct resolved as an expression warning.
//#pragma warning 595 9

// Disable the strange "construct resolved as a declaration/type" warning.
//#pragma warning 594 9

// Disable the "pre-compiled header file cannot be used" warning.
//#pragma warning 698 9

// Disable the "temporary object used to initialize a non-constant reference" warning.
//#pragma warning 665 9

// Disable the "pointer or reference truncated by cast. Cast is supposed to REMOVE warnings, not create them.
//#pragma warning 579 9

// Disable the warning that suggests a null destructor be placed in class definition.
//#pragma warning 656 9

// Disable the warning about moving empty constructors/destructors to the class declaration.
//#pragma warning 657 9

// No virtual destructor is not an error.
#pragma warning 004 9

// Integral constant will be truncated warning is usually ok when dealing with bitfields.
//#pragma warning 388 9

#endif

#endif // ALWAYS_H
