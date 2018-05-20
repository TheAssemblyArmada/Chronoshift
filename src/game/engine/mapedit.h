/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Top element of the IOMap stack, the map editor.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MAPEDIT_H
#define MAPEDIT_H

#include "gmouse.h"

class MapEditorClass : public GameMouseClass
{
    class EditorViewportInputClass : public ControlClass
    {
    public:
        EditorViewportInputClass() :
            ControlClass(CONTROL_EDITOR_VIEWPORT_INPUT, 0, 0, 0, 0,
                MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
        {
        }
        virtual ~EditorViewportInputClass() {}

        virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    };
    MapEditorClass();

public:
    virtual void One_Time() override;
    virtual void Init_IO() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Detach(ObjectClass *object) override;
    virtual void Read_INI(CCINIClass &ini) override;
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;

    void Write_INI(CCINIClass &ini); // virtual in edwin

    static EditorViewportInputClass EditorViewportInput; // this is the selection box input and rightclick menu input area

    struct MapEditorClassStruct // for now
    {
        ObjectClass *CurrentObject;
        BOOL ShowPassable;
#ifndef RAPP_NO_BITFIELDS
        // Union/Struct required to get correct packing when compiler packing set to 1.
        union
        {
            struct
            {
                bool HasUnsavedChanges : 1; // 1
                bool Bit_2 : 1; // 2 set only in ctor, never checked anywhere
            };
            int Bitfield;
        };
#else
        bool HasUnsavedChanges;
        bool Bit_2;
#endif
        int field_8CAF[9];
        int field_8CD3[9];
    };

    static MapEditorClassStruct MapEditor;

    MapEditorClass *Hook_CTOR()
    {
        return new(this) MapEditorClass();
    }
};

#endif // MAPEDIT_H