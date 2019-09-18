/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief FileClass pure virtual base class providing the file io interface.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "basefile.h"

time_t FileClass::Get_Date_Time()
{
    return 0;
}

BOOL FileClass::Set_Date_Time(time_t date_time)
{
    return false;
}
