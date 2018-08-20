/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief  Disk change handling.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cd.h"
#include "ccfileclass.h"
#include "gbuffer.h"
#include "getcd.h"
#include "globals.h"
#include "init.h"
#include "keyboard.h"
#include "language.h"
#include "mixfile.h"
#include "mouse.h"
#include "msgbox.h"
#include "palette.h"
#include "textprint.h"
#include "theme.h"
#include "timer.h"
#include <cstdio>
#include <sys/stat.h>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
int &g_requiredCD = *reinterpret_cast<int *>(0x006017D0);
int &g_currentCD = Make_Global<int>(0x006017D4);
#else
int g_requiredCD = DISK_CDCHECK;
int g_currentCD = DISK_CDCHECK;
#endif

// Was local to Force_CD_Present in orignal but now shared between split out functions.
static int g_last = -1;

/**
 * Work out which game CD we have in the drive.
 *
 * 0x004AAB2C
 */
int Get_CD_Index(int drive, int delay)
{
#ifdef PLATFORM_WINDOWS
    // TODO we don't have nor will ever have a DVD version, can probably remove that entry.
    static const char *_vol_labels[] = { "CD1", "CD2", "CD3", "CD4", "CD1" /*, "RADVD"*/ };
    static const int _num_volumes = 5;
    CountDownTimerClass timer;
    timer.Set(delay);
    char drive_letter = 'A' + drive;
    char path_buff[128];
    char vol_buff[128];
    DWORD max_comp_length = 0;
    DWORD filesys_flags = 0;
    HANDLE fh = 0;
    bool checked = false;

    while (true) {
    recheck_drive:
        // Keep trying to check the CD file system until time out to give disc time to spin up.
        while (true) {
            snprintf(path_buff, sizeof(path_buff), "%c:\\", drive_letter);

            if (GetVolumeInformationA(path_buff, vol_buff, 128, nullptr, &max_comp_length, &filesys_flags, nullptr, 0)) {
                break;
            }

            if (timer.Time() == 0 || GetLastError() != 0x15) {
                return -1;
            }
        }

        // Check if we have a main.mix in the root of the CD.
        snprintf(path_buff, sizeof(path_buff), "%c:\\main.mix", drive_letter);
        fh = CreateFileA(path_buff, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if (fh != INVALID_HANDLE_VALUE) {
            break;
        }

        if (checked) {
            return -1;
        }

        checked = true;
    }

    CloseHandle(fh);

    // Check if the drive has the correct label for a game disc.
    for (int i = 0; i < _num_volumes; ++i) {
        if (strcasecmp(vol_buff, _vol_labels[i]) == 0) {
            return i;
        }
    }

    goto recheck_drive;
#else
    // Don't even bother if its not windows.
    return -1;
#endif
}

/**
 * Checks for local folders containing data from the various discs.
 */
static BOOL Change_Local_Dir(int cd)
{
    static bool _initialised = false;
    static unsigned _detected = 0;
    static const char *_vol_labels[] = { "cd1", "cd2", "cd3", "cd4" };

    //DEBUG_LOG("Requested %d, last is %d.\n", cd, g_last);

    // Detect which if any of the discs have had their data copied to an appropriate local folder.
    if (!_initialised) {
        for (int i = 0; i < 4; ++i) {
            struct stat st;

            if (stat(_vol_labels[i], &st) == 0 && (st.st_mode & S_IFDIR) == S_IFDIR) {
                CDFileClass::Refresh_Search_Drives();
                CDFileClass::Add_Search_Drive(_vol_labels[i]);
                CCFileClass fc("main.mix");

                // Populate _detected as a bitfield for which discs we found a local copy of.
                if (fc.Is_Available()) {
                    _detected |= 1 << i;
                }
            }
        }
    }

    // No local folders with cd data dectected so we can't load any.
    if (_detected == 0) {
        return false;
    }

    // This condition just does a CD check to make sure we have at least one disc available.
    if (cd == DISK_CDCHECK) {
        if (g_last == cd) { // If g_last == DISK_CDCHECK, then we have to do a change.
            // Find highest number CD we have detected.
            for (int i = 3; i >= 0; --i) {
                if (_detected & (1 << i)) {
                    cd = i;
                    break;
                }
            }
        } else {
            return true;
        }
    }

    // This condition handles a request for either expansion disk, if we have one initialised already we are good to go.
    // Otherwise initialise to most recent expansion, aftermath.
    if (cd == DISK_EXPANSION) {
        if (g_last == DISK_AFTERMATH || g_last == DISK_COUNTERSTRIKE) {
            return true;
        } else {
            cd = DISK_AFTERMATH;
        }
    }

    // Prevent unneeded changes.
    if (g_last == cd) {
        return true;
    }

    // If the data from the CD we want was detected, then double check it and change to it.
    if (_detected & (1 << cd)) {
        DEBUG_ASSERT_PRINT(cd >= 0 && cd < 4, "Requested CD is outside expected range.\n");
        struct stat st;

        // Verify that the file is still available and hasn't been deleted out from under us.
        if (stat(_vol_labels[cd], &st) == 0 && (st.st_mode & S_IFDIR) == S_IFDIR) {
            //DEBUG_LOG("Local directory '%s' found, adding to search path and checking for main.mix.\n", _vol_labels[cd]);
            CDFileClass::Refresh_Search_Drives();
            CDFileClass::Add_Search_Drive(_vol_labels[cd]);
            CCFileClass fc("main.mix");

            if (fc.Is_Available()) {
                //DEBUG_LOG("main.mix for '%s' found in search path, reloading secondary mix files.\n", _vol_labels[cd]);
                g_last = cd;
                Reinit_Secondary_Mixfiles();

                return true;
            }
        }
    }

    return false;
}

/**
 * Changes the data loaded from the CD when the disk has been changed.
 */
static void Change_CD(int drive, int index, int &cd)
{
    g_currentCD = index;
    CDFileClass::Set_CD_Drive(drive);
    CDFileClass::Refresh_Search_Drives();

    if (cd == DISK_EXPANSION) {
        cd = DISK_AFTERMATH;
    }

    // If disk needs to change then unload existing files that are present on CD and reload from new CD.
    if (cd > DISK_CDCHECK && cd != g_last /*&& cd != DISK_DVD*/) {
        g_last = cd;
        Theme.Stop();
        Reinit_Secondary_Mixfiles();
    }
}

/**
 * Attempts to get a specific CD present and its data available to the game engine.
 *
 * 0x004AAC58
 */
BOOL Force_CD_Available(int cd)
{
    static void *_font = nullptr;
    // static int _last = -1;
    static const char *_cd_name[] = {
        // TODO hard coded strings need translating and adding to a string table
        "RED ALERT DISK 1",
        "RED ALERT DISK 2",
        "CounterStrike CD",
        "Aftermath CD" /*, "RED ALERT DVD"*/
    };

    //DEBUG_LOG("Force_CD_Available attempting to change to %d\n", cd);

    if (cd == DISK_ANY) {
        return true;
    }

    if (Change_Local_Dir(cd)) {
        //DEBUG_LOG("Force_CD_Available found data locally, returning success.\n");
        return true;
    }

    int cd_index = Get_CD_Index(CDFileClass::Current_Drive(), 60);
    int drive = 0;

    // 3.00 and above checks for DVD which was never released.
    /*if (Using_DVD()) {
        cd = DISK_DVD;
    }*/

    // Adjust cd based on index we got back for current drive.
    if (cd_index >= 0) {
        if (cd == DISK_EXPANSION && (cd_index == 2 || cd_index == 3)) {
            cd = cd_index;
        }

        if (cd == cd_index || cd == DISK_CDCHECK) {
            drive = CDFileClass::Current_Drive();
        }
    }

    Theme.Stop();

    // If current drive didn't give use a valid CD index, try again with last drive.
    if (drive == 0) {
        if (CDFileClass::Last_Drive() != 0 && CDFileClass::Current_Drive() != CDFileClass::Last_Drive()) {
            cd_index = Get_CD_Index(CDFileClass::Last_Drive(), 600);

            if (cd_index >= 0) {
                if (cd == DISK_EXPANSION && (cd_index == 2 || cd_index == 3)) {
                    cd = cd_index;
                }

                if (cd == cd_index || cd == DISK_CDCHECK) {
                    drive = CDFileClass::Last_Drive();
                }
            }
        }
    }

    // If we have a valid drive, check that its not the same as the one we already loaded and reload data.
    if (drive != 0) {
        // change_drive:
        /*g_currentCD = cd_index;
        CDFileClass::Set_CD_Drive(drive);
        CDFileClass::Refresh_Search_Drives();

        if (cd == DISK_EXPANSION) {
            cd = DISK_AFTERMATH;
        }

        // If disk needs to change then unload existing files that are present on CD and reload from new CD.
        if (cd > DISK_CDCHECK && cd != _last && cd != DISK_DVD) {
            _last = cd;
            Theme.Stop();

            // Needs init merging.
        }*/
        Change_CD(drive, cd_index, cd);

        return true;
    }

    int scan_delay = 120;
    char msg_buff[128];
    int state = -1;

    while (true) {
        for (int i = 0; i < g_cdList.Get_Drive_Count(); ++i) {
            int list_drive = g_cdList.Get_CD_Drive();
            cd_index = Get_CD_Index(list_drive, scan_delay);

            if (cd_index >= 0) {
                if (cd == DISK_EXPANSION && (cd_index == 2 || cd_index == 3)) {
                    cd = cd_index;
                }

                if (cd == cd_index || cd == DISK_CDCHECK) {
                    drive = list_drive;
                    break;
                }
            }
        }

        if (drive != 0) {
            Change_CD(drive, cd_index, cd);

            return true;
        }

        scan_delay = 300;

        if (cd == DISK_EXPANSION) {
            cd = DISK_AFTERMATH;
        }

        /*if (cd == DISK_DVD) {
            snprintf(msg_buff, sizeof(msg_buff), "Please insert the %s", "RED ALERT DVD");
        } else */

        // Compose string for insert disc message box.
        if (cd != DISK_COUNTERSTRIKE && cd != DISK_AFTERMATH) {
            if (cd == DISK_CDCHECK) {
                snprintf(msg_buff, sizeof(msg_buff), Fetch_String(TXT_CD_DIALOG_1));
            } else {
                snprintf(msg_buff, sizeof(msg_buff), Fetch_String(TXT_CD_DIALOG_2), cd + 1, _cd_name[cd]);
            }
        } else {
            // TODO hardcoded string needs adding to string table.
            snprintf(msg_buff, sizeof(msg_buff), "Please insert the %s", _cd_name[cd]);
        }

        GraphicViewPortClass *old = Set_Logic_Page(&g_seenBuff);
        Theme.Stop();
        state = g_mouse->Get_Mouse_State();
        _font = g_fontPtr;

        if ((((PaletteClass::CurrentPalette[1].Get_Blue() >> 6) | (PaletteClass::CurrentPalette[1].Get_Blue() << 2))
                + ((PaletteClass::CurrentPalette[1].Get_Red() >> 6) | (PaletteClass::CurrentPalette[1].Get_Red() << 2))
                + ((PaletteClass::CurrentPalette[1].Get_Green() >> 6) | (PaletteClass::CurrentPalette[1].Get_Green() << 2)))
            == 0) {
            GamePalette.Set();
        }

        g_keyboard->Clear();

        while (g_mouse->Get_Mouse_State()) {
            g_mouse->Show_Mouse();
        }

        MessageBoxClass msg;

        if (msg.Process(msg_buff, TXT_OK, TXT_CANCEL, TXT_NULL, true) == 1) {
            break;
        }

        while (--state != -1) {
            g_mouse->Hide_Mouse();
        }

        Set_Font(_font);
        Set_Logic_Page(old);
    }

    while (--state != -1) {
        g_mouse->Hide_Mouse();
    }

    return false;
}
