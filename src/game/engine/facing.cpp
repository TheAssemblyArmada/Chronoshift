/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Classes, types and functions related to managing the facings and directions of objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "facing.h"
#include "abs.h"
#include "gamedebug.h"
#include "minmax.h"
#include "swap.h"

uint8_t const FacingClass::Facing8[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t const FacingClass::Facing16[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t const FacingClass::Facing32[256] = {
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6,
    6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8,
    8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10,
    10, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12,
    13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18,
    18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 22, 22,
    22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24,
    24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26,
    26, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28,
    29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 0, 0, 0, 0, 0, 0
};

int8_t const FacingClass::Rotation16[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1
};

const char *FacingName[FACING_COUNT] = {
    "North",
    "North East",
    "East",
    "South East",
    "South",
    "South West",
    "West",
    "North West"
};

const char *FacingNameAbbreviation[FACING_COUNT] = {
    "N",
    "NE",
    "E",
    "SE",
    "S",
    "SW",
    "W",
    "NW"
};

BOOL FacingClass::Set_Current(DirType dir)
{
    if (dir != Current) {
        Current = dir;

        return true;
    }

    return false;
}

BOOL FacingClass::Set_Desired(DirType dir)
{
    if (dir != Desired) {
        Desired = dir;

        return true;
    }

    return false;
}

BOOL FacingClass::Rotation_Adjust(int adjust)
{
    if (Has_Changed()) {
        DirType curr = Current;
        int diff = (int8_t)(Desired - Current); //Fixed watcom optimising DirType to uint8_t and failing diff check.

        adjust = Min(adjust, 127);

        if (Abs(diff) >= adjust) {
            if (diff >= 0) {
                Current += adjust;
            } else {
                Current -= adjust;
            }

        } else {
            Current = Desired;
        }

        return Facing32[Current] != Facing32[curr];
    }

    return false;
}

DirType Desired_Facing8(int x1, int y1, int x2, int y2)
{
    int xdiff = 0;
    int ydiff = 0;
    char dirtype = 0;

    xdiff = x2 - x1;

    if (xdiff < 0) {
        dirtype = -64;
        xdiff = -xdiff;
    }

    ydiff = y1 - y2;

    if (ydiff < 0) {
        dirtype ^= 0x40;
        ydiff = -ydiff;
    }

    unsigned int lower_diff;

    if (xdiff >= ydiff) {
        lower_diff = ydiff;
        ydiff = xdiff;
    } else {
        lower_diff = xdiff;
    }

    char ranged_dir;

    if (((unsigned)(ydiff + 1) >> 1) > lower_diff) {
        ranged_dir = ((unsigned)dirtype) & 64;

        if (xdiff == ydiff) {
            ranged_dir ^= 64;
        }

        return (DirType)(dirtype + ranged_dir);
    }

    return (DirType)(dirtype + 32);
}

DirType Desired_Facing256(int x1, int y1, int x2, int y2)
{
    int8_t unk1 = 0;

    int x_diff = x2 - x1;

    if (x_diff < 0) {
        x_diff = -x_diff;
        unk1 = -64;
    }

    int y_diff = y1 - y2;

    if (y_diff < 0) {
        unk1 ^= 64;
        y_diff = -y_diff;
    }

    int s_diff;
    unsigned l_diff;

    if (x_diff != 0 || y_diff != 0) {
        if (x_diff >= y_diff) {
            s_diff = y_diff;
            l_diff = x_diff;
        } else {
            s_diff = x_diff;
            l_diff = y_diff;
        }

        unsigned unk2 = 32 * s_diff / l_diff;
        int ranged_dir = unk1 & 64;

        if (x_diff > y_diff) {
            ranged_dir = ranged_dir ^ 64;
        }

        if (ranged_dir != 0) {
            unk2 = ranged_dir - unk2 - 1;
        }

        return (DirType)((unk2 + unk1) & 255);
    }

    return DIR_NONE;

}

DirType Facing_To_Direction(FacingType facing)
{
    return (DirType)(facing * 32);
}

FacingType Direction_To_Facing(DirType direction)
{
    // SS
    // return SS_41B6C0(a1, 16) / 32;

    // return (FacingType)((uint8_t)((uint8_t)((unsigned)direction + 16)) / 32);
    return (FacingType)((((unsigned)direction + 16) & 255) / 32);
}

DirType Round_Direction_To_8(DirType dir)
{
    return (DirType)(8 * ((dir + 16) / 8));
}

DirType Round_Direction_To_16(DirType dir)
{
    return (DirType)(16 * ((dir + 16) / 16));
}

DirType Round_Direction_To_32(DirType dir)
{
    return (DirType)(32 * ((dir + 16) / 32));
}

DirType Round_Direction_To_64(DirType dir)
{
    return (DirType)(64 * ((dir + 16) / 64));
}

DirType Round_Direction_To_128(DirType dir)
{
    return (DirType)(128 * ((dir + 16) / 128));
}

FacingType KN_To_Facing(KeyNumType kn)
{
    if (kn != KN_NONE) {
        switch (kn) {
            case KN_4:
            case KN_LEFT:
                return FACING_WEST;

            case KN_6:
            case KN_RIGHT:
                return FACING_EAST;

            case KN_8:
            case KN_UP:
                return FACING_NORTH;

            case KN_2:
            case KN_DOWN:
                return FACING_SOUTH;

            case KN_7:
            case KN_UPLEFT:
                return FACING_NORTH_WEST;

            case KN_9:
            case KN_UPRIGHT:
                return FACING_NORTH_EAST;

            case KN_1:
            case KN_DOWNLEFT:
                return FACING_SOUTH_WEST;

            case KN_3:
            case KN_DOWNRIGHT:
                return FACING_SOUTH_EAST;

            default:
                break;
        }
    }

    return FACING_NONE;
}

const char *Name_From_Facing(FacingType facing, BOOL abbreviated)
{
    // DEBUG_ASSERT(facing != FACING_NONE);
    DEBUG_ASSERT(facing < FACING_COUNT);

    if (facing != FACING_NONE && facing < FACING_COUNT) {
        if (abbreviated) {
            return FacingNameAbbreviation[facing];
        }
        return FacingName[facing];
    }

    return "None";
}

FacingType Facing_From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (stricmp(name, "<none>") == 0 || stricmp(name, "none") == 0) {
        return FACING_NONE;
    }

    if (name != nullptr) {
        for (FacingType facing = FACING_NORTH; facing < FACING_COUNT; ++facing) {
            if (stricmp(name, FacingName[facing]) == 0) {
                return facing;
            }
        }
    }

    return FACING_NONE;
}

const char *Facing_Name_From_Direction(DirType dir)
{
    FacingType facing = Direction_To_Facing(dir);

    if (facing != FACING_NONE && facing < FACING_COUNT) {
        return Name_From_Facing(facing);
    }

    return nullptr;
}

const char *Name_From_Direction(DirType dir)
{
    // TODO rewrite this to correctly adjust negative directions

    switch (dir) {
        case DIR_NORTH:
            return "North";
            break;

        //case -240:
        case DIR_NORTH_NORTH_EAST:
            return "North-northeast";
            break;

        //case -224:
        case DIR_NORTH_EAST:
            return "Northeast";
            break;

        //case -208:
        case DIR_EAST_NORTH_EAST:
            return "East-northeast";
            break;

        //case -192:
        case DIR_EAST:
            return "East";
            break;

        //case -176:
        case DIR_EAST_SOUTH_EAST:
            return "East-southeast";
            break;

        //case -160:
        case DIR_SOUTH_EAST:
            return "Southeast";
            break;

        //case -144:
        case DIR_SOUTH_SOUTH_EAST:
            return "South-southeast";
            break;

        //case -128:
        case DIR_SOUTH:
            return "South";
            break;

        //case -114:
        case DIR_SOUTH_SOUTH_WEST:
            return "South-southwest";
            break;

        //case -96:
        case DIR_SOUTH_WEST:
            return "Southwest";
            break;

        //case -80:
        case DIR_WEST_SOUTH_WEST:
            return "West-southwest";
            break;

        //case -64:
        case DIR_WEST:
            return "West";
            break;

        //case -48:
        case DIR_WEST_NORTH_WEST:
            return "West-northwest";
            break;

        //case -32:
        case DIR_NORTH_WEST:
            return "Northwest";
            break;

        //case -16:
        case DIR_NORTH_WEST_NORTH:
            return "North-northwest";
            break;

        default:
            // return "None";
            return "[Unsupported]";
    };
}
