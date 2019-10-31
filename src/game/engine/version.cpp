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
#include "version.h"
#include "expansion.h"
#include "gitverinfo.h"
#include "rawfile.h"
#include "language.h"
#include <cstdio>
#include <cstring>

using std::strlen;
using std::snprintf;

#ifndef GAME_DLL
VersionClass g_version;
#endif

VersionClass::VersionClass() :
    m_VersionNumber(0),
    m_MajorVersion(0),
    m_MinorVersion(0),
    m_MinVersion(0),
    m_MaxVersion(0),
    m_VersionSet(false),
    m_MajorSet(false),
    m_MinorSet(false),
    m_FileRead(false)
{
    m_FileVersionName[0] = '\0';
    m_VersionString[0] = '\0';
}

int VersionClass::Version_Number()
{
    if (!m_FileRead) {
        Read_Text_String();
        m_FileRead = true;
    }

    if (!m_VersionSet) {
        m_VersionNumber = (Major_Version() << 16) | Minor_Version();
        m_VersionSet = true;
    }

    return m_VersionNumber;
}

const char *VersionClass::Version_Name()
{
    unsigned minor = Minor_Version();

    for (int i = 0; i < 4; ++i) {
        if (minor % 16) {
            break;
        }

        minor >>= 4;
    }

    snprintf(m_VersionString, sizeof(m_VersionString), "%x.%x", Major_Version(), minor);

    return m_VersionString;
}

const char *VersionClass::Version_String()
{
    static char _buffer[VER_STRING_MAX_LEN];
    static BOOL _done;

    if (!_done) {
        if (!m_FileRead) {
            Read_Text_String();
            m_FileRead = true;
        }

        snprintf(_buffer, sizeof(_buffer), "%d.%02d", Major_Version(), Minor_Version());

        strlcat(_buffer, Get_Language_Char(), sizeof(_buffer));

        if (Is_Counterstrike_Installed()) {
            strlcat(_buffer, "CS", sizeof(_buffer));
        }

        if (Is_Aftermath_Installed()) {
            strlcat(_buffer, "AM", sizeof(_buffer));
        }

        strlcat(_buffer, "\r", sizeof(_buffer));
        strlcat(_buffer, g_gitShortSHA1, sizeof(_buffer));

        if (strlen(m_FileVersionName) > 0) {
            strlcat(_buffer, "\r", sizeof(_buffer));
            strlcat(_buffer, m_FileVersionName, sizeof(_buffer));
        }
    }

    return _buffer;
}

CommProtocolEnum VersionClass::Version_Protocol(unsigned version)
{
    static const VersionProtocolType _version_protocol[] = {
        { 0x1000, COMPROTO_ZERO },
        { 0x2000, COMPROTO_ONE},
        { 0x10000, COMPROTO_TWO }
    };

    // Find lowest protocol that the provided version is greater than.
    for (int i = 0; i < ARRAY_SIZE(_version_protocol); ++i) {
        if (version >= _version_protocol[i].min_version) {
            return _version_protocol[i].protocol;
        }
    }

    // If none found, return highest version protocol.
    return _version_protocol[ARRAY_SIZE(_version_protocol) - 1].protocol;
}

void VersionClass::Init_Clipping()
{
    m_MinVersion = Min_Version();
    m_MaxVersion = Max_Version();
}

int VersionClass::Clip_Version(unsigned min, unsigned max)
{
    if (min > m_MaxVersion) {
        return -1;
    }

    if (max < m_MinVersion) {
        return 0;
    }

    if (min > m_MinVersion) {
        m_MinVersion = min;
    }

    if (max < m_MaxVersion) {
        m_MaxVersion = max;
    }

    return m_MaxVersion;
}

int VersionClass::Min_Version()
{
    return (g_majorVersion << 16) | g_minorVersion;
}

int VersionClass::Max_Version()
{
    return (g_majorVersion << 16) | g_minorVersion;
}

void VersionClass::Read_Text_String()
{
    RawFileClass fc("version.txt");

    if (fc.Is_Available(false)) {
        fc.Read(m_FileVersionName, sizeof(m_FileVersionName));
        m_FileVersionName[sizeof(m_FileVersionName) - 1] = '\0';

        while (m_FileVersionName[strlen(m_FileVersionName) - 1] == '\r') {
            m_FileVersionName[strlen(m_FileVersionName) - 1] = '\0';
        }
    } else {
        m_FileVersionName[0] = '\0';
    }
}

int VersionClass::Minor_Version()
{
    if (!m_FileRead) {
        Read_Text_String();
        m_FileRead = true;
    }

    if (!m_MinorSet) {
        m_MinorVersion = g_minorVersion;
        m_MinorSet = true;
    }

    return m_MinorVersion;
}

int VersionClass::Major_Version()
{
    if (!m_FileRead) {
        Read_Text_String();
        m_FileRead = true;
    }

    if (!m_MajorSet) {
        m_MajorVersion = g_majorVersion;
        m_MajorSet = true;
    }

    return m_MajorVersion;
}

const char *Version_Name() { return g_version.Version_String(); }
