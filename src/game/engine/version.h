/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class handling version related information.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef VERSION_H
#define VERSION_H

#include "always.h"

enum CommProtocolEnum
{
    COMPROTO_ZERO,
    COMPROTO_ONE,
    COMPROTO_TWO,
};

struct VersionProtocolType
{
    unsigned min_version;
    CommProtocolEnum protocol;
};

class VersionClass
{
    enum
    {
        VER_STRING_MAX_LEN = 128,
    };

public:
    VersionClass();
    virtual ~VersionClass() {}

    int Version_Number();
    const char *Version_Name();
    const char *Version_String();
    CommProtocolEnum Version_Protocol(unsigned version);
    void Init_Clipping();
    int Clip_Version(unsigned min, unsigned max);
    int Min_Version();
    int Max_Version();

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

private:
    void Read_Text_String();
    int Minor_Version();
    int Major_Version();

private:
    int m_versionNumber;
    int16_t m_majorVersion;
    int16_t m_minorVersion;
    char m_versionString[30];
    char m_fileVersionName[16];
    unsigned m_minVersion;
    unsigned m_maxVersion;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_versionSet : 1; // & 1
            bool m_majorSet : 1; // & 2
            bool m_minorSet : 1; // & 4
            bool m_fileRead : 1; // & 8
        };
        int m_versionFlags;
    };
#else
    bool m_versionSet;
    bool m_majorSet;
    bool m_minorSet;
    bool m_fileRead;
#endif
};

const char *Version_Name();

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern VersionClass &g_version;

inline void VersionClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x00589960, ::Version_Name);
#endif
}
#else
extern VersionClass g_version;
#endif

#endif // VERSION_H
