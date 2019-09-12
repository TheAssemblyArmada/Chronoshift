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
// this is probably what we will need on linux to detect CDs, not priority as can use no cd for dev
// for OS X, it should be okay to just assume anything under /Volumes is a mounted drive
// http://linux.die.net/man/3/getmntent

#include "cdfile.h"
#include "gamedebug.h"
#include <stdio.h>
#include <string.h>

#if defined PLATFORM_WINDOWS
#ifdef __WATCOMC__
#include <windows.h>
#else
#include <fileapi.h>
#include <handleapi.h>
#endif
#define CD_DRIVE_PREFIX "?:"
#define CD_PREFIX_SIZE 2
#define CD_PATH_SEP "\\"
#elif defined(PLATFORM_OSX)
#define CD_DRIVE_PREFIX "/Volumes"
#define CD_PREFIX_SIZE 8
#define CD_PATH_SEP "/"
#else
// Any path can be a "drive" on unix, but we will assume the standard mount of /media
#define CD_DRIVE_PREFIX "/media"
#define CD_PREFIX_SIZE 6
#define CD_PATH_SEP "/"
#endif

#ifndef GAME_DLL
char CDFileClass::s_rawPath[PATH_MAX * 2]; // full raw path of the search drive set.
CDFileClass::SearchDriveType *CDFileClass::s_first; // first entry in the search drive, each entry is linked.
int CDFileClass::s_currentCDDrive;
int CDFileClass::s_lastCDDrive;
#endif
const char *CDFileClass::s_pathSeperator = ";";

CDFileClass::CDFileClass() : m_disableSearchDrives(false) {}

CDFileClass::CDFileClass(char *filename) : m_disableSearchDrives(false)
{
    CDFileClass::Set_Name(filename);
}

/**
 * @brief Set the file name to use for this instance, checks all search paths for requested file name.
 */
const char *CDFileClass::Set_Name(const char *filename)
{
    char path_buffer[PATH_MAX];

    BufferIOFileClass::Set_Name(filename);

    if (m_disableSearchDrives || s_first == nullptr || BufferIOFileClass::Is_Available()) {
        return File_Name();
    }

    SearchDriveType *drive = s_first;

    while (true) {
        // make full or relivent file path.
        snprintf(path_buffer, sizeof(path_buffer), "%s%s", s_first->m_path, filename);

        BufferIOFileClass::Set_Name(path_buffer);

        if (BufferIOFileClass::Is_Available()) {
            break;
        }

        drive = drive->m_next;

        if (drive == nullptr) {
            BufferIOFileClass::Set_Name(filename);
            break;
        }
    }

    return File_Name();
}

/**
 * @brief Open the file from the internally set file name.
 */
BOOL CDFileClass::Open(int rights)
{
    return BufferIOFileClass::Open(rights);
}

/**
 * @brief Open a file with a given file name.
 */
BOOL CDFileClass::Open(const char *filename, int rights)
{
    BufferIOFileClass::Close();

    if (!filename) {
        Error(1, 0, m_filename);
    }

    if (m_disableSearchDrives || rights == FM_WRITE) {
        BufferIOFileClass::Set_Name(filename);
    } else {
        Set_Name(filename);
    }

    return BufferIOFileClass::Open(rights);
}

/**
 * @brief Resets the search path to only those drives stored in s_rawPath.
 */
void CDFileClass::Refresh_Search_Drives()
{
    Clear_Search_Drives();
    Set_Search_Drives(s_rawPath);
}

/**
 * @brief Parses the paths passed by the -CD command line argument or the stored raw path.
 */
int CDFileClass::Set_Search_Drives(const char *path)
{
    char path_buffer[PATH_MAX];

    BOOL paths_set = false;
    BOOL unk_BOOL = false;
    int retval = 0;

    if (path == nullptr) {
        return 0;
    }

    // DEBUG_LOG("Current search path is '%s', appending '%s'.\n", s_rawPath, path);

    // Append path to raw path list
    if (path != s_rawPath) {
        // PATH_MAX * 2
        int current_len = (int)strlen(s_rawPath);
        // current_len += snprintf(&s_rawPath[current_len], sizeof(s_rawPath) - current_len, "%s", s_pathSeperator);
        // snprintf(&s_rawPath[current_len], sizeof(s_rawPath) - current_len, "%s", path);
        current_len += snprintf(&s_rawPath[current_len], PATH_MAX * 2 - current_len, "%s", s_pathSeperator);
        snprintf(&s_rawPath[current_len], PATH_MAX * 2 - current_len, "%s", path);
    }

    // Scan through the path looking for semicolons to delimit seperate
    // paths to add.
    for (char *path_ptr = strtok(s_rawPath, s_pathSeperator); path_ptr; path_ptr = strtok(0, s_pathSeperator)) {
        if (strlen(path_ptr) != 0) {
            snprintf(path_buffer, sizeof(path_buffer), "%s", path_ptr);
            // strncpy(path_buffer, path_ptr, sizeof(path_buffer));

            // This checks the end of the path for a path seperator and puts one in place if it doesn't have one.
            char end_check = path_ptr[strlen(path_buffer) - 1];
            if ((end_check < ':' && end_check != '/') || (end_check > ':' && end_check != '\\')) {
                // strcat(path_buffer, CD_PATH_SEP);
                snprintf(&path_buffer[strlen(path_buffer)], sizeof(path_buffer) - strlen(path_buffer), "%s", CD_PATH_SEP);
            }

            // Check if we have our "disk drive" prefix, if so we need to check which
            // disk we are dealing with.
            // DEBUG_LOG("Checking if '%s' is a CD path.\n", path_buffer);
            if (strncmp(path_buffer, CD_DRIVE_PREFIX, CD_PREFIX_SIZE) == 0) {
// Only windows uses the drive logic that this applies to
#if defined(PLATFORM_WINDOWS)
                if (s_currentCDDrive != 0) {
                    paths_set = true;
                    // Move Get_CD_Index and its static vars if any into CDFileClass?
                    // Need to decide what we are doing with this
                    // DEBUG_LOG("Adding CD drive path as search drive.\n", path_buffer);
                    if (Get_CD_Index(s_currentCDDrive, 120) >= 0) {
                        path_buffer[0] = s_currentCDDrive + 'A';
                        // surely this is path buffer
                        // Add_Search_Drive((const char *)v13);
                        DEBUG_LOG("Adding '%s as CD path.\n", path_buffer);
                        Add_Search_Drive(path_buffer);
                    }
                }
#else // !PLATFORM_WINDOWS
                paths_set = true;
                Add_Search_Drive(path_buffer);
#endif // PLATFORM_WINDOWS

            } else {
                paths_set = true;
                Add_Search_Drive(path_buffer);
            }
        }
    }

    // This gets set if we add a valid path at any point
    if (paths_set) {
        // Never actually gets set that I can see.
        if (unk_BOOL) {
            return 2;
        }

        return 0;
    }

    return 1;
}

/**
 * @brief Add a path to the list of paths to search for a file.
 */
void CDFileClass::Add_Search_Drive(const char *path)
{
    SearchDriveType *entry = new SearchDriveType(nullptr, strdup(path));

    if (s_first != nullptr) {
        // Traverse linked list to end and add new node
        SearchDriveType *i = s_first;

        while (i->m_next != nullptr) {
            i = i->m_next;
        }

        i->m_next = entry;

    } else {
        s_first = entry;
    }
}

/**
 * @brief Set current CD drive to use.
 */
void CDFileClass::Set_CD_Drive(int cd_drive)
{
    s_lastCDDrive = s_currentCDDrive;
    s_currentCDDrive = cd_drive;
}

/**
 * @brief Clears the list of paths to search for files.
 */
void CDFileClass::Clear_Search_Drives()
{
    SearchDriveType *entry = s_first;

    while (entry != nullptr) {
        // delete the current 'entry' and set it to the next pointer in the list
        SearchDriveType *next = entry->m_next;
        delete entry;
        entry = next;
    }

    // void 's_first', removes access the linked list.
    s_first = nullptr;
}
/**
 * @brief Tests if a given CD drive has a disk inserted by checking if any files are found at the root path.
 */
BOOL CDFileClass::Is_Disk_Inserted(int cd_drive)
{
    // Change to return true for no-cd.
#if defined(PLATFORM_WINDOWS)
    static const char Scan[] = "?:\\*.*";

    char name[16];
    LPWIN32_FIND_DATAA file;
    memset(&file, 0, sizeof(file));

    strcpy(name, Scan);

    name[0] = cd_drive + 'A';

    HANDLE h = FindFirstFileA(name, file);
    if (h == INVALID_HANDLE_VALUE) {
        return false;
    }

    FindClose(h);

    return true;
#else
    // TODO!
    return true;
#endif // _MSC_VER
}
