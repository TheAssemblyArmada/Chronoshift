/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for finding the CD drive.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "getcd.h"
#include <cstring>

using std::memset;

#ifndef CHRONOSHIFT_STANDALONE
GetCDClass &g_cdList = Make_Global<GetCDClass>(0x00680884);
#else
GetCDClass g_cdList;
#endif

GetCDClass::GetCDClass()
{
#ifdef PLATFORM_WINDOWS
    char drive_name[20];

    strcpy(drive_name, "a:\\");
    Set_Drive_Count(0);
    Set_Current_Drive(0);
    memset(m_drivelist, INVALID_CD_DRIVE, sizeof(m_drivelist));

    for (char driveletter = 'a'; driveletter <= 'z'; ++driveletter) {
        drive_name[0] = driveletter;

        if(GetDriveTypeA(drive_name) == DRIVE_CDROM) {
            m_drivelist[m_driveCount++] = driveletter - 'a';
        }
    }
#else
    Set_Drive_Count(0);
    Set_Current_Drive(0);
    memset(m_drivelist, INVALID_CD_DRIVE, sizeof(m_drivelist));
#endif
}

int GetCDClass::Get_CD_Drive()
{
    if (Get_Drive_Count()) {
        if (m_currentDrive == Get_Drive_Count()) {
            Set_Current_Drive(0);
        }

        return m_drivelist[m_currentDrive++];
    }

    return INVALID_CD_DRIVE;
}

int GetCDClass::Reset_And_Get_CD_Drive()
{
    Set_Current_Drive(0);
    return Get_CD_Drive();
}
