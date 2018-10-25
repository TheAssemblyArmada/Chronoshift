/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Class for handling ground types.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GROUND_H
#define GROUND_H

#include "always.h"
#include "fixed.h"
#include "gametypes.h"
#include "land.h"

enum GroundType
{
    GROUND_NONE = -1,

    GROUND_FIRST = 0,

    GROUND_FOOT = 0,
    GROUND_TRACK = 1,
    GROUND_WHEEL = 2,
    GROUND_WINGED = 3,
    GROUND_FLOAT = 4,

    GROUND_COUNT = 5
};

DEFINE_ENUMERATION_OPERATORS(GroundType);

class GameINIClass;

class GroundClass
{
public:
    GroundClass() : Buildable(false) { memcpy(Speeds, 0, sizeof(Speeds)); }

    GroundClass(GroundClass const &that) : Buildable(that.Buildable) { memcpy(Speeds, that.Speeds, sizeof(Speeds)); }

    //~GroundClass() {}

    bool operator==(GroundClass const &that) const
    {
        return memcmp(Speeds, that.Speeds, sizeof(Speeds)) == 0 && Buildable == that.Buildable;
    }

    bool operator!=(GroundClass const &that) const { return !(this == &that); }

    BOOL Read_INI(GameINIClass &ini, const LandType land);
    BOOL Write_INI(GameINIClass &ini, const LandType land) const;

    BOOL const Is_Buildable() { return Buildable; }
    fixed const Get_Speed(SpeedType speed) { return Speeds[speed]; }

    static GroundType From_Name(const char *name);
    static const char *Name_From(GroundType ground);

public:
    // Percent of full speed for each speed type (0.0 means impassable) [def = 1.0]
    fixed Speeds[SPEED_COUNT];

#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Buildable : 1; // & 1
        };
        int Bitfield;
    };
#else
    bool Buildable; // Can buildings be built upon this terrain [def = false]?
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern GroundClass *Ground;
#else
extern GroundClass Ground[LAND_COUNT];
#endif

extern const char *GroundTypes[GROUND_COUNT];

#endif // GROUND_H
