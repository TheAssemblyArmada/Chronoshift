/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Functions for exception handling and log outputting.
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

#ifndef EXCEPTHANDLER_H
#define EXCEPTHANDLER_H

#include "always.h"

#if defined COMPILER_WATCOM || defined COMPILER_MSVC
LONG __cdecl Exception_Handler(unsigned int u, struct _EXCEPTION_POINTERS *e_info);
#endif

#ifdef COMPILER_WATCOM
int __cdecl Watcom_Exception_Handler(
    EXCEPTION_RECORD *ex, void *establisher_frame, CONTEXT *context, void *dispatch_context);
#endif

#endif // EXCEPTHANDLER_H
