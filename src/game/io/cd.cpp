/**
* @file
*
* @Author OmniBlade
*
* @brief  Disk change handling.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#include	"cd.h"

int Get_CD_Index(int cd, int delay)
{
    // TODO Requires CountdownTimer
    static int(*_get_cd)(int, int) = reinterpret_cast<int(*)(int, int)>(0x004AAB2C);
    return _get_cd(cd, delay);
}

int Force_CD_Available(int cd)
{
    // TODO Requires ThemeClass CCFileClass MixFileClass
    static int(*_force_cd)(int) = reinterpret_cast<int(*)(int)>(0x004AAC58);
    return _force_cd(cd);
}