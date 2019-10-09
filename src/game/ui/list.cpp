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
    TextStyle(text_style),
    Tabs(nullptr),
    Entries(),
    YSpacing(0),
    ThumbSize(0),
    HasScrollbar(false),
    ScrollUpButton(0, up_btn_shape, x + w, y),
    ScrollDownButton(0, down_btn_shape, x + w, y + h),
    Scrollbar(0, x + w, y, 0, h, 1),
    CurrentIndex(0),
    ViewIndex(0)
{
    // set position
    ScrollUpButton.Set_XPos(ScrollUpButton.Get_XPos() - ScrollUpButton.Get_Width());
    ScrollDownButton.Set_XPos(ScrollDownButton.Get_XPos() - ScrollDownButton.Get_Width());
    ScrollDownButton.Set_YPos(ScrollDownButton.Get_YPos() - ScrollDownButton.Get_Height());

    Scrollbar.Set_XPos(Scrollbar.Get_XPos() - std::max(ScrollUpButton.Get_Width(), ScrollDownButton.Get_Width()));
    Scrollbar.Set_YPos(ScrollUpButton.Get_Height() + YPos);
    Scrollbar.Set_Height(Scrollbar.Get_Height() - ScrollDownButton.Get_Height() + ScrollUpButton.Get_Height());
    Scrollbar.Set_Width(std::max(ScrollUpButton.Get_Width(), ScrollDownButton.Get_Width()));

    // The nullptr Fancy_Text_Print sets the font pointer and the dimension globals
    // for the YSpacing and ThumbSize
    Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, TextStyle);
    YSpacing = g_fontYSpacing + g_fontHeight - 1;
    ThumbSize = (h - 1) / YSpacing;
}

ListClass::ListClass(ListClass &that) :
    ControlClass(that),
    TextStyle(that.TextStyle),
    Tabs(that.Tabs),
    Entries(that.Entries),
    YSpacing(that.YSpacing),
    ThumbSize(that.ThumbSize),
    HasScrollbar(that.HasScrollbar),
    ScrollUpButton(that.ScrollUpButton),
    ScrollDownButton(that.ScrollDownButton),
    Scrollbar(that.Scrollbar),
    CurrentIndex(that.CurrentIndex),
    ViewIndex(that.ViewIndex)
{
    ScrollUpButton.Make_Peer(*this);
    ScrollDownButton.Make_Peer(*this);
    Scrollbar.Make_Peer(*this);
}

ListClass::~ListClass()
{
    Remove_Scroll_Bar();
}

LinkClass &ListClass::Add(LinkClass &that)
{
    if (HasScrollbar) {
        ScrollUpButton.Add(that);
        ScrollDownButton.Add(that);
        Scrollbar.Add(that);
    }

    return LinkClass::Add(that);
}

LinkClass &ListClass::Add_Tail(LinkClass &that)
{
    LinkClass::Add_Tail(that);

    if (HasScrollbar) {
        ScrollUpButton.Add_Tail(that);
        ScrollDownButton.Add_Tail(that);
        Scrollbar.Add_Tail(that);
    }

    return Head_Of_List();
}

LinkClass &ListClass::Add_Head(LinkClass &that)
{
    if (HasScrollbar) {
        ScrollUpButton.Add_Head(that);
        ScrollDownButton.Add_Head(that);
        Scrollbar.Add_Head(that);
    }

    return LinkClass::Add_Head(that);
}

GadgetClass *ListClass::Remove()
{
    if (HasScrollbar) {
        ScrollUpButton.Remove();
        ScrollDownButton.Remove();
        Scrollbar.Remove();
    }

    return GadgetClass::Remove();
}

void ListClass::Flag_To_Redraw()
{
    if (HasScrollbar) {
        ScrollUpButton.Flag_To_Redraw();
        ScrollDownButton.Flag_To_Redraw();
        Scrollbar.Flag_To_Redraw();
    }

    GadgetClass::Flag_To_Redraw();
}

void ListClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    if (flags & MOUSE_LEFT_RLSE) {
        if (&ScrollUpButton == &peer) {
            Step(true); // step up
        }

        if (&ScrollDownButton == &peer) {
            Step(false); // step down
        }
    }

    if (&Scrollbar == &peer) {
        Set_View_Index(Scrollbar.Get_Value());
    }
}

void ListClass::Set_Position(int x, int y)
{
    // Set scroll up button position.
    ScrollUpButton.Set_XPos(x + Width - ScrollUpButton.Get_Width());
    ScrollUpButton.Set_YPos(y);

    // Set scroll down button position.
    ScrollDownButton.Set_XPos(x + Width - ScrollDownButton.Get_Width());
    ScrollDownButton.Set_YPos(y + Height - ScrollDownButton.Get_Height());

    // Set scroll bar position.
    Scrollbar.Set_XPos(x + Width - std::max(ScrollUpButton.Get_Width(), ScrollDownButton.Get_Width()));
    Scrollbar.Set_YPos(y + ScrollUpButton.Get_Height());
    Scrollbar.Set_Width(std::max(ScrollUpButton.Get_Width(), ScrollDownButton.Get_Width()));
    Scrollbar.Set_Height(Height - (ScrollDownButton.Get_Height() + ScrollUpButton.Get_Height()));
}

BOOL ListClass::Draw_Me(BOOL redraw)
{
    if (GadgetClass::Draw_Me(redraw)) {
        if (&g_seenBuff == g_logicPage) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, Width + XPos, Height + YPos);
        }

        Draw_Box(XPos, YPos, Width, Height, BOX_STYLE_4, 1);

        if (Entries.Count()) {
            for (int index = 0; index < ThumbSize; ++index) {
                if (Entries.Count() > index + ViewIndex) {
                    BOOL redraw_entry = index + ViewIndex == Current_Index();
                    Draw_Entry(index + ViewIndex, XPos + 1, YPos + index * YSpacing + 1, Width - 2, redraw_entry);
                }
            }
        }

        if (&g_seenBuff == g_logicPage) {
            g_mouse->Conditional_Show_Mouse();
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
        CurrentIndex = ViewIndex + (g_mouse->Get_Mouse_Y() - YPos) / YSpacing;
        CurrentIndex = std::min(CurrentIndex, Entries.Count() - 1);

        if (CurrentIndex == -1) {
            CurrentIndex = 0;
        }
    }

    return ControlClass::Action(flags, key);
}

int ListClass::Add_Item(int str_id)
{
    if (str_id) {
        Add_Item(Fetch_String((TextEnum)str_id));
    }

    return Count() - 1;
}

int ListClass::Add_Item(const char *string)
{
    if (string) {
        Entries.Add(string);
        Flag_To_Redraw();

        if (Count() > ThumbSize) {
            Add_Scroll_Bar();
        }

        if (HasScrollbar) {
            Scrollbar.Set_Maximum(Count());
        }
    }

    return Count() - 1;
}

BOOL ListClass::Add_Scroll_Bar()
{
    // Does this list instance have a scroll bar?
    // If not, lets add it!
    if (!HasScrollbar) {
        HasScrollbar = true;

        // Set the flag to redraw the gadget.
        Flag_To_Redraw();

        // Recalculate the list width to exclude the soon to be
        // added scroll bar's width.
        Width -= Scrollbar.Get_Width();
        ScrollUpButton.Make_Peer(*this);
        ScrollDownButton.Make_Peer(*this);
        Scrollbar.Make_Peer(*this);
        ScrollUpButton.Add(*this);
        ScrollDownButton.Add(*this);
        Scrollbar.Add(*this);

        // Set the redraw flag on the scroll bar gadgets.
        ScrollUpButton.Flag_To_Redraw();
        ScrollDownButton.Flag_To_Redraw();
        Scrollbar.Flag_To_Redraw();

        Scrollbar.Set_Maximum(Count());
        Scrollbar.Set_Thumb_Size(ThumbSize);
        Scrollbar.Set_Value(ViewIndex);

        // We sucessfully added the scroll bar!
        return true;
    }

    // I already have a scroll bar, so reflect this by returning false.
    return false;
}

void ListClass::Bump(BOOL bump_up)
{
    // the code actually does use Step(), is this a error? needs testing and maybe changing to Bump()
    if (HasScrollbar && Scrollbar.Step(bump_up)) {
        ViewIndex = Scrollbar.Get_Value();
        Flag_To_Redraw();
    }
}

int ListClass::Count() const
{
    // Return the number of active entries in the vector array.
    return Entries.Count();
}

int ListClass::Current_Index() const
{
    return CurrentIndex;
}

const char *ListClass::Current_Item() const
{
    if (Count() > Current_Index()) {
        return Entries[Current_Index()];
    }

    return nullptr;
}

const char *ListClass::Get_Item(int string_index) const
{
    if (Count()) {
        return Entries[std::min(string_index, Count() - 1)];
    }

    return nullptr;
}

int ListClass::Step_Selected_Index(int index)
{
    int curr_idx = CurrentIndex;
    Set_Selected_Index(index + CurrentIndex);

    return curr_idx;
}

void ListClass::Remove_Item(int string_index)
{
    if (Count() > string_index) {
        Entries.Delete(string_index);

        if (Count() <= ThumbSize) {
            Remove_Scroll_Bar();
        }

        if (HasScrollbar) {
            Scrollbar.Set_Maximum(Count());
        }

        if (Count() <= Current_Index()) {
            --CurrentIndex;
            if (Current_Index() < 0) {
                CurrentIndex = 0;
            }
        }

        if (Count() <= ViewIndex) {
            --ViewIndex;

            if (ViewIndex < 0) {
                ViewIndex = 0;
            }

            if (HasScrollbar) {
                Scrollbar.Step(true);
            }
        }
    }
}

void ListClass::Remove_Item(const char *string)
{
    int id = Entries.ID(string);

    if (string && id != -1) {
        Remove_Item(id);
    }
}

BOOL ListClass::Remove_Scroll_Bar()
{
    // Does this list instance have a scroll bar?
    // If so, lets remove it!
    if (HasScrollbar) {
        // Recalculate the list width to include the soon to be
        // removed scroll bar's width.
        Width += Scrollbar.Get_Width();

        // Remove the attached scroll bar.
        Scrollbar.Remove();
        HasScrollbar = false;

        // Remove the attached scroll control buttons.
        ScrollUpButton.Remove();
        ScrollDownButton.Remove();

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
            if (strcasecmp(Entries[index], string) == 0) {
                Set_Selected_Index(index);
                return;
            }
        }
    }
}

void ListClass::Set_Selected_Index(int string_index)
{
    if ((unsigned)string_index >= (unsigned)Entries.Count()) {
        CurrentIndex = 0;
    } else {
        CurrentIndex = string_index;
        Flag_To_Redraw();

        if (CurrentIndex < ViewIndex) {
            Set_View_Index(CurrentIndex);
        }

        if (ThumbSize + ViewIndex <= CurrentIndex) {
            Set_View_Index(CurrentIndex - (ThumbSize - 1));
        }
    }
}

void ListClass::Set_Tabs(int *tab_list)
{
    Tabs = tab_list;
}

BOOL ListClass::Set_View_Index(int index)
{
    int new_index = std::clamp(index, 0, std::max(Entries.Count() - ThumbSize, 0));

    if (new_index == ViewIndex) {
        return false;
    }

    ViewIndex = new_index;
    Flag_To_Redraw();

    if (HasScrollbar) {
        Scrollbar.Set_Value(ViewIndex);
    }

    return true;
}

void ListClass::Step(BOOL step_up)
{
    if (HasScrollbar && Scrollbar.Step(step_up)) {
        ViewIndex = Scrollbar.Get_Value();
        Flag_To_Redraw();
    }
}

void ListClass::Draw_Entry(int index, int x, int y, int x_max, BOOL redraw)
{
    int style = TextStyle;

    if (redraw) {
        style |= TPF_USE_BRIGHT;
        g_logicPage->Fill_Rect(x, y, x_max + x - 1, YSpacing + y - 1, GadgetClass::ColorScheme->WindowPalette[0]);
    } else {
        if (!(style & TPF_USE_GRAD_PAL)) {
            style |= TPF_USE_MEDIUM;
        }
    }

    Conquer_Clip_Text_Print(Entries[index], x, y, GadgetClass::ColorScheme, 0, (TextPrintType)style, x_max, Tabs);
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
