/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Source type enum and conversion to and from strings.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SOURCE_H
#define SOURCE_H

#include "always.h"

enum SourceType
{
    SOURCE_NONE = -1,

    SOURCE_FIRST = 0,

    SOURCE_NORTH = 0,
    SOURCE_EAST = 1,
    SOURCE_SOUTH = 2,
    SOURCE_WEST = 3,
    SOURCE_AIR = 4,

    SOURCE_COUNT = 5
};

DEFINE_ENUMERATION_OPERATORS(SourceType);

SourceType Source_From_Name(const char *name);
const char *Name_From_Source(SourceType source);

#endif // SOURCE_H
