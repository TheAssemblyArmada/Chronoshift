/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Functions for handling things relating to expansion packs.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "expansion.h"
#include "gamefile.h"
#include "gameoptions.h"
#include <captainslog.h>

#ifdef PLATFORM_WINDOWS
#include <winreg.h>
#endif

BOOL Is_Counterstrike_Installed()
{
    static BOOL _checked;
    static BOOL _installed;

    if (g_Options.Expansion_Options_Present()) {
        return g_Options.Is_Counterstrike_Enabled();
    } else if (!_checked) {
#ifdef PLATFORM_WINDOWS
        HKEY result;
        captainslog_debug("Expansion options in chronoshift.ini not present, checking registry for Counterstrike.");

        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Westwood\\Red Alert Windows 95 Edition", 0, KEY_READ, &result)
            == 0) {
            DWORD cbdata = 4;
            BYTE data[4];

            if (RegQueryValueExA(result, "CStrikeInstalled", nullptr, nullptr, data, &cbdata) != 0) {
                _installed = false;
            } else {
                captainslog_debug(
                    "Query succeeded, registry value for Counterstrike is %d.", *reinterpret_cast<DWORD *>(data));
                _installed = *reinterpret_cast<DWORD *>(data);
            }

            RegCloseKey(result);
            _checked = true;
        }
#endif

        if (!_checked) {
            captainslog_debug("Expansion options in chronoshift.ini not present, checking for expand.mix for Counterstrike.");
            GameFileClass fc("expand.mix");
            _installed = fc.Is_Available();
            _checked = true;
        }
    }

    return _installed;
}


BOOL Is_Aftermath_Installed()
{
    static BOOL _checked;
    static BOOL _installed;

    if (g_Options.Expansion_Options_Present()) {
        return g_Options.Is_Aftermath_Enabled();
    } else if (!_checked) {
#ifdef PLATFORM_WINDOWS
        HKEY result;
        captainslog_debug("Expansion options in chronoshift.ini not present, checking registry for Aftermath.");

        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Westwood\\Red Alert Windows 95 Edition", 0, KEY_READ, &result)
            == 0) {
            DWORD cbdata = 4;
            BYTE data[4];

            if (RegQueryValueExA(result, "AftermathInstalled", nullptr, nullptr, data, &cbdata) != 0) {
                _installed = false;
            } else {
                captainslog_debug(
                    "Query succeeded, registry value for Aftermath is %d.", *reinterpret_cast<DWORD *>(data));
                _installed = *reinterpret_cast<DWORD *>(data);
            }

            RegCloseKey(result);
            _checked = true;
        }
#endif

        if (!_checked) {
            captainslog_debug(
                "Expansion options in chronoshift.ini not present, checking for expand2.mix for Aftermath.");
            GameFileClass fc("expand2.mix");
            _installed = fc.Is_Available();
            _checked = true;
        }
    }

    return _installed;
}
