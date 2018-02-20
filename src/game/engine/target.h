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
#pragma once

#ifndef TARGET_H
#define TARGET_H

#include "always.h"
#include "rtti.h"

int32_t As_Target(int16_t cellnum);

inline RTTIType Target_Get_RTTI(int32_t target)
{
    return RTTIType((target & 0xFF000000) >> 24);
}

#endif // TARGET_H
