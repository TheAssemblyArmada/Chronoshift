/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief INI file parsing for game specific objects/types.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CCINI_H
#define CCINI_H

#include "always.h"
#include "ini.h"
#include "sha.h"

class CCINIClass : public INIClass
{
public:
    CCINIClass();

    // TODO implement functions as required.
private:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool DigestValid : 1; // & 1
        };
        int Bitfield;
    };
#else
    bool DigestValid;
#endif
    SHAEngine::SHADigest Digest;
};

#endif // CCINI_H
