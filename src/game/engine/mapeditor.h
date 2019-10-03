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
#pragma once

#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "always.h"
#include "gmouse.h"

class NoInitClass;

class MapEditorClass : public GameMouseClass
{
public:
    MapEditorClass();
    MapEditorClass(const NoInitClass &noinit) : GameMouseClass(noinit) {}

    ~MapEditorClass();

    virtual void One_Time() override;
    virtual void Init_IO() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw = false) override;
    virtual void Detach(ObjectClass *object) override;
    virtual void Read_INI(GameINIClass &ini) override;
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;
    virtual void Write_INI(GameINIClass &ini) override;

protected:
    // NOTE: All members for this class must be static otherwise it
    //       it will break the exiting MapIO we hook into in RA!
};

#endif // MAPEDITOR_H
