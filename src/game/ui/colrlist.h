/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for the remap color selector type UI controls.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef COLRLIST_H
#define COLRLIST_H

#include "always.h"
#include "list.h"

enum SelectEnum
{
    SELECT_0 = 0, // Text only, indexed color
    SELECT_1 = 1, // Text only, RemapType color, shadow on Grad6pt
    SELECT_2 = 2, // OutlineRect in RemapType, Text in indexed
    SELECT_3 = 3, // Filled Rect in RemapType, Text in indexed, shadow on Grad6pt
    SELECT_4 = 4 // Filled Rect in RemapType, no text.
};

class ColorListClass : public ListClass
{
public:
    ColorListClass(
        int id, int x, int y, int w, int h, TextPrintType text_style, void *up_btn_shape, void *down_btn_shape);
    ColorListClass(ColorListClass &that);
    virtual ~ColorListClass();

    ColorListClass &operator=(ColorListClass &that);

    virtual void Remove_Item(const char *string) override;
    virtual void Draw_Entry(int index, int x, int y, int x_max, BOOL redraw) override;
    virtual int Add_Item(const char *string, RemapControlType *remap);
    virtual int Add_Item(int string_index, RemapControlType *remap);
    virtual void Set_Selected_Style(SelectEnum select_style, RemapControlType *remap);

protected:
    DynamicVectorClass<RemapControlType *> RemapList;
    SelectEnum SelectStyle;
    RemapControlType *RemapType;
};

inline ColorListClass &ColorListClass::operator=(ColorListClass &that)
{
    if (this != &that) {
        ListClass::operator=(that);
        RemapList = that.RemapList;
        SelectStyle = that.SelectStyle;
        RemapType = that.RemapType;
    }

    return *this;
}
#endif // COLRLIST_H
