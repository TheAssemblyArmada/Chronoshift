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

BOOL Is_Counterstrike_Installed()
{
    if (Options.Is_Counterstrike_Enabled()) {
        return true;
    }
#ifdef PLATFORM_WINDOWS
    static BOOL _checked;
    static BOOL _installed;
    HKEY result;
    BYTE data[4];
    DWORD cbdata;

    if (!_checked) {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Westwood\\Red Alert Windows 95 Edition", 0, KEY_READ, &result)
            != 0) {
            return false;
        }

        if (RegQueryValueExA(result, "CStrikeInstalled", nullptr, nullptr, data, &cbdata) != 0) {
            _installed = false;
        } else {
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
    if (Options.Is_Aftermath_Enabled()) {
        return true;
    }
#ifdef PLATFORM_WINDOWS
    static BOOL _checked;
    static BOOL _installed;
    HKEY result;
    BYTE data[4];
    DWORD cbdata;

    if (!_checked) {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Westwood\\Red Alert Windows 95 Edition", 0, KEY_READ, &result)
            != 0) {
            return false;
        }

        if (RegQueryValueExA(result, "AftermathInstalled", nullptr, nullptr, data, &cbdata) != 0) {
            _installed = false;
        } else {
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
