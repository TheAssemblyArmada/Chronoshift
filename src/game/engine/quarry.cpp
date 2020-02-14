/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Holds quarry type enum and conversion functions to/from strings.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "quarry.h"
#include <captainslog.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

const char *g_QuarryName[QUARRY_COUNT] = { "N/A",
    "Anything",
    "Buildings - any",
    "Harvesters",
    "Infantry",
    "Vehicles - any",
    "Ships - any",
    "Factories",
    "Base Defenses",
    "Base Threats",
    "Power Facilities",
    "Fake Buildings" };

QuarryType Quarry_From_Name(const char *name)
{
    captainslog_assert(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return QUARRY_NONE;
    }

    if (name != nullptr) {
        for (QuarryType type = QUARRY_FIRST; type < QUARRY_COUNT; ++type) {
            if (strcasecmp(name, g_QuarryName[type]) == 0) {
                return type;
            }
        }
    }

    return QUARRY_NONE;
}

const char *Name_From_Quarry(QuarryType type)
{
    captainslog_assert(type != QUARRY_NONE);
    captainslog_assert(type < QUARRY_COUNT);

    if (type != QUARRY_NONE && type < QUARRY_COUNT) {
        return g_QuarryName[type];
    }

    return "None";
}
