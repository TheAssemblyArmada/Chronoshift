/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling theater specific details.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef THEATER_H
#define THEATER_H

#include	"always.h"

enum TheaterType
{
    THEATER_NONE = -1,
    THEATER_TEMPERATE,
    THEATER_SNOW,
    THEATER_INTERIOR,
    THEATER_COUNT,
    THEATER_DEFAULT = THEATER_TEMPERATE,
};

enum TheaterBitEnum {
    THEATER_BIT_NONE = 0,
    THEATER_BIT_TEMPERATE = 1 << THEATER_TEMPERATE,
    THEATER_BIT_SNOW = 1 << THEATER_SNOW,
    THEATER_BIT_INTERIOR = 1 << THEATER_INTERIOR,
};

struct TheaterDataType
{
    const char name[16];
    const char data[10];
    const char ext[4];
};

TheaterType Theater_From_Name(const char *name);

extern TheaterDataType g_theaters[THEATER_COUNT];

#ifndef RAPP_STANDALONE
#include "hooker.h"
extern TheaterType &g_lastTheater;
#else
extern TheaterType g_lastTheater;
#endif

#endif // THEATER_H
