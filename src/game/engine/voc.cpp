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

SoundEffectType SoundEffectName[] = {
    { "GIRLOKAY", 20, 0 },
    { "GIRLYEAH", 20, 0 },
    { "GUYOKAY1", 20, 0 },
    { "GUYYEAH1", 20, 0 },
    { "MINELAY1", 5, 0 }, // Changed from 1 to 0 as it doesnt have a variation.
    { "ACKNO", 20, 1 },
    { "AFFIRM1", 20, 1 },
    { "AWAIT1", 20, 1 },
    { "EAFFIRM1", 20, 0 },
    { "EENGIN1", 20, 0 },
    { "NOPROB", 20, 1 },
    { "READY", 20, 1 },
    { "REPORT1", 20, 1 },
    { "RITAWAY", 20, 1 },
    { "ROGER", 20, 1 },
    { "UGOTIT", 20, 1 },
    { "VEHIC1", 20, 1 },
    { "YESSIR1", 20, 1 },
    { "DEDMAN1", 10, 0 },
    { "DEDMAN2", 10, 0 },
    { "DEDMAN3", 10, 0 },
    { "DEDMAN4", 10, 0 },
    { "DEDMAN5", 10, 0 },
    { "DEDMAN6", 10, 0 },
    { "DEDMAN7", 10, 0 },
    { "DEDMAN8", 10, 0 },
    { "DEDMAN10", 10, 0 },
    { "CHRONO2", 5, 0 },
    { "CANNON1", 1, 0 },
    { "CANNON2", 1, 0 },
    { "IRONCUR9", 10, 0 },
    { "EMOVOUT1", 20, 0 },
    { "SONPULSE", 10, 0 },
    { "SANDBAG2", 5, 0 },
    { "MINEBLO1", 5, 0 },
    { "CHUTE1", 1, 0 },
    { "DOGY1", 5, 0 },
    { "DOGW5", 10, 0 },
    { "DOGG5P", 10, 0 },
    { "FIREBL3", 1, 0 },
    { "FIRETRT1", 1, 0 },
    { "GRENADE1", 1, 0 },
    { "GUN11", 1, 0 },
    { "GUN13", 1, 0 },
    { "EYESSIR1", 1, 0 },
    { "GUN27", 1, 0 },
    { "HEAL2", 1, 0 },
    { "HYDROD1", 1, 0 },
    { "INVUL2", 1, 0 },
    { "KABOOM1", 1, 0 },
    { "KABOOM12", 1, 0 },
    { "KABOOM15", 1, 0 },
    { "SPLASH9", 5, 0 },
    { "KABOOM22", 5, 0 },
    { "AACANON3", 1, 0 },
    { "TANDETH1", 1, 0 },
    { "MGUNINF1", 1, 0 },
    { "MISSILE1", 1, 0 },
    { "MISSILE6", 1, 0 },
    { "MISSILE7", 1, 0 },
    { "ORETRUK1", 1, 0 },
    { "PILLBOX1", 1, 0 },
    { "RABEEP1", 1, 0 },
    { "RAMENU1", 1, 0 },
    { "SILENCER", 1, 0 },
    { "TANK5", 1, 0 },
    { "TANK6", 1, 0 },
    { "TORPEDO1", 1, 0 },
    { "TURRET1", 1, 0 },
    { "TSLACHG2", 10, 0 },
    { "TESLA1", 10, 0 },
    { "SQUISHY2", 10, 0 },
    { "SCOLDY1", 10, 0 },
    { "RADARON2", 10, 0 },
    { "RADARDN1", 10, 0 },
    { "PLACBLDG", 10, 0 },
    { "KABOOM30", 10, 0 },
    { "KABOOM25", 10, 0 },
    { "KABOOM21", 10, 0 },
    { "DOGW7", 10, 0 },
    { "DOGW3PX", 10, 0 },
    { "CRMBLE2", 10, 0 },
    { "CASHUP1", 10, 0 },
    { "CASHDN1", 10, 0 },
    { "BUILD5", 10, 0 },
    { "BLEEP9", 10, 0 },
    { "BLEEP6", 10, 0 },
    { "BLEEP5", 10, 0 },
    { "BLEEP17", 10, 0 },
    { "BLEEP13", 10, 0 },
    { "BLEEP12", 10, 0 },
    { "BLEEP11", 10, 0 },
    { "H2OBOMB2", 10, 0 },
    { "CASHTURN", 10, 0 },
    { "TUFFGUY1", 20, 0 },
    { "ROKROLL1", 20, 0 },
    { "LAUGH1", 20, 0 },
    { "CMON1", 20, 0 },
    { "BOMBIT1", 20, 0 },
    { "GOTIT1", 20, 0 },
    { "KEEPEM1", 20, 0 },
    { "ONIT1", 20, 0 },
    { "LEFTY1", 20, 0 },
    { "YEAH1", 20, 0 },
    { "YES1", 20, 0 },
    { "YO1", 20, 0 },
    { "WALLKIL2", 5, 0 },
    { "<NONE>", 10, 0 },
    { "GUN5", 5, 0 },
    { "SUBSHOW1", 5, 0 },
    { "EINAH1", 20, 0 },
    { "EINOK1", 20, 0 },
    { "EINYES1", 20, 0 },
    { "MINE1", 20, 0 },
    { "SCOMND1", 20, 0 },
    { "SYESSIR1", 20, 0 },
    { "SINDEED1", 20, 0 },
    { "SONWAY1", 20, 0 },
    { "SKING1", 20, 0 },
    { "MRESPON1", 20, 0 },
    { "MYESSIR1", 20, 0 },
    { "MAFFIRM1", 20, 0 },
    { "MMOVOUT1", 20, 0 },
    { "BEEPSLCT", 20, 0 },
    { "SYEAH1", 20, 0 },
    { "ANTDIE", 20, 0 },
    { "ANTBITE", 20, 0 },
    { "SMOUT1", 20, 0 },
    { "SOKAY1", 20, 0 },
    { "<NONE>", 20, 0 },
    { "SWHAT1", 20, 0 },
    { "SAFFIRM1", 20, 0 },
    { "STAVCMDR", 20, 0 },
    { "STAVCRSE", 20, 0 },
    { "STAVYES", 20, 0 },
    { "STAVMOV", 20, 0 },
    { "BUZZY1", 20, 0 },
    { "RAMBO1", 20, 0 },
    { "RAMBO2", 20, 0 },
    { "RAMBO3", 20, 0 },
    { "MYES1", 20, 0 },
    { "MHOWDY1", 20, 0 },
    { "MRISE1", 20, 0 },
    { "MHUH1", 20, 0 },
    { "MHEAR1", 20, 0 },
    { "MLAFF1", 20, 0 },
    { "MBOSS1", 20, 0 },
    { "MYEEHAW1", 20, 0 },
    { "MHOTDIG1", 20, 0 },
    { "MWRENCH1", 20, 0 },
    { "JBURN1", 20, 0 },
    { "JCHRGE1", 20, 0 },
    { "JCRISP1", 20, 0 },
    { "JDANCE1", 20, 0 },
    { "JJUICE1", 20, 0 },
    { "JJUMP1", 20, 0 },
    { "JLIGHT1", 20, 0 },
    { "JPOWER1", 20, 0 },
    { "JSHOCK1", 20, 0 },
    { "JYES1", 20, 0 },
    { "CHROTNK1", 20, 0 },
    { "FIXIT1", 20, 0 },
    { "MADCHRG2", 20, 0 },
    { "MADEXPLO", 20, 0 },
    { "SHKTROP1", 20, 0 } };

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
