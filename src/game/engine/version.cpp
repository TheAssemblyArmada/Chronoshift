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
#include "version.h"
#include "expansion.h"
#include "minmax.h"
#include "rawfileclass.h"
#include "stringex.h"
#include <stdio.h>

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
VersionClass &g_version = Make_Global<VersionClass>(0x00667810);
#else
VersionClass g_version;
#endif

VersionClass::VersionClass() :
    m_versionNumber(0),
    m_majorVersion(0),
    m_minorVersion(0),
    m_minVersion(0),
    m_maxVersion(0),
    m_versionSet(false),
    m_majorSet(false),
    m_minorSet(false),
    m_fileRead(false)
{
    m_fileVersionName[0] = '\0';
    m_versionString[0] = '\0';
}

int VersionClass::Version_Number()
{
    if (!m_fileRead) {
        Read_Text_String();
        m_fileRead = true;
    }

    if (!m_versionSet) {
        m_versionNumber = (Major_Version() << 16) | Minor_Version();
        m_versionSet = true;
    }

    return m_versionNumber;
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

    snprintf(m_versionString, sizeof(m_versionString), "%x.%x", Major_Version(), minor);

    return m_versionString;
}

const char *VersionClass::Version_String()
{
    static char _buffer[VER_STRING_MAX_LEN];
    static BOOL _done;

    if (!_done) {
        if (!m_fileRead) {
            Read_Text_String();
            m_fileRead = true;
        }

        snprintf(_buffer, sizeof(_buffer), "%d.%02d", Major_Version(), Minor_Version());
        // TODO, this should change based on current language.
        strlcat(_buffer, "E", sizeof(_buffer));

        if (Is_Counterstrike_Installed()) {
            strlcat(_buffer, "CS", sizeof(_buffer));
        }

        if (Is_Aftermath_Installed()) {
            strlcat(_buffer, "AM", sizeof(_buffer));
        }

        strlcat(_buffer, "\r" RAPP_COMMIT_SHA1_SHORT, sizeof(_buffer));

        if (strlen(m_fileVersionName) > 0) {
            strlcat(_buffer, "\r", sizeof(_buffer));
            strlcat(_buffer, m_fileVersionName, sizeof(_buffer));
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
    m_minVersion = Min_Version();
    m_maxVersion = Max_Version();
}

int VersionClass::Clip_Version(unsigned min, unsigned max)
{
    if (min > m_maxVersion) {
        return -1;
    }

    if (max < m_minVersion) {
        return 0;
    }

    if (min > m_minVersion) {
        m_minVersion = min;
    }

    if (max < m_maxVersion) {
        m_maxVersion = max;
    }

    return m_maxVersion;
}

int VersionClass::Min_Version()
{
    return (RAPP_MAJOR << 16) | RAPP_MINOR;
}

int VersionClass::Max_Version()
{
    return (RAPP_MAJOR << 16) | RAPP_MINOR;
}

void VersionClass::Read_Text_String()
{
    RawFileClass fc("version.txt");

    if (fc.Is_Available(false)) {
        fc.Read(m_fileVersionName, sizeof(m_fileVersionName));
        m_fileVersionName[sizeof(m_fileVersionName) - 1] = '\0';

        while (m_fileVersionName[strlen(m_fileVersionName) - 1] == '\r') {
            m_fileVersionName[strlen(m_fileVersionName) - 1] = '\0';
        }
    } else {
        m_fileVersionName[0] = '\0';
    }
}

int VersionClass::Minor_Version()
{
    if (!m_fileRead) {
        Read_Text_String();
        m_fileRead = true;
    }

    if (!m_minorSet) {
        m_minorVersion = RAPP_MINOR;
        m_minorSet = true;
    }

    return m_minorVersion;
}

int VersionClass::Major_Version()
{
    if (!m_fileRead) {
        Read_Text_String();
        m_fileRead = true;
    }

    if (!m_majorSet) {
        m_majorVersion = RAPP_MAJOR;
        m_majorSet = true;
    }

    return m_majorVersion;
}

const char *Version_Name() { return g_version.Version_String(); }
