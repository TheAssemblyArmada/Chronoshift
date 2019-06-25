/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Struct for handling differences between the difficulty levels.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include "always.h"
#include "fixed.h"

class GameINIClass;

enum DiffType
{
    DIFF_NONE = -1,
    DIFF_EASIEST, // very easy?
    DIFF_EASIER, // easy?
    DIFF_NORMAL, // normal
    DIFF_HARDER, // hard?
    DIFF_HARDEST = 4, // very hard?
    DIFF_COUNT
};

struct DifficultyClass
{
    fixed_t Firepower; // Multiplier to firepower for all weapons [larger means more damage].
    fixed_t Groundspeed; // Multiplier to speed for all ground units [larger means faster].
    fixed_t Airspeed; // Multiplier to speed for all air units [larger means faster].
    fixed_t Armor; // Multiplier to armor damage for all units and buildings [smaller means less damage].
    fixed_t ROF; // Multiplier to Rate Of Fire for all weapons [larger means slower ROF].
    fixed_t Cost; // Multiplier to cost for all units and buildings [larger means costlier].
    fixed_t BuildTime; // Multiplier to general object build time [larger means longer to build].
    fixed_t RepairDelay; // Average delay (minutes) between initiating building repair.
    fixed_t BuildDelay; // Average delay (minutes) between initiating construction.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool BuildSlowdown : 1; // & 1 Should the computer build slower than the player.
            bool DestroyWalls : 1; // & 2 Allow scanning for nearby enemy walls and destroy them.
            bool ContentScan : 1; // & 4 Should the contents of a transport be considered when picking best target?
        };
        int Bitfield;
    };
#else
    bool BuildSlowdown; // Should the computer build slower than the player.
    bool DestroyWalls; // Allow scanning for nearby enemy walls and destroy them.
    bool ContentScan; // Should the contents of a transport be considered when picking best target?
#endif
};

void Difficulty_Get(GameINIClass &ini, DifficultyClass &diff, const char *section);
int Fetch_Difficulty_Dialog(BOOL one_time_mission = false);

#endif // DIFFICULTY_H
