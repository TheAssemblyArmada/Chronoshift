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
#include "list.h"
#include "gbuffer.h"
#include "mouse.h"
#include "textprint.h"
#include <algorithm>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

ListClass::ListClass(
    int id, int x, int y, int w, int h, TextPrintType text_style, void *up_btn_shape, void *down_btn_shape) :
    ControlClass(id, x, y, w, h, MOUSE_LEFT_PRESS | MOUSE_LEFT_RLSE | KEYBOARD_INPUT /*261*/, false),
    m_TextStyle(text_style),
    m_Tabs(nullptr),
    m_Entries(),
    m_YSpacing(0),
    m_ThumbSize(0),
    m_HasScrollbar(false),
    m_ScrollUpButton(0, up_btn_shape, x + w, y),
    m_ScrollDownButton(0, down_btn_shape, x + w, y + h),
    m_Scrollbar(0, x + w, y, 0, h, 1),
    m_CurrentIndex(0),
    m_ViewIndex(0)
{
    // set position
    m_ScrollUpButton.Set_XPos(m_ScrollUpButton.Get_XPos() - m_ScrollUpButton.Get_Width());
    m_ScrollDownButton.Set_XPos(m_ScrollDownButton.Get_XPos() - m_ScrollDownButton.Get_Width());
    m_ScrollDownButton.Set_YPos(m_ScrollDownButton.Get_YPos() - m_ScrollDownButton.Get_Height());

    m_Scrollbar.Set_XPos(m_Scrollbar.Get_XPos() - std::max(m_ScrollUpButton.Get_Width(), m_ScrollDownButton.Get_Width()));
    m_Scrollbar.Set_YPos(m_ScrollUpButton.Get_Height() + m_YPos);
    m_Scrollbar.Set_Height(m_Scrollbar.Get_Height() - (m_ScrollDownButton.Get_Height() + m_ScrollUpButton.Get_Height()));
    m_Scrollbar.Set_Width(std::max(m_ScrollUpButton.Get_Width(), m_ScrollDownButton.Get_Width()));

    // The nullptr Fancy_Text_Print sets the font pointer and the dimension globals
    // for the YSpacing and ThumbSize
    Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, m_TextStyle);
    m_YSpacing = g_FontYSpacing + g_FontHeight - 1;
    m_ThumbSize = (h - 1) / m_YSpacing;
}

ListClass::ListClass(ListClass &that) :
    ControlClass(that),
    m_TextStyle(that.m_TextStyle),
    m_Tabs(that.m_Tabs),
    m_Entries(that.m_Entries),
    m_YSpacing(that.m_YSpacing),
    m_ThumbSize(that.m_ThumbSize),
    m_HasScrollbar(that.m_HasScrollbar),
    m_ScrollUpButton(that.m_ScrollUpButton),
    m_ScrollDownButton(that.m_ScrollDownButton),
    m_Scrollbar(that.m_Scrollbar),
    m_CurrentIndex(that.m_CurrentIndex),
    m_ViewIndex(that.m_ViewIndex)
{
    m_ScrollUpButton.Make_Peer(*this);
    m_ScrollDownButton.Make_Peer(*this);
    m_Scrollbar.Make_Peer(*this);
}

ListClass::~ListClass()
{
    Remove_Scroll_Bar();
}

LinkClass &ListClass::Add(LinkClass &that)
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Add(that);
        m_ScrollDownButton.Add(that);
        m_Scrollbar.Add(that);
    }

    return LinkClass::Add(that);
}

LinkClass &ListClass::Add_Tail(LinkClass &that)
{
    LinkClass::Add_Tail(that);

    if (m_HasScrollbar) {
        m_ScrollUpButton.Add_Tail(that);
        m_ScrollDownButton.Add_Tail(that);
        m_Scrollbar.Add_Tail(that);
    }

    return Head_Of_List();
}

LinkClass &ListClass::Add_Head(LinkClass &that)
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Add_Head(that);
        m_ScrollDownButton.Add_Head(that);
        m_Scrollbar.Add_Head(that);
    }

    return LinkClass::Add_Head(that);
}

GadgetClass *ListClass::Remove()
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Remove();
        m_ScrollDownButton.Remove();
        m_Scrollbar.Remove();
    }

    return GadgetClass::Remove();
}

void ListClass::Flag_To_Redraw()
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Flag_To_Redraw();
        m_ScrollDownButton.Flag_To_Redraw();
        m_Scrollbar.Flag_To_Redraw();
    }

    GadgetClass::Flag_To_Redraw();
}

void ListClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    if (flags & MOUSE_LEFT_RLSE) {
        if (&m_ScrollUpButton == &peer) {
            Step(true); // step up
        }

        if (&m_ScrollDownButton == &peer) {
            Step(false); // step down
        }
    }

    if (&m_Scrollbar == &peer) {
        Set_View_Index(m_Scrollbar.Get_Value());
    }
}

void ListClass::Set_Position(int x, int y)
{
    // Set scroll up button position.
    m_ScrollUpButton.Set_XPos(x + m_Width - m_ScrollUpButton.Get_Width());
    m_ScrollUpButton.Set_YPos(y);

    // Set scroll down button position.
    m_ScrollDownButton.Set_XPos(x + m_Width - m_ScrollDownButton.Get_Width());
    m_ScrollDownButton.Set_YPos(y + m_Height - m_ScrollDownButton.Get_Height());

    // Set scroll bar position.
    m_Scrollbar.Set_XPos(x + m_Width - std::max(m_ScrollUpButton.Get_Width(), m_ScrollDownButton.Get_Width()));
    m_Scrollbar.Set_YPos(y + m_ScrollUpButton.Get_Height());
    m_Scrollbar.Set_Width(std::max(m_ScrollUpButton.Get_Width(), m_ScrollDownButton.Get_Width()));
    m_Scrollbar.Set_Height(m_Height - (m_ScrollDownButton.Get_Height() + m_ScrollUpButton.Get_Height()));
}

BOOL ListClass::Draw_Me(BOOL redraw)
{
    if (GadgetClass::Draw_Me(redraw)) {
        if (&g_SeenBuff == g_LogicPage) {
            g_Mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, m_Width + m_XPos, m_Height + m_YPos);
        }

        Draw_Box(m_XPos, m_YPos, m_Width, m_Height, BOX_STYLE_4, 1);

        if (m_Entries.Count()) {
            for (int index = 0; index < m_ThumbSize; ++index) {
                if (m_Entries.Count() > index + m_ViewIndex) {
                    BOOL redraw_entry = index + m_ViewIndex == Current_Index();
                    Draw_Entry(index + m_ViewIndex, m_XPos + 1, m_YPos + index * m_YSpacing + 1, m_Width - 2, redraw_entry);
                }
            }
        }

        if (&g_SeenBuff == g_LogicPage) {
            g_Mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

BOOL ListClass::Action(unsigned flags, KeyNumType &key)
{
    if (flags & MOUSE_LEFT_RLSE) {
        key = KN_NONE;
        ControlClass::Action(flags & (~MOUSE_LEFT_RLSE), key);

        return true;
    }

    if (flags & KEYBOARD_INPUT) {
        if (key == KN_UP) {
            Step_Selected_Index(-1);
            key = KN_NONE;
        } else if (key == KN_DOWN) {
            Step_Selected_Index(1);
            key = KN_NONE;
        } else {
            flags &= ~KEYBOARD_INPUT;
        }
    } else {
        m_CurrentIndex = m_ViewIndex + (g_Mouse->Get_Mouse_Y() - m_YPos) / m_YSpacing;
        m_CurrentIndex = std::min(m_CurrentIndex, m_Entries.Count() - 1);

        if (m_CurrentIndex == -1) {
            m_CurrentIndex = 0;
        }
    }

    return ControlClass::Action(flags, key);
}

int ListClass::Add_Item(int str_id)
{
    if (str_id > 0) {
        Add_Item(Text_String((TextEnum)str_id));
    }

    return Count() - 1;
}

int ListClass::Add_Item(const char *string)
{
    if (string) {
        m_Entries.Add(string);
        Flag_To_Redraw();

        if (Count() > m_ThumbSize) {
            Add_Scroll_Bar();
        }

        if (m_HasScrollbar) {
            m_Scrollbar.Set_Maximum(Count());
        }
    }

    return Count() - 1;
}

BOOL ListClass::Add_Scroll_Bar()
{
    // Does this list instance have a scroll bar?
    // If not, lets add it!
    if (!m_HasScrollbar) {
        m_HasScrollbar = true;

        // Set the flag to redraw the gadget.
        Flag_To_Redraw();

        // Recalculate the list width to exclude the soon to be
        // added scroll bar's width.
        m_Width -= m_Scrollbar.Get_Width();
        m_ScrollUpButton.Make_Peer(*this);
        m_ScrollDownButton.Make_Peer(*this);
        m_Scrollbar.Make_Peer(*this);
        m_ScrollUpButton.Add(*this);
        m_ScrollDownButton.Add(*this);
        m_Scrollbar.Add(*this);

        // Set the redraw flag on the scroll bar gadgets.
        m_ScrollUpButton.Flag_To_Redraw();
        m_ScrollDownButton.Flag_To_Redraw();
        m_Scrollbar.Flag_To_Redraw();

        m_Scrollbar.Set_Maximum(Count());
        m_Scrollbar.Set_Thumb_Size(m_ThumbSize);
        m_Scrollbar.Set_Value(m_ViewIndex);

        // We sucessfully added the scroll bar!
        return true;
    }

    // I already have a scroll bar, so reflect this by returning false.
    return false;
}

void ListClass::Bump(BOOL bump_up)
{
    // the code actually does use Step(), is this a error? needs testing and maybe changing to Bump()
    if (m_HasScrollbar && m_Scrollbar.Step(bump_up)) {
        m_ViewIndex = m_Scrollbar.Get_Value();
        Flag_To_Redraw();
    }
}

int ListClass::Count() const
{
    // Return the number of active entries in the vector array.
    return m_Entries.Count();
}

int ListClass::Current_Index() const
{
    return m_CurrentIndex;
}

const char *ListClass::Current_Item() const
{
    if (Count() > Current_Index()) {
        return m_Entries[Current_Index()];
    }

    return nullptr;
}

const char *ListClass::Get_Item(int string_index) const
{
    if (Count()) {
        return m_Entries[std::min(string_index, Count() - 1)];
    }

    return nullptr;
}

int ListClass::Step_Selected_Index(int index)
{
    int curr_idx = m_CurrentIndex;
    Set_Selected_Index(index + m_CurrentIndex);

    return curr_idx;
}

void ListClass::Remove_Item(int string_index)
{
    if (Count() > string_index) {
        m_Entries.Delete(string_index);

        if (Count() <= m_ThumbSize) {
            Remove_Scroll_Bar();
        }

        if (m_HasScrollbar) {
            m_Scrollbar.Set_Maximum(Count());
        }

        if (Count() <= Current_Index()) {
            --m_CurrentIndex;
            if (Current_Index() < 0) {
                m_CurrentIndex = 0;
            }
        }

        if (Count() <= m_ViewIndex) {
            --m_ViewIndex;

            if (m_ViewIndex < 0) {
                m_ViewIndex = 0;
            }

            if (m_HasScrollbar) {
                m_Scrollbar.Step(true);
            }
        }
    }
}

void ListClass::Remove_Item(const char *string)
{
    int id = m_Entries.ID(string);

    if (string && id != -1) {
        Remove_Item(id);
    }
}

BOOL ListClass::Remove_Scroll_Bar()
{
    // Does this list instance have a scroll bar?
    // If so, lets remove it!
    if (m_HasScrollbar) {
        // Recalculate the list width to include the soon to be
        // removed scroll bar's width.
        m_Width += m_Scrollbar.Get_Width();

        // Remove the attached scroll bar.
        m_Scrollbar.Remove();
        m_HasScrollbar = false;

        // Remove the attached scroll control buttons.
        m_ScrollUpButton.Remove();
        m_ScrollDownButton.Remove();

        // Set the redraw flag on the list gadget.
        Flag_To_Redraw();

        // We sucessfully removed the scroll bar!
        return true;
    }

    // I don't have a scroll bar, so reflect this by returning false.
    return false;
}

void ListClass::Set_Selected_Index(const char *string)
{
    if (string && Count() > 0) {
        for (int index = 0; index < Count(); ++index) {
            if (strcasecmp(m_Entries[index], string) == 0) {
                Set_Selected_Index(index);
                return;
            }
        }
    }
}

void ListClass::Set_Selected_Index(int string_index)
{
    if ((unsigned)string_index >= (unsigned)m_Entries.Count()) {
        m_CurrentIndex = 0;
    } else {
        m_CurrentIndex = string_index;
        Flag_To_Redraw();

        if (m_CurrentIndex < m_ViewIndex) {
            Set_View_Index(m_CurrentIndex);
        }

        if (m_ThumbSize + m_ViewIndex <= m_CurrentIndex) {
            Set_View_Index(m_CurrentIndex - (m_ThumbSize - 1));
        }
    }
}

void ListClass::Set_Tabs(int *tab_list)
{
    m_Tabs = tab_list;
}

BOOL ListClass::Set_View_Index(int index)
{
    int new_index = std::clamp(index, 0, std::max(m_Entries.Count() - m_ThumbSize, 0));

    if (new_index == m_ViewIndex) {
        return false;
    }

    m_ViewIndex = new_index;
    Flag_To_Redraw();

    if (m_HasScrollbar) {
        m_Scrollbar.Set_Value(m_ViewIndex);
    }

    return true;
}

void ListClass::Step(BOOL step_up)
{
    if (m_HasScrollbar && m_Scrollbar.Step(step_up)) {
        m_ViewIndex = m_Scrollbar.Get_Value();
        Flag_To_Redraw();
    }
}

void ListClass::Draw_Entry(int index, int x, int y, int x_max, BOOL redraw)
{
    int style = m_TextStyle;

    if (redraw) {
        style |= TPF_USE_BRIGHT;
        g_LogicPage->Fill_Rect(x, y, x_max + x - 1, m_YSpacing + y - 1, GadgetClass::s_ColorScheme->WindowPalette[0]);
    } else {
        if (!(style & TPF_USE_GRAD_PAL)) {
            style |= TPF_USE_MEDIUM;
        }
    }

    Conquer_Clip_Text_Print(m_Entries[index], x, y, GadgetClass::s_ColorScheme, 0, (TextPrintType)style, x_max, m_Tabs);
}

void Clear_Listbox(ListClass *list)
{
    while (list->Count()) {
        const char *item = list->Get_Item(0);
        list->Remove_Item(item);
        delete[] item;
    }

    list->Flag_To_Redraw();
}

BOOL Is_Link_In_List(LinkClass *list, LinkClass *link)
{
    for (LinkClass *l = list; l != nullptr; l = l->Get_Next()) {
        if (l == link) {
            return true;
        }
    }

    return false;
}
