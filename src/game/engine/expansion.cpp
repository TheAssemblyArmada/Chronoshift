#include "expansion.h"

BOOL Is_Counterstrike_Installed()
{
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
    return true;
#endif
}

BOOL Is_Aftermath_Installed()
{
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
    return true;
#endif
}
