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
    SOURCE_NORTH,
    SOURCE_EAST,
    SOURCE_SOUTH,
    SOURCE_WEST,
    SOURCE_AIR,

    SOURCE_COUNT,

    SOURCE_NONE = -1,
    SOURCE_FIRST = SOURCE_NORTH
};

DEFINE_ENUMERATION_OPERATORS(SourceType);

SourceType Source_From_Name(const char *name);
const char *Name_From_Source(SourceType source);

#endif // SOURCE_H
