/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for converting to and from target values.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "target.h"
#include "coord.h"

int32_t As_Target(int16_t cellnum)
{
    return (((16 * Cell_Get_Y(cellnum) + 8) << 12) | (16 * Cell_Get_X(cellnum) + 8)) & 0xFFFFFF | 0x9000000;
}