/**
 * @file
 *
 * @author CCHyper
 *
 * @brief 
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "mapeditor.h"
#include "globals.h"

MapEditorClass::MapEditorClass() :
    GameMouseClass()
{
}

MapEditorClass::~MapEditorClass()
{
}

void MapEditorClass::One_Time()
{
    GameMouseClass::One_Time();
}

void MapEditorClass::Init_IO()
{
    GameMouseClass::Init_IO();
}

void MapEditorClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    GameMouseClass::AI(key, mouse_x, mouse_y);
}

void MapEditorClass::Draw_It(BOOL force_redraw)
{
    GameMouseClass::Draw_It(force_redraw);
}

void MapEditorClass::Detach(ObjectClass *object)
{
    GameMouseClass::Detach(object);
}

void MapEditorClass::Read_INI(GameINIClass &ini)
{
    GameMouseClass::Read_INI(ini);
}

BOOL MapEditorClass::Scroll_Map(DirType dir, int &distance, BOOL redraw)
{
    return GameMouseClass::Scroll_Map(dir, distance, redraw);
}

void MapEditorClass::Write_INI(GameINIClass &ini)
{
    GameMouseClass::Write_INI(ini);
}
