/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief  Disk change handling.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CD_H
#define CD_H

#include "always.h"

enum DiskType
{
    DISK_ANY = -2, // Any disk or none at all.
    DISK_CDCHECK = -1, // Any disk, but must have a disk in drive. Acts as no-cd check.
    DISK_FIRST = 0,
    DISK_ALLIED = 0,
    DISK_SOVIET,
    DISK_COUNTERSTRIKE,
    DISK_AFTERMATH,
    DISK_EXPANSION, // Either CS or AM. If neither is already in, it will ask for AM.
    DISK_DVD, // Unused as DVD was never released.
    DISK_COUNT,
};

int Get_CD_Index(int drive, int delay);
BOOL Force_CD_Available(int cd);

#ifdef GAME_DLL
extern int &g_CurrentCD;
extern int &g_RequiredCD;
#else
extern int g_CurrentCD;
extern int g_RequiredCD;
#endif

#endif // CD_H
