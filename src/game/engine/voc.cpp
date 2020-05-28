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
#include "coord.h"
#include "gamefile.h"
#include "gameoptions.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

// clang-format off
SoundEffectType g_SoundEffectName[] = {
    { "girlokay", 20, VAR_NONE },
    { "girlyeah", 20, VAR_NONE },
    { "guyokay1", 20, VAR_NONE },
    { "guyyeah1", 20, VAR_NONE },
    { "minelay1", 5, VAR_NONE }, // BUGFIX: Changed from HOUSE to NONE, as it doesnt have a variation file.
    { "ackno", 20, VAR_HOUSE },
    { "affirm1", 20, VAR_HOUSE },
    { "await1", 20, VAR_HOUSE },
    { "eaffirm1", 20, VAR_NONE },
    { "eengin1", 20, VAR_NONE },
    { "noprob", 20, VAR_HOUSE },
    { "ready", 20, VAR_HOUSE },
    { "report1", 20, VAR_HOUSE },
    { "ritaway", 20, VAR_HOUSE },
    { "roger", 20, VAR_HOUSE },
    { "ugotit", 20, VAR_HOUSE },
    { "vehic1", 20, VAR_HOUSE },
    { "yessir1", 20, VAR_HOUSE },
    { "dedman1", 10, VAR_NONE },
    { "dedman2", 10, VAR_NONE },
    { "dedman3", 10, VAR_NONE },
    { "dedman4", 10, VAR_NONE },
    { "dedman5", 10, VAR_NONE },
    { "dedman6", 10, VAR_NONE },
    { "dedman7", 10, VAR_NONE },
    { "dedman8", 10, VAR_NONE },
    { "dedman10", 10, VAR_NONE },
    { "chrono2", 5, VAR_NONE },
    { "cannon1", 1, VAR_NONE },
    { "cannon2", 1, VAR_NONE },
    { "ironcur9", 10, VAR_NONE },
    { "emovout1", 20, VAR_NONE },
    { "sonpulse", 10, VAR_NONE },
    { "sandbag2", 5, VAR_NONE },
    { "mineblo1", 5, VAR_NONE },
    { "chute1", 1, VAR_NONE },
    { "dogy1", 5, VAR_NONE },
    { "dogw5", 10, VAR_NONE },
    { "dogg5p", 10, VAR_NONE },
    { "firebl3", 1, VAR_NONE },
    { "firetrt1", 1, VAR_NONE },
    { "grenade1", 1, VAR_NONE },
    { "gun11", 1, VAR_NONE },
    { "gun13", 1, VAR_NONE },
    { "eyessir1", 1, VAR_NONE },
    { "gun27", 1, VAR_NONE },
    { "heal2", 1, VAR_NONE },
    { "hydrod1", 1, VAR_NONE },
    { "invul2", 1, VAR_NONE },
    { "kaboom1", 1, VAR_NONE },
    { "kaboom12", 1, VAR_NONE },
    { "kaboom15", 1, VAR_NONE },
    { "splash9", 5, VAR_NONE },
    { "kaboom22", 5, VAR_NONE },
    { "aacanon3", 1, VAR_NONE },
    { "tandeth1", 1, VAR_NONE },
    { "mguninf1", 1, VAR_NONE },
    { "missile1", 1, VAR_NONE },
    { "missile6", 1, VAR_NONE },
    { "missile7", 1, VAR_NONE },
    { "oretruk1", 1, VAR_NONE },
    { "pillbox1", 1, VAR_NONE },
    { "rabeep1", 1, VAR_NONE },
    { "ramenu1", 1, VAR_NONE },
    { "silencer", 1, VAR_NONE },
    { "tank5", 1, VAR_NONE },
    { "tank6", 1, VAR_NONE },
    { "torpedo1", 1, VAR_NONE },
    { "turret1", 1, VAR_NONE },
    { "tslachg2", 10, VAR_NONE },
    { "tesla1", 10, VAR_NONE },
    { "squishy2", 10, VAR_NONE },
    { "scoldy1", 10, VAR_NONE },
    { "radaron2", 10, VAR_NONE },
    { "radardn1", 10, VAR_NONE },
    { "placbldg", 10, VAR_NONE },
    { "kaboom30", 10, VAR_NONE },
    { "kaboom25", 10, VAR_NONE },
    { "kaboom21", 10, VAR_NONE },
    { "dogw7", 10, VAR_NONE },
    { "dogw3px", 10, VAR_NONE },
    { "crmble2", 10, VAR_NONE },
    { "cashup1", 10, VAR_NONE },
    { "cashdn1", 10, VAR_NONE },
    { "build5", 10, VAR_NONE },
    { "bleep9", 10, VAR_NONE },
    { "bleep6", 10, VAR_NONE },
    { "bleep5", 10, VAR_NONE },
    { "bleep17", 10, VAR_NONE },
    { "bleep13", 10, VAR_NONE },
    { "bleep12", 10, VAR_NONE },
    { "bleep11", 10, VAR_NONE },
    { "h2obomb2", 10, VAR_NONE },
    { "cashturn", 10, VAR_NONE },
    { "tuffguy1", 20, VAR_NONE },
    { "rokroll1", 20, VAR_NONE },
    { "laugh1", 20, VAR_NONE },
    { "cmon1", 20, VAR_NONE },
    { "bombit1", 20, VAR_NONE },
    { "gotit1", 20, VAR_NONE },
    { "keepem1", 20, VAR_NONE },
    { "onit1", 20, VAR_NONE },
    { "lefty1", 20, VAR_NONE },
    { "yeah1", 20, VAR_NONE },
    { "yes1", 20, VAR_NONE },
    { "yo1", 20, VAR_NONE },
    { "wallkil2", 5, VAR_NONE },
    { "mrj7", 10, VAR_NONE },
    { "gun5", 5, VAR_NONE },
    { "subshow1", 5, VAR_NONE },
    { "einah1", 20, VAR_NONE },
    { "einok1", 20, VAR_NONE },
    { "einyes1", 20, VAR_NONE },
    { "mine1", 20, VAR_NONE },
    { "scomnd1", 20, VAR_NONE },
    { "syessir1", 20, VAR_NONE },
    { "sindeed1", 20, VAR_NONE },
    { "sonway1", 20, VAR_NONE },
    { "sking1", 20, VAR_NONE },
    { "mrespon1", 20, VAR_NONE },
    { "myessir1", 20, VAR_NONE },
    { "maffirm1", 20, VAR_NONE },
    { "mmovout1", 20, VAR_NONE },
    { "beepslct", 20, VAR_NONE },
    { "syeah1", 20, VAR_NONE },
    { "antdie", 20, VAR_NONE },
    { "antbite", 20, VAR_NONE },
    { "smout1", 20, VAR_NONE },
    { "sokay1", 20, VAR_NONE },
    { "ssteal1", 20, VAR_NONE },
    { "swhat1", 20, VAR_NONE },
    { "saffirm1", 20, VAR_NONE },
    { "stavcmdr", 20, VAR_NONE },
    { "stavcrse", 20, VAR_NONE },
    { "stavyes", 20, VAR_NONE },
    { "stavmov", 20, VAR_NONE },
    { "buzzy1", 20, VAR_NONE },
    { "rambo1", 20, VAR_NONE },
    { "rambo2", 20, VAR_NONE },
    { "rambo3", 20, VAR_NONE },
    { "myes1", 20, VAR_NONE },
    { "mhowdy1", 20, VAR_NONE },
    { "mrise1", 20, VAR_NONE },
    { "mhuh1", 20, VAR_NONE },
    { "mhear1", 20, VAR_NONE },
    { "mlaff1", 20, VAR_NONE },
    { "mboss1", 20, VAR_NONE },
    { "myeehaw1", 20, VAR_NONE },
    { "mhotdig1", 20, VAR_NONE },
    { "mwrench1", 20, VAR_NONE },
    { "jburn1", 20, VAR_NONE },
    { "jchrge1", 20, VAR_NONE },
    { "jcrisp1", 20, VAR_NONE },
    { "jdance1", 20, VAR_NONE },
    { "jjuice1", 20, VAR_NONE },
    { "jjump1", 20, VAR_NONE },
    { "jlight1", 20, VAR_NONE },
    { "jpower1", 20, VAR_NONE },
    { "jshock1", 20, VAR_NONE },
    { "jyes1", 20, VAR_NONE },
    { "chrotnk1", 20, VAR_NONE },
    { "fixit1", 20, VAR_NONE },
    { "madchrg2", 20, VAR_NONE },
    { "madexplo", 20, VAR_NONE },
    { "shktrop1", 20, VAR_NONE }
};
// clang-format on

VocType Voc_From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return VOC_NONE;
    }

    if (name != nullptr) {
        for (VocType voc = VOC_FIRST; voc < VOC_COUNT; ++voc) {
            if (strcasecmp(name, g_SoundEffectName[voc].BaseName) == 0) {
                return voc;
            }
        }
    }
    return VOC_NONE;
}

const char *Name_From_Voc(VocType voc)
{
    if (voc >= VOC_FIRST && voc < VOC_COUNT) {
        return g_SoundEffectName[voc].BaseName;
    }
    return "<none>";
}

void Sound_Effect(VocType voc, coord_t location, int int2, HousesType house)
{
    if (g_Debug_Quiet || !g_SoundOn) {
        return;
    }

    if (g_Options.Get_Sound_Volume() == 0 || voc == VOC_NONE) {
        return;
    }

    short pan = PAN_CENTER;

    // Initial volume is max, 1.0.
    fixed_t volume(1, 1);

    // Do we have a location and is out side player's view?
    cell_t cell = Coord_To_Cell(location);
    if (location && !g_Map.In_View(cell)) {
        // Distance relative from top left of the current viewport location.
        int dist = Coord_Distance(location, g_Map.Get_DisplayPos());

        // Create reduction value from distance.
        fixed_t reduction(dist / CELL_LEPTONS, 192);

        // Reduce volume, with the reduction having a maximum of fixed 1.0.
        volume -= std::min<fixed_t>(reduction, 1);

        // Determine cell distance as pan, positive values left, negative values right.
        int pan_distance =
            Cell_Get_X(cell) - ((Lepton_To_Cell_Coord(g_Map.Get_DisplayWidth()) / 2) + Coord_Cell_X(g_Map.Get_DisplayPos()));

        // Is it not in screen center?
        if (abs(pan_distance) > Lepton_To_Cell_Coord(g_Map.Get_DisplayWidth() / 2)) {
            // If so calculate final pan and clamp it in case of a overflow.
            pan = std::clamp((pan_distance * 32768) / 32, PAN_RIGHT, PAN_LEFT);
        }
    }

    Sound_Effect(voc, volume, int2, pan, house);
}

int Sound_Effect(VocType voc, fixed_t volume, int int1, int16_t pan, HousesType house)
{
    if (g_Debug_Quiet || !g_SoundOn) {
        return INVALID_AUDIO_HANDLE;
    }

    if (g_Options.Get_Sound_Volume() == 0) {
        return INVALID_AUDIO_HANDLE;
    }

    if (voc == VOC_NONE) {
        return INVALID_AUDIO_HANDLE;
    }

    fixed_t vol = volume * g_Options.Get_Sound_Volume();

    char *ext = "aud";

    if (g_SoundEffectName[voc].Var == VAR_HOUSE) {
        if (house == HOUSES_NONE) {
            house = g_PlayerPtr->Acts_Like();
        }

        if ((1 << house) & SIDE_ALLIES) {
            if (int1 < 0) {
                if (abs(int1) % 2) {
                    ext = "v00";
                } else {
                    ext = "v02";
                }
            } else {
                if (int1 % 2) {
                    ext = "v01";
                } else {
                    ext = "v03";
                }
            }
        } else {
            if (int1 < 0) {
                if (abs(int1) % 2) {
                    ext = "r00";
                } else {
                    ext = "r02";
                }
            } else {
                if (int1 % 2) {
                    ext = "r01";
                } else {
                    ext = "r03";
                }
            }
        }
    }
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.%s", g_SoundEffectName[voc].BaseName, ext);
    void *file = GameFileClass::Retrieve(filename);
    if (file != nullptr) {
        // Volume maximum is fixed 1.0.
        vol = std::min<fixed_t>(vol, 1);

        return Play_Sample(file, vol * g_SoundEffectName[voc].Priority, vol * 256, pan);
    }
    return INVALID_AUDIO_HANDLE;
}
