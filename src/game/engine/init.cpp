/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Engine initialisation functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "init.h"
#include "callback.h"
#include "gadget.h"
#include "gamefile.h"
#include "gameini.h"
#include "gbuffer.h"
#include "globals.h"
#include "iomap.h"
#include "keyboard.h"
#include "language.h"
#include "mixfile.h"
#include "mouse.h"
#include "mouseshape.h"
#include "msgbox.h"
#include "palette.h"
#include "pcx.h"
#include "picture.h"
#include "pk.h"
#include "ramfile.h"
#include "scenario.h"
#include "session.h"
#include "shape.h"
#include "surfacemonitor.h"
#include "textprint.h"
#include "theme.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif

#ifdef HAVE_FNMATCH_H
#include <fnmatch.h>
#endif

#define SHAPE_BUFF_SIZE 65000

using std::sprintf;


// These pointers are used only within this translation unit for reinitialisation after CD changes and such.
#ifdef GAME_DLL
extern GameMixFile *&MainMix;
extern GameMixFile *&ConquerMix;
extern GameMixFile *&GeneralMix;
extern GameMixFile *&MoviesMix;
extern GameMixFile *&ScoreMix;
#else
namespace
{
GameMixFile *MainMix;
GameMixFile *ConquerMix;
GameMixFile *GeneralMix;
GameMixFile *MoviesMix;
GameMixFile *ScoreMix;
} // namespace
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
                new GameMixFile(find.cFileName, &g_publicKey);
                GameFileClass::Cache_Mix(find.cFileName);
            }
        } while (FindNextFileA(hndl, &find));
    }

    hndl = FindFirstFileA("ss*.mix", &find);

    // Find mix files starting with sc and do not cache them. Largely only useful for wsa and vqa files.
    if (hndl != INVALID_HANDLE_VALUE) {
        do {
            new GameMixFile(find.cFileName, &g_publicKey);
        } while (FindNextFileA(hndl, &find));
    }
#elif defined HAVE_DIRENT_H && defined HAVE_FNMATCH_H
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
                new GameMixFile(dirp->d_name, &g_publicKey);
                GameFileClass::Cache_Mix(dirp->d_name);
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
                new GameMixFile(dirp->d_name, &g_publicKey);
            }
        }
    }
    closedir(dp);
#else
#error Suitable directory enumeration functions not found.
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

    GameFileClass wolapi("wolapi.mix");
    if (wolapi.Is_Available()) {
        new GameMixFile("wolapi.mix", &g_publicKey);
        GameFileClass::Cache_Mix("wolapi.mix");
    }

    GameFileClass expand2("expand2.mix");
    if (expand2.Is_Available()) {
        new GameMixFile("expand2.mix", &g_publicKey);
        GameFileClass::Cache_Mix("expand2.mix");
    }

    new GameMixFile("hires1.mix", &g_publicKey);
    GameFileClass::Cache_Mix("hires1.mix");

    GameFileClass expand("expand.mix");
    if (expand.Is_Available()) {
        new GameMixFile("expand.mix", &g_publicKey);
        GameFileClass::Cache_Mix("expand.mix");
    }

    new GameMixFile("redalert.mix", &g_publicKey);

    new GameMixFile("local.mix", &g_publicKey);
    GameFileClass::Cache_Mix("local.mix");

    new GameMixFile("editor.mix", &g_publicKey);
    GameFileClass::Cache_Mix("editor.mix");

    new GameMixFile("hires.mix", &g_publicKey);
    GameFileClass::Cache_Mix("hires.mix");

    new GameMixFile("nchires.mix", &g_publicKey);

    g_requiredCD = reqcd;
}

/**
 * Initialises mix files that were not copied to the hard drive during an original install and were used from CD instead.
 *
 * 0x004F8018
 */
void Init_Secondary_Mixfiles()
{
    MainMix = new GameMixFile("main.mix", &g_publicKey); // In RA main.mix contains the others.

    new GameMixFile("edhi.mix", &g_publicKey);
    GameFileClass::Cache_Mix("edhi.mix");

    ConquerMix = new GameMixFile("conquer.mix", &g_publicKey);

    if (GeneralMix == nullptr) {
        GeneralMix = new GameMixFile("general.mix", &g_publicKey);
    }

    GameFileClass movies1("movies1.mix");

    if (movies1.Is_Available()) {
        MoviesMix = new GameMixFile("movies1.mix", &g_publicKey);
    } else {
        MoviesMix = new GameMixFile("movies2.mix", &g_publicKey);
    }

    ScoreMix = new GameMixFile("scores.mix", &g_publicKey);

    if (ScoreMix != nullptr) {
        ScoresPresent = true;
        ThemeClass::Scan();
    }

    new GameMixFile("speech.mix", &g_publicKey);
    new GameMixFile("sounds.mix", &g_publicKey);
    new GameMixFile("russian.mix", &g_publicKey);
    new GameMixFile("allies.mix", &g_publicKey);
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

    MainMix = new GameMixFile("main.mix", &g_publicKey); // In RA main.mix contains the others.

    GameFileClass movies1("movies1.mix");

    if (movies1.Is_Available()) {
        MoviesMix = new GameMixFile("movies1.mix", &g_publicKey);
    } else {
        MoviesMix = new GameMixFile("movies2.mix", &g_publicKey);
    }

    GeneralMix = new GameMixFile("general.mix", &g_publicKey);
    ScoreMix = new GameMixFile("scores.mix", &g_publicKey);
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
    GameINIClass tempini;
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
    Metal12FontPtr = static_cast<char *>(GameFileClass::Retrieve("12metfnt.fnt"));
    MapFontPtr = static_cast<char *>(GameFileClass::Retrieve("help.fnt"));
    Font6Ptr = static_cast<char *>(GameFileClass::Retrieve("6point.fnt"));
    GradFont6Ptr = static_cast<char *>(GameFileClass::Retrieve("grad6fnt.fnt"));
    EditorFont = static_cast<char *>(GameFileClass::Retrieve("editfnt.fnt"));
    Font8Ptr = static_cast<char *>(GameFileClass::Retrieve("8point.fnt"));
    Font3Ptr = static_cast<char *>(GameFileClass::Retrieve("3point.fnt"));
    ScoreFontPtr = static_cast<char *>(GameFileClass::Retrieve("scorefnt.fnt"));
    FontLEDPtr = static_cast<char *>(GameFileClass::Retrieve("led.fnt"));
    VCRFontPtr = static_cast<char *>(GameFileClass::Retrieve("vcr.fnt"));
    TypeFontPtr = static_cast<char *>(GameFileClass::Retrieve("8point.fnt"));
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
#elif defined HAVE_SYS_TIME_H
    struct tm *sys_time;
    struct timeval curr_time;
    gettimeofday(&curr_time, nullptr);
    sys_time = localtime(&curr_time.tv_sec);

    g_cryptRandom.Seed_Byte(curr_time.tv_usec / 1000);
    g_cryptRandom.Seed_Bit(sys_time->tm_sec);
    g_cryptRandom.Seed_Bit(sys_time->tm_sec >> 1);
    g_cryptRandom.Seed_Bit(sys_time->tm_sec >> 2);
    g_cryptRandom.Seed_Bit(sys_time->tm_sec >> 3);
    g_cryptRandom.Seed_Bit(sys_time->tm_sec >> 4);
    g_cryptRandom.Seed_Bit(sys_time->tm_min);
    g_cryptRandom.Seed_Bit(sys_time->tm_min >> 1);
    g_cryptRandom.Seed_Bit(sys_time->tm_min >> 2);
    g_cryptRandom.Seed_Bit(sys_time->tm_min >> 3);
    g_cryptRandom.Seed_Bit(sys_time->tm_min >> 4);
    g_cryptRandom.Seed_Bit(sys_time->tm_hour);
    g_cryptRandom.Seed_Bit(sys_time->tm_mday);
    g_cryptRandom.Seed_Bit(sys_time->tm_wday);
    g_cryptRandom.Seed_Bit(sys_time->tm_mon);
    g_cryptRandom.Seed_Bit(sys_time->tm_year);
#else
#error Suitable time functions not found.
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

    // White Scheme
    for (int i = 0; i < 256; ++i ) {
        WhiteScheme.RemapPalette[i] = i;
    }

    // Write small gradient.
    for (int i = 4; i < 10; ++i) {
        WhiteScheme.FontPalette[i] = g_sysMemPage.Get_Pixel(1, 10);
    }

    WhiteScheme.BrightColor = g_sysMemPage.Get_Pixel(1, 10);
    WhiteScheme.MediumColor = g_sysMemPage.Get_Pixel(1, 10);

    for (int i = 0; i < 8; ++i) {
        WhiteScheme.WindowPalette[i] = g_sysMemPage.Get_Pixel(1, 10);
    }

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

/**
 * Initialises the data needed for the mouse cursor and sets the default state.
 *
 * 0x004F8390
 */
void Init_Mouse()
{
   if (!MouseInstalled) {
        char buff[256];
        GamePalette.Set();
        sprintf(buff, "Chronoshift is unable to detect your mouse driver.");
        g_visiblePage.Clear();
        MessageBoxClass box;
        box.Process(buff);
        Emergency_Exit(1);
    }

#ifdef PLATFORM_WINDOWS
   ShowCursor(FALSE); // Hides the OS GUI cursor.
#endif
 
    void *mouse_shp = MixFileClass<GameFileClass>::Retrieve("mouse.shp");
    void *edmouse_shp = MixFileClass<GameFileClass>::Retrieve("edmouse.shp");

    if (mouse_shp != nullptr) {
        g_mouse->Set_Cursor(0, 0, Extract_Shape(mouse_shp));

        while (g_mouse->Get_Mouse_State() > 1) {
            g_mouse->Show_Mouse();
        }
    }

    Map.Set_Default_Mouse(MOUSE_POINTER);

    do {
        g_mouse->Show_Mouse();
    } while (g_mouse->Get_Mouse_State() > 1);

    Call_Back();
    g_mouse->Hide_Mouse();
}

/**
 * Bootstraps a bunch of data needed by later code.
 *
 * 0x004F81CC
 */
void Bootstrap()
{
    static RemapControlType _sidebar_scheme;

    BlackPalette.Set();

    if (CDFileClass::Has_Paths()) {
        g_requiredCD = DISK_ANY;
    }

    // Call event handler until we are in focus.
    do {
        g_keyboard->Check();
    } while (!g_gameInFocus);

    g_allSurfaces.Clear_Surfaces_Restored();
    // Mono_Clear_Screen();
    Init_Bootstrap_Mixfiles();
    Init_Fonts();
    g_keyboard->Clear();
    Set_Shape_Buffer(new uint8_t[SHAPE_BUFF_SIZE], SHAPE_BUFF_SIZE);
    Init_Language(); // Original just assigned the pointers here, didn't have separate function.
    memmove(&GamePalette, GameFileClass::Retrieve("temperat.pal"), sizeof(GamePalette));

    if (&WhitePalette != &BlackPalette) {
        WhitePalette[0] = BlackPalette[0];
    }

    Init_Expansion_Files();

    // Prep a gadget color scheme.
    _sidebar_scheme.WindowPalette[0] = 13;
    _sidebar_scheme.WindowPalette[1] = 12;
    _sidebar_scheme.WindowPalette[2] = 14;
    _sidebar_scheme.WindowPalette[3] = 15;
    _sidebar_scheme.WindowPalette[4] = 14;
    _sidebar_scheme.WindowPalette[5] = 15;
    _sidebar_scheme.BrightColor = 15;
    _sidebar_scheme.MediumColor = 14;
    GadgetClass::Set_Color_Scheme(&_sidebar_scheme);
}

/**
 * Performs checks for a CD drive and if an game CD is present.
 *
 * 0x004F7A08
 */
void Init_CDROM_Access()
{
    g_visiblePage.Clear();
    g_hidPage.Clear();

    if (CDFileClass::Has_Paths()) {
        g_requiredCD = DISK_ANY;
    } else {
        int result;
        MessageBoxClass msg;
        Force_CD_Available(DISK_CDCHECK);

        do {
            result = CDFileClass::Set_Search_Drives("?:\\");

            switch (result) {
                default: // Fallthrough
                case 0: // Set drives succeeded.
                    g_visiblePage.Clear();
                    g_mouse->Show_Mouse();

                    if (!Force_CD_Available(g_requiredCD)) {
                        Emergency_Exit(0xFF);
                    }

                    g_mouse->Hide_Mouse();

                    break;
                case 1: // Set drives failed with no CD drive found.
                    g_visiblePage.Clear();
                    GamePalette.Set();
                    g_mouse->Show_Mouse();
                    msg.Process(TXT_CD_DIALOG_ERROR1);
                    Emergency_Exit(0xFF);
                    break;
                case 2: // Set drives failed with CD not being RA disk.
                    g_visiblePage.Clear();
                    GamePalette.Set();
                    g_mouse->Show_Mouse();

                    if (msg.Process(TXT_CD_DIALOG_1, TXT_OK, TXT_CANCEL) == 1) {
                        Emergency_Exit(0xFF);
                    }

                    g_mouse->Hide_Mouse();
                    break;
            }

        } while (result != 0);

        g_requiredCD = DISK_ANY;
    }
}

/**
 * Loads a PCX file data into the provided viewport and palette.
 *
 * 0x005B3CD8
 */
void Load_Title_Screen(const char *filename, GraphicViewPortClass *vp, PaletteClass *pal)
{
    GraphicBufferClass *buff = Read_PCX_File(filename, pal);

    if (buff == nullptr) {
        return;
    }

    buff->Blit(*vp);
    delete buff;
}

/**
 * Loads and displays the prolog page.
 *
 * 0x004F3E98
 */
void Load_Prolog_Page()
{
    g_mouse->Hide_Mouse();
    Load_Title_Screen("prolog.pcx", &g_hidPage, &CCPalette);
    g_hidPage.Blit(g_seenBuff);
    CCPalette.Set();
    g_mouse->Show_Mouse();
}
