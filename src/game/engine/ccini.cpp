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
#include "ccini.h"
#include "minmax.h"

CCINIClass::CCINIClass() : DigestValid(false) {}

int16_t CCINIClass::Get_Lepton(const char *section, const char *entry, const int16_t defvalue) const
{
    return Get_Fixed(section, entry, fixed(defvalue, 256)) * 256;
}

MPHType CCINIClass::Get_MPHType(const char *section, const char *entry, const MPHType defvalue) const
{
    int value = Clamp(Get_Int(section, entry, (100 * defvalue) / 256), 0, 100);

    return (MPHType)Min((value * 256) / 100, 255);
}

CrateType CCINIClass::Get_CrateType(const char *section, const char *entry, const CrateType defvalue) const
{
    char value[INIClass::MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", value, sizeof(value)) > 0) {
        return CrateClass::Crate_From_Name(value);
    }

    return defvalue;
}

VocType CCINIClass::Get_VocType(const char *section, const char *entry, const VocType defvalue) const
{
    char value[INIClass::MAX_LINE_LENGTH];

    if (Get_String(section, entry, Voc_Name(defvalue), value, sizeof(value)) > 0) {
        return Voc_From_Name(value);
    }

    return defvalue;
}

AnimType CCINIClass::Get_AnimType(const char *section, const char *entry, const AnimType defvalue) const
{
    char value[INIClass::MAX_LINE_LENGTH];

    if (Get_String(section, entry, AnimTypeClass::Name_From(defvalue), value, sizeof(value)) > 0) {
        return AnimTypeClass::From_Name(value);
    }

    return defvalue;
}

WarheadType CCINIClass::Get_WarheadType(const char *section, const char *entry, const WarheadType defvalue) const
{
    char value[INIClass::MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", value, sizeof(value)) > 0) {
        for (WarheadType wh = WARHEAD_FIRST; wh < WARHEAD_COUNT; ++wh) {
            if (strcasecmp(WarheadTypeClass::As_Reference(wh).Get_Name(), value) == 0) {
                return wh;
            }
        }
    }

    return defvalue;
}

BulletType CCINIClass::Get_BulletType(const char *section, const char *entry, const BulletType defvalue) const
{
    char value[INIClass::MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", value, sizeof(value)) > 0) {
        for (BulletType bt = BULLET_FIRST; bt < BULLET_COUNT; ++bt) {
            if (strcasecmp(BulletTypeClass::As_Reference(bt).Get_Name(), value) == 0) {
                return bt;
            }
        }
    }

    return defvalue;
}
