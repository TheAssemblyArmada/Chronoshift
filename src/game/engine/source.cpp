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
#include "source.h"

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

const char *SourceName[SOURCE_COUNT] = {
    "North",
    "East",
    "South",
    "West",
    "Air"
};

SourceType Source_From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return SOURCE_NONE;
    }

    if (name != nullptr) {
        for (SourceType source = SOURCE_FIRST; source < SOURCE_COUNT; ++source) {
            if (strcasecmp(name, SourceName[source]) == 0) {
                return source;
            }
        }
    }

    return SOURCE_NONE;
}

const char *Name_From_Source(SourceType source)
{
    if (source < SOURCE_COUNT && source != SOURCE_NONE) {
        return SourceName[source];
    }
    return "<none>";
}
