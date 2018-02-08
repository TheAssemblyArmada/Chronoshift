/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling map loading and logic.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "map.h"

MapClass::MapClass() :
    MapCellX(0),
    MapCellY(0),
    MapCellWidth(0),
    MapCellHeight(0),
    TotalValue(0),
    Array(),
    XSize(0),
    YSize(0),
    TotalSize(0)
{
}

void MapClass::One_Time()
{
}

void MapClass::Init_Clear()
{
}

void MapClass::Alloc_Cells()
{
}

void MapClass::Free_Cells()
{
}

void MapClass::Init_Cells()
{
}

void MapClass::Detach(ObjectClass * object)
{
}

void MapClass::Logic_AI()
{
}

void MapClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
}
