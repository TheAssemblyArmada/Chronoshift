/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling theater specific details.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "theater.h"

#ifndef CHRONOSHIFT_STANDALONE
TheaterType &g_lastTheater = Make_Global<TheaterType>(0x006017CC);
#else
TheaterType g_lastTheater = THEATER_NONE;
#endif

TheaterDataType g_theaters[THEATER_COUNT] = {
    {"TEMPERATE", "TEMPERAT", "TEM"},
    {"SNOW", "SNOW", "SNO"},
    {"INTERIOR", "INTERIOR", "INT"},
};

TheaterType Theater_From_Name(const char *name)
{
    if (name != nullptr) {
        for (int i = 0; i < THEATER_COUNT; ++i) {
            if (stricmp(name, g_theaters[i].name) == 0) {
                return TheaterType(i);
            }
        }
    }

    return THEATER_NONE;
}

const char *Name_From_Theater(TheaterType theater)
{
    if (theater >= THEATER_NONE && theater < THEATER_COUNT) {
        return g_theaters[theater].name;
    }

    return "none";
}
