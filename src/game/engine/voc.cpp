/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Voc enum and conversion to and from a string.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "voc.h"

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

// clang-format off
SoundEffectType SoundEffectName[] = {
    { "GIRLOKAY", 20, VAR_NONE },
    { "GIRLYEAH", 20, VAR_NONE },
    { "GUYOKAY1", 20, VAR_NONE },
    { "GUYYEAH1", 20, VAR_NONE },
    { "MINELAY1", 5, VAR_NONE }, // BUGFIX: Changed from HOUSE to NONE, as it doesnt have a variation file.
    { "ACKNO", 20, VAR_HOUSE },
    { "AFFIRM1", 20, VAR_HOUSE },
    { "AWAIT1", 20, VAR_HOUSE },
    { "EAFFIRM1", 20, VAR_NONE },
    { "EENGIN1", 20, VAR_NONE },
    { "NOPROB", 20, VAR_HOUSE },
    { "READY", 20, VAR_HOUSE },
    { "REPORT1", 20, VAR_HOUSE },
    { "RITAWAY", 20, VAR_HOUSE },
    { "ROGER", 20, VAR_HOUSE },
    { "UGOTIT", 20, VAR_HOUSE },
    { "VEHIC1", 20, VAR_HOUSE },
    { "YESSIR1", 20, VAR_HOUSE },
    { "DEDMAN1", 10, VAR_NONE },
    { "DEDMAN2", 10, VAR_NONE },
    { "DEDMAN3", 10, VAR_NONE },
    { "DEDMAN4", 10, VAR_NONE },
    { "DEDMAN5", 10, VAR_NONE },
    { "DEDMAN6", 10, VAR_NONE },
    { "DEDMAN7", 10, VAR_NONE },
    { "DEDMAN8", 10, VAR_NONE },
    { "DEDMAN10", 10, VAR_NONE },
    { "CHRONO2", 5, VAR_NONE },
    { "CANNON1", 1, VAR_NONE },
    { "CANNON2", 1, VAR_NONE },
    { "IRONCUR9", 10, VAR_NONE },
    { "EMOVOUT1", 20, VAR_NONE },
    { "SONPULSE", 10, VAR_NONE },
    { "SANDBAG2", 5, VAR_NONE },
    { "MINEBLO1", 5, VAR_NONE },
    { "CHUTE1", 1, VAR_NONE },
    { "DOGY1", 5, VAR_NONE },
    { "DOGW5", 10, VAR_NONE },
    { "DOGG5P", 10, VAR_NONE },
    { "FIREBL3", 1, VAR_NONE },
    { "FIRETRT1", 1, VAR_NONE },
    { "GRENADE1", 1, VAR_NONE },
    { "GUN11", 1, VAR_NONE },
    { "GUN13", 1, VAR_NONE },
    { "EYESSIR1", 1, VAR_NONE },
    { "GUN27", 1, VAR_NONE },
    { "HEAL2", 1, VAR_NONE },
    { "HYDROD1", 1, VAR_NONE },
    { "INVUL2", 1, VAR_NONE },
    { "KABOOM1", 1, VAR_NONE },
    { "KABOOM12", 1, VAR_NONE },
    { "KABOOM15", 1, VAR_NONE },
    { "SPLASH9", 5, VAR_NONE },
    { "KABOOM22", 5, VAR_NONE },
    { "AACANON3", 1, VAR_NONE },
    { "TANDETH1", 1, VAR_NONE },
    { "MGUNINF1", 1, VAR_NONE },
    { "MISSILE1", 1, VAR_NONE },
    { "MISSILE6", 1, VAR_NONE },
    { "MISSILE7", 1, VAR_NONE },
    { "ORETRUK1", 1, VAR_NONE },
    { "PILLBOX1", 1, VAR_NONE },
    { "RABEEP1", 1, VAR_NONE },
    { "RAMENU1", 1, VAR_NONE },
    { "SILENCER", 1, VAR_NONE },
    { "TANK5", 1, VAR_NONE },
    { "TANK6", 1, VAR_NONE },
    { "TORPEDO1", 1, VAR_NONE },
    { "TURRET1", 1, VAR_NONE },
    { "TSLACHG2", 10, VAR_NONE },
    { "TESLA1", 10, VAR_NONE },
    { "SQUISHY2", 10, VAR_NONE },
    { "SCOLDY1", 10, VAR_NONE },
    { "RADARON2", 10, VAR_NONE },
    { "RADARDN1", 10, VAR_NONE },
    { "PLACBLDG", 10, VAR_NONE },
    { "KABOOM30", 10, VAR_NONE },
    { "KABOOM25", 10, VAR_NONE },
    { "KABOOM21", 10, VAR_NONE },
    { "DOGW7", 10, VAR_NONE },
    { "DOGW3PX", 10, VAR_NONE },
    { "CRMBLE2", 10, VAR_NONE },
    { "CASHUP1", 10, VAR_NONE },
    { "CASHDN1", 10, VAR_NONE },
    { "BUILD5", 10, VAR_NONE },
    { "BLEEP9", 10, VAR_NONE },
    { "BLEEP6", 10, VAR_NONE },
    { "BLEEP5", 10, VAR_NONE },
    { "BLEEP17", 10, VAR_NONE },
    { "BLEEP13", 10, VAR_NONE },
    { "BLEEP12", 10, VAR_NONE },
    { "BLEEP11", 10, VAR_NONE },
    { "H2OBOMB2", 10, VAR_NONE },
    { "CASHTURN", 10, VAR_NONE },
    { "TUFFGUY1", 20, VAR_NONE },
    { "ROKROLL1", 20, VAR_NONE },
    { "LAUGH1", 20, VAR_NONE },
    { "CMON1", 20, VAR_NONE },
    { "BOMBIT1", 20, VAR_NONE },
    { "GOTIT1", 20, VAR_NONE },
    { "KEEPEM1", 20, VAR_NONE },
    { "ONIT1", 20, VAR_NONE },
    { "LEFTY1", 20, VAR_NONE },
    { "YEAH1", 20, VAR_NONE },
    { "YES1", 20, VAR_NONE },
    { "YO1", 20, VAR_NONE },
    { "WALLKIL2", 5, VAR_NONE },
    { "MRJ7", 10, VAR_NONE },
    { "GUN5", 5, VAR_NONE },
    { "SUBSHOW1", 5, VAR_NONE },
    { "EINAH1", 20, VAR_NONE },
    { "EINOK1", 20, VAR_NONE },
    { "EINYES1", 20, VAR_NONE },
    { "MINE1", 20, VAR_NONE },
    { "SCOMND1", 20, VAR_NONE },
    { "SYESSIR1", 20, VAR_NONE },
    { "SINDEED1", 20, VAR_NONE },
    { "SONWAY1", 20, VAR_NONE },
    { "SKING1", 20, VAR_NONE },
    { "MRESPON1", 20, VAR_NONE },
    { "MYESSIR1", 20, VAR_NONE },
    { "MAFFIRM1", 20, VAR_NONE },
    { "MMOVOUT1", 20, VAR_NONE },
    { "BEEPSLCT", 20, VAR_NONE },
    { "SYEAH1", 20, VAR_NONE },
    { "ANTDIE", 20, VAR_NONE },
    { "ANTBITE", 20, VAR_NONE },
    { "SMOUT1", 20, VAR_NONE },
    { "SOKAY1", 20, VAR_NONE },
    { "SSTEAL1", 20, VAR_NONE },
    { "SWHAT1", 20, VAR_NONE },
    { "SAFFIRM1", 20, VAR_NONE },
    { "STAVCMDR", 20, VAR_NONE },
    { "STAVCRSE", 20, VAR_NONE },
    { "STAVYES", 20, VAR_NONE },
    { "STAVMOV", 20, VAR_NONE },
    { "BUZZY1", 20, VAR_NONE },
    { "RAMBO1", 20, VAR_NONE },
    { "RAMBO2", 20, VAR_NONE },
    { "RAMBO3", 20, VAR_NONE },
    { "MYES1", 20, VAR_NONE },
    { "MHOWDY1", 20, VAR_NONE },
    { "MRISE1", 20, VAR_NONE },
    { "MHUH1", 20, VAR_NONE },
    { "MHEAR1", 20, VAR_NONE },
    { "MLAFF1", 20, VAR_NONE },
    { "MBOSS1", 20, VAR_NONE },
    { "MYEEHAW1", 20, VAR_NONE },
    { "MHOTDIG1", 20, VAR_NONE },
    { "MWRENCH1", 20, VAR_NONE },
    { "JBURN1", 20, VAR_NONE },
    { "JCHRGE1", 20, VAR_NONE },
    { "JCRISP1", 20, VAR_NONE },
    { "JDANCE1", 20, VAR_NONE },
    { "JJUICE1", 20, VAR_NONE },
    { "JJUMP1", 20, VAR_NONE },
    { "JLIGHT1", 20, VAR_NONE },
    { "JPOWER1", 20, VAR_NONE },
    { "JSHOCK1", 20, VAR_NONE },
    { "JYES1", 20, VAR_NONE },
    { "CHROTNK1", 20, VAR_NONE },
    { "FIXIT1", 20, VAR_NONE },
    { "MADCHRG2", 20, VAR_NONE },
    { "MADEXPLO", 20, VAR_NONE },
    { "SHKTROP1", 20, VAR_NONE }
};
// clang-format on

VocType Voc_From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return VOC_NONE;
    }

    if (name != nullptr) {
        for (VocType voc = VOC_FIRST; voc < VOC_COUNT; ++voc) {
            if (strcasecmp(name, SoundEffectName[voc].BaseName)) {
                return voc;
            }
        }
    }
    return VOC_NONE;
}

const char *Name_From_Voc(VocType voc)
{
    if (voc >= VOC_FIRST && voc < VOC_COUNT) {
        return SoundEffectName[voc].BaseName;
    }
    return "<none>";
}

void Sound_Effect(VocType voc, coord_t location, int int2, HousesType house)
{
#ifdef GAME_DLL
    void (*func)(VocType, coord_t, int, HousesType) =
        reinterpret_cast<void (*)(VocType, coord_t, int, HousesType)>(0x00425D1C);
    func(voc, location, int2, house);
#endif
}

int Sound_Effect(VocType voc, fixed_t volume, int int1, int16_t pan, HousesType house)
{
#ifdef GAME_DLL
    int (*func)(VocType, fixed_t, int, int16_t, HousesType) =
        reinterpret_cast<int (*)(VocType, fixed_t, int, int16_t, HousesType)>(0x00425F24);
    return func(voc, volume, int1, pan, house);
#else
    return 0;
#endif
}
