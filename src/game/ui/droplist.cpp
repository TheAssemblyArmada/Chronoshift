/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing a droppable lists of selectable options.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "droplist.h"
#include "dialog.h"
#include "drawshape.h"

DropListClass::DropListClass(int id, char *text_buff, int text_size, TextPrintType style, int x, int y, int w, int h,
    void *up_btn_shape, void *down_btn_shape) :
    EditClass(id, text_buff, text_size, style, x, y, w, 18, EDIT_TEXT | EDIT_NUMS | EDIT_SYMS),
    IsExpanded(false),
    DropHeight(h),
    DropButton(0, down_btn_shape, w + x, y),
    DropList(0, x, y + Get_Build_Frame_Height(down_btn_shape), w + Get_Build_Frame_Width(down_btn_shape), h, style,
        up_btn_shape, down_btn_shape)
{
    Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, TextStyle); // EDWIN doesnt do this...
    DropButton.Make_Peer(*this);
    DropList.Make_Peer(*this);
}

DropListClass::DropListClass(DropListClass &that) :
    EditClass(that),
    IsExpanded(that.IsExpanded),
    DropHeight(that.DropHeight),
    DropButton(that.DropButton),
    DropList(that.DropList)
{
}

LinkClass &DropListClass::Add(LinkClass &that)
{
    DropButton.Add(that);

    return LinkClass::Add(that);
}

LinkClass &DropListClass::Add_Tail(LinkClass &that)
{
    DropButton.Add_Tail(that);

    return LinkClass::Add_Tail(that);
}

LinkClass &DropListClass::Add_Head(LinkClass &that)
{
    DropButton.Add_Head(that);

    return LinkClass::Add_Head(that);
}

void DropListClass::Zap()
{
    Collapse();
    DropList.Zap();
    DropButton.Zap();
    LinkClass::Zap();
}

GadgetClass *DropListClass::Remove()
{
    if (IsExpanded) {
        Collapse();
    }

    DropButton.Remove();

    return GadgetClass::Remove();
}

void DropListClass::Flag_To_Redraw()
{
    if (IsExpanded) {
        DropList.Flag_To_Redraw();
    }

    GadgetClass::Flag_To_Redraw();
}

void DropListClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    if (&peer == &DropButton && flags & MOUSE_LEFT_RLSE) {
        if (IsExpanded) {
            Collapse();
            key = (KeyNumType)(ID | KN_BUTTON);
        } else {
            Expand();
        }
    }

    if (&peer == &DropList) {
        strncpy(Text, DropList.Current_Item(), MaxTextLength);
        Flag_To_Redraw();
        key = (KeyNumType)(ID | KN_BUTTON);
    }
}

void DropListClass::Clear_Focus()
{
    Collapse();
}

void DropListClass::Set_Position(int x, int y)
{
    GadgetClass::Set_Position(x, y);
    DropList.Set_Position(x, y + Get_Build_Frame_Height(DropButton.Get_Shape()));
    DropButton.Set_Position(x + Width, y);
}

int DropListClass::Add_Item(const char *string)
{
    strncpy(Text, string, MaxTextLength);
    Flag_To_Redraw();

    return DropList.Add_Item(string);
}

const char *DropListClass::Current_Item() const
{
    return DropList.Current_Item();
}

int DropListClass::Current_Index()
{
    return DropList.Current_Index();
}

void DropListClass::Set_Selected_Index(int string_index)
{
    if (string_index > DropList.Count()) {
        *Text = '\0';
    } else {
        DropList.Set_Selected_Index(string_index);
        const char *item = DropList.Get_Item(Current_Index());
        strcpy(Text, item);
    }
}

void DropListClass::Set_Selected_Index(const char *string)
{
    if (string) {
        for (int i = 0; i < DropList.Count(); ++i) {
            const char *item = DropList.Get_Item(i);

            if (strcasecmp(string, item) == 0) {
                Set_Selected_Index(i);

                return;
            }
        }
    }
}

int DropListClass::Count() const
{
    return DropList.Count();
}

const char *DropListClass::Get_Item(int const string_index) const
{
    return DropList.Get_Item(string_index);
}

void DropListClass::Expand()
{
    if (!IsExpanded) {
        DropList.Set_XPos(XPos);
        DropList.Set_YPos(YPos + 18);
        DropList.Set_Width(Width);
        DropList.Set_Height(DropHeight);
        DropList.Add(Head_Of_List());
        DropList.Flag_To_Redraw();
        IsExpanded = true;
    }
}

void DropListClass::Collapse()
{
    if (IsExpanded) {
        DropList.Remove();
        IsExpanded = false;
    }
}
