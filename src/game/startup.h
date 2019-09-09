/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Startup code.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef STARTUP_H
#define STARTUP_H

#include "always.h"

class FileClass;

void Emergency_Exit(int exit_code);
BOOL InitDDraw();
void Read_Setup_Options(FileClass *fc); // Original takes RawFileClass specifically
uint64_t Disk_Space_Available(); // don't hook this, abi does not match.

#ifdef GAME_DLL
#include "hooker.h"
#endif

#endif // STARTUP_H
