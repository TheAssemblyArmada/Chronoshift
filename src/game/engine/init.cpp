/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Engine initialisation functions.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "init.h"
#include "globals.h"
#include "pk.h"
#include "theme.h"

#ifndef PLATFORM_WINDOWS
#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifndef CHRONOSHIFT_STANDALONE
MixFileClass<CCFileClass> *&MainMix = Make_Global<MixFileClass<CCFileClass> *>(0x00668180);
MixFileClass<CCFileClass> *&ConquerMix = Make_Global<MixFileClass<CCFileClass> *>(0x00668184);
MixFileClass<CCFileClass> *&GeneralMix = Make_Global<MixFileClass<CCFileClass> *>(0x00668178);
MixFileClass<CCFileClass> *&MoviesMix = Make_Global<MixFileClass<CCFileClass> *>(0x00668174);
MixFileClass<CCFileClass> *&ScoreMix = Make_Global<MixFileClass<CCFileClass> *>(0x0066817C);
#else
MixFileClass<CCFileClass> *MainMix;
MixFileClass<CCFileClass> *ConquerMix;
MixFileClass<CCFileClass> *GeneralMix;
MixFileClass<CCFileClass> *MoviesMix;
MixFileClass<CCFileClass> *ScoreMix;
#endif

/**
 * Initialises mix files named using wildcard expansion scheme sc*.mix and ss*.mix.
 *
 * 0x004F781C
 */
void Init_Expansion_Files()
{
#ifdef PLATFORM_WINDOWS
    WIN32_FIND_DATA find;
    HANDLE hndl = FindFirstFileA("sc*.mix", &find);

    // Find mix files starting with sc and cache them, original would catch scores.mix too, fixed here.
    if (hndl != INVALID_HANDLE_VALUE) {
        do {
            if (strcasecmp(find.cFileName, "scores.mix") != 0) {
                new MixFileClass<CCFileClass>(find.cFileName, &g_publicKey);
                MixFileClass<CCFileClass>::Cache(find.cFileName);
            }
        } while (FindNextFileA(hndl, &find));
    }

    hndl = FindFirstFileA("ss*.mix", &find);

    // Find mix files starting with sc and do not cache them. Largely only useful for wsa and vqa files.
    if (hndl != INVALID_HANDLE_VALUE) {
        do {
            new MixFileClass<CCFileClass>(find.cFileName, &g_publicKey);
        } while (FindNextFileA(hndl, &find));
    }
#else // Assuming posix dirent here
    DIR *dp;
    struct dirent *dirp;
    struct stat st;

    // Find mix files starting with sc and cache them, original would catch scores.mix too, fixed here.
    if ((dp = opendir("./")) != nullptr) {
        while ((dirp = readdir(dp)) != nullptr) {
            stat(dirp->d_name, &st);

            // Make sure we don't have a folder and name matches
            if (!S_ISDIR(st.st_mode) && fnmatch("sc*.mix", dirp->d_name, FNM_PATHNAME | FNM_CASEFOLD) == 0
                && strcasecmp(dirp->d_name, "scores.mix") != 0) {
                fileptr = new MixFileClass<CCFileClass>(dirp->d_name, &PublicKey);
                MixFileClass<CCFileClass>::Cache(dirp->d_name);
            }
        }
    }
    closedir(dp);

    // Find mix files starting with sc and do not cache them. Largely only useful for wsa and vqa files.
    if ((dp = opendir("./")) != nullptr) {
        while ((dirp = readdir(dp)) != nullptr) {
            stat(dirp->d_name, &st);

            // Make sure we don't have a folder and name matches
            if (!S_ISDIR(st.st_mode) && !fnmatch("ss*.mix", dirp->d_name, FNM_PATHNAME | FNM_CASEFOLD)) {
                new MixFileClass<CCFileClass>(dirp->d_name, &PublicKey);
            }
        }
    }
    closedir(dp);
#endif
}

/**
 * Initialises local mix files.
 *
 * 0x004F7DF0
 */
void Init_Bootstrap_Mixfiles()
{
    int reqcd = g_requiredCD;
    // We are currently reading local files
    g_requiredCD = -2;

    CCFileClass wolapi("wolapi.mix");
    if (wolapi.Is_Available()) {
        new MixFileClass<CCFileClass>("wolapi.mix", &g_publicKey);
        MixFileClass<CCFileClass>::Cache("wolapi.mix");
    }

    CCFileClass expand2("expand2.mix");
    if (expand2.Is_Available()) {
        new MixFileClass<CCFileClass>("expand2.mix", &g_publicKey);
        MixFileClass<CCFileClass>::Cache("expand2.mix");
    }

    new MixFileClass<CCFileClass>("hires1.mix", &g_publicKey);
    MixFileClass<CCFileClass>::Cache("hires1.mix");

    CCFileClass expand("expand.mix");
    if (expand.Is_Available()) {
        new MixFileClass<CCFileClass>("expand.mix", &g_publicKey);
        MixFileClass<CCFileClass>::Cache("expand.mix");
    }

    new MixFileClass<CCFileClass>("redalert.mix", &g_publicKey);

    new MixFileClass<CCFileClass>("local.mix", &g_publicKey);
    MixFileClass<CCFileClass>::Cache("local.mix");

    new MixFileClass<CCFileClass>("hires.mix", &g_publicKey);
    MixFileClass<CCFileClass>::Cache("hires.mix");

    new MixFileClass<CCFileClass>("nchires.mix", &g_publicKey);

    g_requiredCD = reqcd;
}

/**
 * Initialises mix files that were not copied to the hard drive during an original install and were used from CD instead.
 *
 * 0x004F8018
 */
void Init_Secondary_Mixfiles()
{
    MainMix = new MixFileClass<CCFileClass>("main.mix", &g_publicKey); // In RA main.mix contains the others.
    ConquerMix = new MixFileClass<CCFileClass>("conquer.mix", &g_publicKey);

    if (GeneralMix != nullptr) {
        GeneralMix = new MixFileClass<CCFileClass>("general.mix", &g_publicKey);
    }

    CCFileClass movies1("movies1.mix");

    if (movies1.Is_Available()) {
        MoviesMix = new MixFileClass<CCFileClass>("movies1.mix", &g_publicKey);
    } else {
        MoviesMix = new MixFileClass<CCFileClass>("movies2.mix", &g_publicKey);
    }

    ScoreMix = new MixFileClass<CCFileClass>("scores.mix", &g_publicKey);

    if (ScoreMix != nullptr) {
        ScoresPresent = true;
        ThemeClass::Scan();
    }

    new MixFileClass<CCFileClass>("speech.mix", &g_publicKey);
    new MixFileClass<CCFileClass>("sounds.mix", &g_publicKey);
    new MixFileClass<CCFileClass>("russian.mix", &g_publicKey);
    new MixFileClass<CCFileClass>("allies.mix", &g_publicKey);
}
