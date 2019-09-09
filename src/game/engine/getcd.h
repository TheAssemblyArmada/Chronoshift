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

#pragma once

#ifndef GETCD_H
#define GETCD_H

#include "always.h"

class GetCDClass
{
    enum
    {
        INVALID_CD_DRIVE = -1,
    };
public:
    GetCDClass();
    ~GetCDClass(){};

    int Get_Drive_Count() { return m_driveCount; }
    void Set_Drive_Count(int count) { m_driveCount = count; }
    int Get_CD_Drive();
    int Reset_And_Get_CD_Drive();
    void Set_Current_Drive(int drive) { m_currentDrive = drive; }

private:
    int m_drivelist[26];
    int m_driveCount;
    int m_currentDrive;
};

#ifdef GAME_DLL
#include "hooker.h"
extern GetCDClass &g_cdList;
#else
extern GetCDClass g_cdList;
#endif

#endif // GETCD_H
