/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing lists of selectable options.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LIST_H
#define LIST_H

#include "always.h"
#include "controlc.h"
#include "dialog.h"
#include "language.h"
#include "shapebtn.h"
#include "slider.h"
#include "vector.h"

class ListClass : public ControlClass
{
public:
    ListClass(int id, int x, int y, int w, int h, TextPrintType text_style, void *up_btn_shape, void *down_btn_shape);
    ListClass(ListClass &that);
    virtual ~ListClass();

    virtual LinkClass &Add(LinkClass &that) override;
    virtual LinkClass &Add_Tail(LinkClass &that) override;
    virtual LinkClass &Add_Head(LinkClass &that) override;
    virtual GadgetClass *Remove() override;
    virtual void Flag_To_Redraw() override;
    virtual void Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer) override;
    virtual void Set_Position(int x, int y) override;
    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    virtual int Add_Item(const char *string);
    virtual int Add_Item(int str_id);
    virtual BOOL Add_Scroll_Bar();
    virtual void Bump(BOOL bump_up);
    virtual int Count() const;
    virtual int Current_Index() const;
    virtual const char *Current_Item() const;
    virtual const char *Get_Item(int string_index) const;
    virtual int Step_Selected_Index(int index);
    virtual void Remove_Item(const char *string);
    virtual void Remove_Item(int string_index);
    virtual BOOL Remove_Scroll_Bar();
    virtual void Set_Selected_Index(int string_index);
    virtual void Set_Selected_Index(const char *string);
    virtual void Set_Tabs(int *tab_list);
    virtual BOOL Set_View_Index(int index);
    virtual void Step(BOOL step_up);
    virtual void Draw_Entry(int index, int x, int y, int x_max, BOOL redraw);

    ListClass &operator=(ListClass &that);

protected:
    TextPrintType TextStyle;
    int *Tabs;
    DynamicVectorClass<const char *> Entries;
    int YSpacing; // horziontal line spacing.
    int ThumbSize; // this visible is a line count/page size.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL HasScrollbar : 1; // & 1
#else
    bool HasScrollbar; // Does this List have a scroll bar for scrolling through the entries?
#endif
    ShapeButtonClass ScrollUpButton; // Scroll Up button instance for the scroll bar.
    ShapeButtonClass ScrollDownButton; // Scroll Down button instance for the scroll bar.
    SliderClass Scrollbar; // Scroll bar instance for the list.
    int CurrentIndex;
    int ViewIndex;
};

void Clear_Listbox(ListClass *list);
BOOL Is_Link_In_List(LinkClass *list, LinkClass *link);

inline ListClass &ListClass::operator=(ListClass &that)
{
    if (this != &that) {
        ControlClass::operator=(that);
        TextStyle = that.TextStyle;
        Tabs = that.Tabs;
        Entries = that.Entries;
        YSpacing = that.YSpacing;
        ThumbSize = that.ThumbSize;
        HasScrollbar = that.HasScrollbar;
        ScrollUpButton = that.ScrollUpButton;
        ScrollDownButton = that.ScrollDownButton;
        Scrollbar = that.Scrollbar;
        CurrentIndex = that.CurrentIndex;
        ViewIndex = that.ViewIndex;
        ScrollUpButton.Make_Peer(*this);
        ScrollDownButton.Make_Peer(*this);
        Scrollbar.Make_Peer(*this);
    }

    return *this;
}

#endif // LIST_H
