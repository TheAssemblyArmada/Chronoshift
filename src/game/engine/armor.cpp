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
#include "armor.h"
#include "gamedebug.h"

#ifndef PLATFORM_WINDOWS
#include <strings.h>
#endif

const char *ArmorName[ARMOR_COUNT] = { "none", "wood", "light", "heavy", "concrete" };

ArmorType Armor_From_Name(const char *name)
{
    if (name != nullptr) {
        for (ArmorType armor = ARMOR_NONE; armor < ARMOR_COUNT; ++armor) {
            if (strcasecmp(name, ArmorName[armor]) == 0) {
                return armor;
            }
        }
    }

    return ARMOR_NONE;
}

const char *Name_From_Armor(ArmorType armor)
{
    if (armor >= ARMOR_NONE && armor < ARMOR_COUNT) {
        return ArmorName[armor];
    }

    return "none";
}
