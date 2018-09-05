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
#include "ccfileclass.h"
#include "ccini.h"
#include "globals.h"
#include "mixfile.h"
#include "picture.h"
#include "pk.h"
#include "ramfile.h"
#include "scenario.h"
#include "session.h"
#include "textprint.h"
#include "theme.h"
#include <cstdlib>
#include <ctime>

#ifndef PLATFORM_WINDOWS
#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace {
// These pointers are used only within this translation unit for reinitialisation after CD changes and such.
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
} // namespace

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

    if (GeneralMix == nullptr) {
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

/**
 * Reinitialises secondary mix files that aren't cached, such as after a CD change has taken place.
 */
void Reinit_Secondary_Mixfiles()
{
    if (MoviesMix != nullptr) {
        delete MoviesMix;
    }

    if (GeneralMix != nullptr) {
        delete GeneralMix;
    }

    if (ScoreMix != nullptr) {
        delete ScoreMix;
    }

    if (MainMix != nullptr) {
        delete MainMix;
    }

    MainMix = new MixFileClass<CCFileClass>("main.mix", &g_publicKey); // In RA main.mix contains the others.

    CCFileClass movies1("movies1.mix");

    if (movies1.Is_Available()) {
        MoviesMix = new MixFileClass<CCFileClass>("movies1.mix", &g_publicKey);
    } else {
        MoviesMix = new MixFileClass<CCFileClass>("movies2.mix", &g_publicKey);
    }

    GeneralMix = new MixFileClass<CCFileClass>("general.mix", &g_publicKey);
    ScoreMix = new MixFileClass<CCFileClass>("scores.mix", &g_publicKey);
    ThemeClass::Scan();
}

/**
 * Initialises the RSA keys used to decrypt mix file headers.
 *
 * 0x004F8664
 */
void Init_Keys()
{
    // Hardcode keys and read from RAM file.
    static char Keys[] =
        "[PublicKey]\n1=AihRvNoIbTn85FZRYNZRcT+i6KpU+maCsEqr3Q5q+LDB5tH7Tz2qQ38V\n\n"
        "[PrivateKey]\n1=AigKVje8mROcR8QixnxUEF5b29Curkq01DNDWCdOG99XBqH79OaCiTCB\n\n";
    RAMFileClass mem_keys(Keys, strlen(Keys));
    CCINIClass tempini;
    tempini.Load(mem_keys);
    g_publicKey = tempini.Get_PKey(true);
    g_privateKey = tempini.Get_PKey(false); // Private key is only used to write mix files.
}

/**
 * Initialises pointers to bitmap font file data.
 *
 * 0x004F7948
 */
void Init_Fonts()
{
    Metal12FontPtr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("12metfnt.fnt"));
    MapFontPtr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("help.fnt"));
    Font6Ptr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("6point.fnt"));
    GradFont6Ptr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("grad6fnt.fnt"));
    EditorFont = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("editfnt.fnt"));
    Font8Ptr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("8point.fnt"));
    Font3Ptr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("3point.fnt"));
    ScoreFontPtr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("scorefnt.fnt"));
    FontLEDPtr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("led.fnt"));
    VCRFontPtr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("vcr.fnt"));
    TypeFontPtr = static_cast<char *>(MixFileClass<CCFileClass>::Retrieve("8point.fnt"));
    Set_Font(Font8Ptr);
}

/**
 * Initialises random data generator and random number generator that is synced between clients.
 *
 * 0x004F5EC4
 */
void Init_Random()
{
#ifdef PLATFORM_WINDOWS
    struct _SYSTEMTIME sys_time;
    GetSystemTime(&sys_time);
    g_cryptRandom.Seed_Byte(sys_time.wMilliseconds);
    g_cryptRandom.Seed_Bit(sys_time.wSecond);
    g_cryptRandom.Seed_Bit(sys_time.wSecond >> 1);
    g_cryptRandom.Seed_Bit(sys_time.wSecond >> 2);
    g_cryptRandom.Seed_Bit(sys_time.wSecond >> 3);
    g_cryptRandom.Seed_Bit(sys_time.wSecond >> 4);
    g_cryptRandom.Seed_Bit(sys_time.wMinute);
    g_cryptRandom.Seed_Bit(sys_time.wMinute >> 1);
    g_cryptRandom.Seed_Bit(sys_time.wMinute >> 2);
    g_cryptRandom.Seed_Bit(sys_time.wMinute >> 3);
    g_cryptRandom.Seed_Bit(sys_time.wMinute >> 4);
    g_cryptRandom.Seed_Bit(sys_time.wHour);
    g_cryptRandom.Seed_Bit(sys_time.wDay);
    g_cryptRandom.Seed_Bit(sys_time.wDayOfWeek);
    g_cryptRandom.Seed_Bit(sys_time.wMonth);
    g_cryptRandom.Seed_Bit(sys_time.wYear);
#else
    struct tm *sys_time;
    struct timeval curr_time;
    gettimeofday(&curr_time, nullptr);
    sys_time = localtime(&curr_time.tv_sec);

    CryptRandom.Seed_Byte(curr_time.tv_usec / 1000);
    CryptRandom.Seed_Bit(sys_time->tm_sec);
    CryptRandom.Seed_Bit(sys_time->tm_sec >> 1);
    CryptRandom.Seed_Bit(sys_time->tm_sec >> 2);
    CryptRandom.Seed_Bit(sys_time->tm_sec >> 3);
    CryptRandom.Seed_Bit(sys_time->tm_sec >> 4);
    CryptRandom.Seed_Bit(sys_time->tm_min);
    CryptRandom.Seed_Bit(sys_time->tm_min >> 1);
    CryptRandom.Seed_Bit(sys_time->tm_min >> 2);
    CryptRandom.Seed_Bit(sys_time->tm_min >> 3);
    CryptRandom.Seed_Bit(sys_time->tm_min >> 4);
    CryptRandom.Seed_Bit(sys_time->tm_hour);
    CryptRandom.Seed_Bit(sys_time->tm_mday);
    CryptRandom.Seed_Bit(sys_time->tm_wday);
    CryptRandom.Seed_Bit(sys_time->tm_mon);
    CryptRandom.Seed_Bit(sys_time->tm_year);
#endif

    if (!Session.Loading_Game()) {
        if (!Session.Playback_Game()) {
            if (Session.Game_To_Play() == GAME_CAMPAIGN || Session.Game_To_Play() == GAME_SKIRMISH) {
                if (CustomSeed) {
                    g_seed = CustomSeed;
                } else {
                    std::srand(std::time(nullptr));
                    g_seed = std::rand();
                }
            }
        }

        RandNumb = g_seed;
        Scen.Set_Random_Seed(g_seed);
    }
}

/**
 * Initialises remap tables used for house colors and gadget drawing.
 *
 * 0x004F6240
 */
void Init_Color_Remaps()
{
    g_sysMemPage.Clear();
    Load_Picture("palette.cps", *g_sysMemPage.Get_GBuffer(), *g_sysMemPage.Get_GBuffer(), nullptr);

    // General Remaps
    for (int remap = REMAP_FIRST; remap < REMAP_COUNT; ++remap) {
        // Set a default palette mapping
        for (int j = 0; j < 256; ++j) {
            ColorRemaps[remap].RemapPalette[j] = j;
        }

        // Replace remap region with remapped indexes
        for (int j = 0; j < 16; ++j) {
            ColorRemaps[remap].RemapPalette[g_sysMemPage.Get_Pixel(j, 0)] = g_sysMemPage.Get_Pixel(j, remap);
        }

        // Write font gradient
        for (int j = 0; j < 6; ++j) {
            ColorRemaps[remap].FontPalette[j + 10] = g_sysMemPage.Get_Pixel(j + 2, remap);
        }

        ColorRemaps[remap].BrightColor = COLOR_WHITE; // Index 15 is always white in the palette
        ColorRemaps[remap].MediumColor = g_sysMemPage.Get_Pixel(4, remap);
        ColorRemaps[remap].WindowPalette[0] = g_sysMemPage.Get_Pixel(10, remap);
        ColorRemaps[remap].WindowPalette[1] = g_sysMemPage.Get_Pixel(9, remap);
        ColorRemaps[remap].WindowPalette[2] = g_sysMemPage.Get_Pixel(7, remap);
        ColorRemaps[remap].WindowPalette[3] = g_sysMemPage.Get_Pixel(4, remap);
        ColorRemaps[remap].WindowPalette[4] = g_sysMemPage.Get_Pixel(4, remap);
        ColorRemaps[remap].WindowPalette[5] = g_sysMemPage.Get_Pixel(0, remap);
        ColorRemaps[remap].WindowPalette[6] = g_sysMemPage.Get_Pixel(0, remap);
        ColorRemaps[remap].WindowPalette[7] = g_sysMemPage.Get_Pixel(6, remap);
    }

    // Grey Scheme
    // Set a default palette mapping for GreyScheme
    for (int i = 0; i < 256; ++i) {
        GreyScheme.RemapPalette[i] = i;
    }

    // Write font gradient
    for (int i = 0; i < 6; ++i) {
        GreyScheme.FontPalette[i + 10] = g_sysMemPage.Get_Pixel(i + 9, 5);
    }

    GreyScheme.BrightColor = g_sysMemPage.Get_Pixel(3, 5);
    GreyScheme.MediumColor = g_sysMemPage.Get_Pixel(7, 5);
    GreyScheme.WindowPalette[0] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(15, 5)];
    GreyScheme.WindowPalette[1] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(14, 5)];
    GreyScheme.WindowPalette[2] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(13, 5)];
    GreyScheme.WindowPalette[3] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(9, 5)];
    GreyScheme.WindowPalette[4] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(11, 5)];
    GreyScheme.WindowPalette[5] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(5, 5)];
    GreyScheme.WindowPalette[6] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(5, 5)];
    GreyScheme.WindowPalette[7] = ColorRemaps[REMAP_5].RemapPalette[g_sysMemPage.Get_Pixel(11, 5)];

    // Metal Scheme
    memset(&MetalScheme, 4, sizeof(RemapControlType));

    for (int i = 0; i < 16; ++i) {
        MetalScheme.FontPalette[i] = i;
    }

    MetalScheme.FontPalette[1] = 128;
    MetalScheme.FontPalette[2] = COLOR_BLACK;
    MetalScheme.FontPalette[4] = COLOR_LTGREY;
    MetalScheme.MediumColor = 128;
    MetalScheme.WindowPalette[1] = COLOR_TBLACK;
    MetalScheme.WindowPalette[6] = 128;
    MetalScheme.FontPalette[3] = COLOR_GREY;

    // REMAP_8 adjustments
    for (int i = 0; i < 16; ++i) {
        ColorRemaps[REMAP_8].FontPalette[i] = g_sysMemPage.Get_Pixel(i, 8);
    }

    ColorRemaps[REMAP_8].WindowPalette[0] = COLOR_BLUE;
    ColorRemaps[REMAP_8].WindowPalette[3] = COLOR_LTCYAN;
    ColorRemaps[REMAP_8].WindowPalette[5] = COLOR_WHITE;
    ColorRemaps[REMAP_8].BrightColor = COLOR_WHITE;
    ColorRemaps[REMAP_8].MediumColor = COLOR_LTCYAN;
    ColorRemaps[REMAP_8].WindowPalette[1] = COLOR_LTBLUE;
    ColorRemaps[REMAP_8].WindowPalette[2] = COLOR_LTBLUE;
    ColorRemaps[REMAP_8].WindowPalette[6] = COLOR_BLUE;
    ColorRemaps[REMAP_8].WindowPalette[7] = COLOR_BLUE;
    ColorRemaps[REMAP_8].WindowPalette[4] = COLOR_LTBLUE;

    // Set gadget remap type.
    GadgetClass::Set_Color_Scheme(&ColorRemaps[REMAP_10]);
}
