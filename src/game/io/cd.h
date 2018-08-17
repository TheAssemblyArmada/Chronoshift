/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief  Disk change handling.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
    DISK_ANY = -2, // Any disk will do, normally used when disk specific info not needed.
    DISK_NONE = -1,
    DISK_FIRST = 0,
    DISK_ALLIED = 0,
    DISK_SOVIET = 1,
    DISK_COUNTERSTRIKE = 2,
    DISK_AFTERMATH = 3,
    DISK_EXPANSION = 4, // A catch all that will be valid for both CS and AM.
    DISK_DVD = 5,
    DISK_COUNT = 6,
};

int Get_CD_Index(int drive, int delay);
BOOL Force_CD_Available(int cd);

#ifndef CHRONOSHIFT_STANDALONE
extern int &g_currentCD;
extern int &g_requiredCD;
#else
extern int g_currentCD;
extern int g_requiredCD;
#endif

#endif // CD_H
