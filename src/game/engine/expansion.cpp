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
#include "gameoptions.h"
#include <captainslog.h>

#ifdef PLATFORM_WINDOWS
#include <winreg.h>
#endif

BOOL Is_Counterstrike_Installed()
{
    if (g_Options.Is_Counterstrike_Enabled()) {
        return true;
    }
#ifdef PLATFORM_WINDOWS
    captainslog_debug("Counterstrike install check from chronoshift.ini failed, checking registry.");
    static BOOL _checked;
    static BOOL _installed;
    HKEY result;

    if (!_checked) {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Westwood\\Red Alert Windows 95 Edition", 0, KEY_READ, &result)
            != 0) {
            captainslog_debug(
                "Failed to open 'SOFTWARE\\Westwood\\Red Alert Windows 95 Edition' subkey in %s.", __CURRENT_FUNCTION__);
            return false;
        }

        DWORD cbdata = 4;
        BYTE data[4];

        if (RegQueryValueExA(result, "CStrikeInstalled", nullptr, nullptr, data, &cbdata) != 0) {
            _installed = false;
        } else {
            captainslog_debug("Query succeeded, registry value for Counterstrike is %d.", *reinterpret_cast<DWORD *>(data));
            _installed = *reinterpret_cast<DWORD *>(data);
        }

        RegCloseKey(result);
        _checked = true;
    }

    return _installed;
#else
    return false;
#endif
}

BOOL Is_Aftermath_Installed()
{
    if (g_Options.Is_Aftermath_Enabled()) {
        return true;
    }
#ifdef PLATFORM_WINDOWS
    captainslog_debug("Aftermath install check from chronoshift.ini failed, checking registry.");
    static BOOL _checked;
    static BOOL _installed;
    HKEY result;

    if (!_checked) {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Westwood\\Red Alert Windows 95 Edition", 0, KEY_READ, &result)
            != 0) {
            captainslog_debug(
                "Failed to open 'SOFTWARE\\Westwood\\Red Alert Windows 95 Edition' subkey in %s.", __CURRENT_FUNCTION__);
            return false;
        }

        DWORD cbdata = 4;
        BYTE data[4];

        if (RegQueryValueExA(result, "AftermathInstalled", nullptr, nullptr, data, &cbdata) != 0) {
            _installed = false;
        } else {
            captainslog_debug("Query succeeded, registry value for Aftermath is %d.", *reinterpret_cast<DWORD *>(data));
            _installed = *reinterpret_cast<DWORD *>(data);
        }

        RegCloseKey(result);
        _checked = true;
    }

    return _installed;
#else
    return false;
#endif
}
