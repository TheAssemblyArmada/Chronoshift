/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class handling version related information.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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

private:
    void Read_Text_String();
    int Minor_Version();
    int Major_Version();

private:
    int m_VersionNumber;
    int16_t m_MajorVersion;
    int16_t m_MinorVersion;
    char m_VersionString[30];
    char m_FileVersionName[16];
    unsigned m_MinVersion;
    unsigned m_MaxVersion;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_VersionSet : 1; // & 1
    BOOL m_MajorSet : 1; // & 2
    BOOL m_MinorSet : 1; // & 4
    BOOL m_FileRead : 1; // & 8
#else
    bool m_VersionSet;
    bool m_MajorSet;
    bool m_MinorSet;
    bool m_FileRead;
#endif
};

const char *Version_Name();

#ifdef GAME_DLL
extern VersionClass &g_version;
#else
extern VersionClass g_version;
#endif

#endif // VERSION_H
