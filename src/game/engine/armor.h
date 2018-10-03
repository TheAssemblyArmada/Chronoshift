/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Armor enum and functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ARMOR_H
#define ARMOR_H

#include	"always.h"

enum ArmorType {
    ARMOR_INVALID = -1,
    ARMOR_FIRST = 0,
    ARMOR_NONE = 0,
    ARMOR_WOOD,
    ARMOR_LIGHT,
    ARMOR_HEAVY,
    ARMOR_CONCRETE,
    ARMOR_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(ArmorType);

ArmorType Armor_From_Name(const char *name);
const char *Name_From_Armor(ArmorType armor);

#endif // ARMOR_H
