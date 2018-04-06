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
#include "animtype.h"
#include "audio.h"
#include "bullettype.h"
#include "crate.h"
#include "gametypes.h"
#include "ini.h"
#include "sha.h"
#include "warheadtype.h"

class CCINIClass : public INIClass
{
public:
    CCINIClass();

    int16_t Get_Lepton(const char *section, const char *entry, const int16_t defvalue = 0) const;
    MPHType Get_MPHType(const char *section, const char *entry, const MPHType defvalue = MPH_MIN) const;
    CrateType Get_CrateType(const char *section, const char *entry, const CrateType defvalue = CRATE_MONEY) const;
    VocType Get_VocType(const char *section, const char *entry, const VocType defvalue = VOC_NONE) const;
    AnimType Get_AnimType(const char *section, const char *entry, const AnimType defvalue = ANIM_NONE) const;
    WarheadType Get_WarheadType(const char *section, const char *entry, const WarheadType defvalue = WARHEAD_NONE) const;
    BulletType Get_BulletType(const char *section, const char *entry, const BulletType defvalue = BULLET_NONE) const;

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
