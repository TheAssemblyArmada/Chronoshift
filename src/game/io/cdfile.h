/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief  FileClass layer for handling CDs and additional search paths.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CDFILECLASS_H
#define CDFILECLASS_H

#include "always.h"
#include "bfiofile.h"
#include "cd.h"
#include <malloc.h>

class CDFileClass : public BufferIOFileClass
{
private:
    // linked drive list
    struct SearchDriveType
    {
        SearchDriveType() : m_Next(nullptr), m_Path(nullptr) {}
        SearchDriveType(SearchDriveType *next, const char *path) : m_Next(next), m_Path(path) {}
        ~SearchDriveType()
        {
            if (m_Path != nullptr) {
                free((void *)m_Path);
            }
        }

        SearchDriveType *m_Next;
        const char *m_Path;
    };
public:
    CDFileClass();
    CDFileClass(char *filename);
    virtual ~CDFileClass() {}

    virtual const char *Set_Name(const char *filename) override;
    virtual BOOL Open(int rights) override;
    virtual BOOL Open(const char *filename, int rights) override;
    void Disable_Search_Drives(bool disable) { m_DisableSearchDrives = disable; }

    static void Refresh_Search_Drives();
    static int Set_Search_Drives(const char *path);
    static void Add_Search_Drive(const char *path);
    static void Set_CD_Drive(int cd_drive);
    static void Clear_Search_Drives();
    static BOOL Is_Disk_Inserted(int cd_drive);
    static BOOL Has_Paths() { return s_first != nullptr; }
    static int Last_Drive() { return s_lastCDDrive; }
    static int Current_Drive() { return s_currentCDDrive; }

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_DisableSearchDrives : 1;
#else
    bool m_DisableSearchDrives;
#endif

private:
#ifdef GAME_DLL
    static char *s_rawPath;
    static SearchDriveType *&s_first;
    static int &s_currentCDDrive;
    static int &s_lastCDDrive;
#else
    static char s_rawPath[];
    static SearchDriveType *s_first;
    static int s_currentCDDrive;
    static int s_lastCDDrive;
#endif
    static const char *s_pathSeperator;
};

#endif // CDFILECLASS_H
