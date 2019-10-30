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
    m_IsExpanded(false),
    m_DropHeight(h),
    m_DropButton(0, down_btn_shape, w + x, y),
    m_DropList(0, x, y + Get_Build_Frame_Height(down_btn_shape), w + Get_Build_Frame_Width(down_btn_shape), h, style,
        up_btn_shape, down_btn_shape)
{
    Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, m_TextStyle); // EDWIN doesnt do this...
    m_DropButton.Make_Peer(*this);
    m_DropList.Make_Peer(*this);
}

DropListClass::DropListClass(DropListClass &that) :
    EditClass(that),
    m_IsExpanded(that.m_IsExpanded),
    m_DropHeight(that.m_DropHeight),
    m_DropButton(that.m_DropButton),
    m_DropList(that.m_DropList)
{
}

LinkClass &DropListClass::Add(LinkClass &that)
{
    m_DropButton.Add(that);

    return LinkClass::Add(that);
}

LinkClass &DropListClass::Add_Tail(LinkClass &that)
{
    m_DropButton.Add_Tail(that);

    return LinkClass::Add_Tail(that);
}

LinkClass &DropListClass::Add_Head(LinkClass &that)
{
    m_DropButton.Add_Head(that);

    return LinkClass::Add_Head(that);
}

void DropListClass::Zap()
{
    Collapse();
    m_DropList.Zap();
    m_DropButton.Zap();
    LinkClass::Zap();
}

GadgetClass *DropListClass::Remove()
{
    if (m_IsExpanded) {
        Collapse();
    }

    m_DropButton.Remove();

    return GadgetClass::Remove();
}

void DropListClass::Flag_To_Redraw()
{
    if (m_IsExpanded) {
        m_DropList.Flag_To_Redraw();
    }

    GadgetClass::Flag_To_Redraw();
}

void DropListClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    if (&peer == &m_DropButton && flags & MOUSE_LEFT_RLSE) {
        if (m_IsExpanded) {
            Collapse();
            key = (KeyNumType)(m_ID | KN_BUTTON);
        } else {
            Expand();
        }
    }

    if (&peer == &m_DropList) {
        strncpy(m_Text, m_DropList.Current_Item(), m_MaxTextLength);
        Flag_To_Redraw();
        key = (KeyNumType)(m_ID | KN_BUTTON);
    }
}

void DropListClass::Clear_Focus()
{
    Collapse();
}

void DropListClass::Set_Position(int x, int y)
{
    GadgetClass::Set_Position(x, y);
    m_DropList.Set_Position(x, y + Get_Build_Frame_Height(m_DropButton.Get_Shape()));
    m_DropButton.Set_Position(x + m_Width, y);
}

int DropListClass::Add_Item(const char *string)
{
    strncpy(m_Text, string, m_MaxTextLength);
    Flag_To_Redraw();

    return m_DropList.Add_Item(string);
}

const char *DropListClass::Current_Item() const
{
    return m_DropList.Current_Item();
}

int DropListClass::Current_Index()
{
    return m_DropList.Current_Index();
}

void DropListClass::Set_Selected_Index(int string_index)
{
    if (string_index > m_DropList.Count()) {
        *m_Text = '\0';
    } else {
        m_DropList.Set_Selected_Index(string_index);
        const char *item = m_DropList.Get_Item(Current_Index());
        strcpy(m_Text, item);
    }
}

void DropListClass::Set_Selected_Index(const char *string)
{
    if (string) {
        for (int i = 0; i < m_DropList.Count(); ++i) {
            const char *item = m_DropList.Get_Item(i);

            if (strcasecmp(string, item) == 0) {
                Set_Selected_Index(i);

                return;
            }
        }
    }
}

int DropListClass::Count() const
{
    return m_DropList.Count();
}

const char *DropListClass::Get_Item(int const string_index) const
{
    return m_DropList.Get_Item(string_index);
}

void DropListClass::Expand()
{
    if (!m_IsExpanded) {
        m_DropList.Set_XPos(m_XPos);
        m_DropList.Set_YPos(m_YPos + 18);
        m_DropList.Set_Width(m_Width);
        m_DropList.Set_Height(m_DropHeight);
        m_DropList.Add(Head_Of_List());
        m_DropList.Flag_To_Redraw();
        m_IsExpanded = true;
    }
}

void DropListClass::Collapse()
{
    if (m_IsExpanded) {
        m_DropList.Remove();
        m_IsExpanded = false;
    }
}
