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
    virtual void Remove_Item(int string_index);
    virtual void Remove_Item(const char *string);
    virtual BOOL Remove_Scroll_Bar();
    virtual void Set_Selected_Index(int string_index);
    virtual void Set_Selected_Index(const char *string);
    virtual void Set_Tabs(int *tab_list);
    virtual BOOL Set_View_Index(int index);
    virtual void Step(BOOL step_up);
    virtual void Draw_Entry(int index, int x, int y, int x_max, BOOL redraw);

    ListClass &operator=(ListClass &that);
#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif
protected:
    TextPrintType TextStyle;
    int *Tabs;
    DynamicVectorClass<const char *> Entries;
    int YSpacing; // horziontal line spacing.
    int ThumbSize; // this visible is a line count/page size.
#ifndef CHRONOSHIFT_NO_BITFIELDS
                   // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool HasScrollbar : 1; // & 1
        };
        int m_listFlags;
    };
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

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void ListClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004FCB38, *ListClass::Add);
    Hook_Function(0x004FCBC0, *ListClass::Add_Tail);
    Hook_Function(0x004FCB7C, *ListClass::Add_Head);
    Hook_Function(0x004FCC08, *ListClass::Remove);
    Hook_Function(0x004FCCD8, *ListClass::Flag_To_Redraw);
    Hook_Function(0x004FC710, *ListClass::Peer_To_Peer);
    Hook_Function(0x004FC1B0, *ListClass::Set_Position);
    Hook_Function(0x004FC558, *ListClass::Draw_Me);
    Hook_Function(0x004FC474, *ListClass::Action);
    //Hook_Function(0x004FC2C0, static_cast<int(*)(const char *)>(*ListClass::Add_Item));//char
    //Hook_Function(0x004FC320, static_cast<int(*)(int)>(*ListClass::Add_Item));//int
    Hook_Function(0x004FC7C8, *ListClass::Add_Scroll_Bar);
    Hook_Function(0x004FC640, *ListClass::Bump);
    //Hook_Function(0x004A2840, *ListClass::Count);
    //Hook_Function(0x004FC6D8, *ListClass::Current_Index);
    //Hook_Function(0x004FC700, *ListClass::Current_Item);
    //Hook_Function(0x004FC6B0, *ListClass::Get_Item);
    Hook_Function(0x004FCCB8, *ListClass::Step_Selected_Index);
    //Hook_Function(0x004FC394, *ListClass::Remove_Item);//int
    //Hook_Function(0x004FC444, *ListClass::Remove_Item);//char*
    Hook_Function(0x004FC890, *ListClass::Remove_Scroll_Bar);
    //Hook_Function(0x004FCC40, *ListClass::Set_Selected_Index);//int
    //Hook_Function(0x004FCD10, static_cast<void(*)(int)>(&ListClass::Set_Selected_Index));//char*
    Hook_Function(0x004FC8E8, *ListClass::Set_Tabs);
    Hook_Function(0x004FC768, *ListClass::Set_View_Index);
    Hook_Function(0x004FC678, *ListClass::Step);
    Hook_Function(0x004FC8F4, *ListClass::Draw_Entry);
#endif
}
#endif

#endif // LIST_H
