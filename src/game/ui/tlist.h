/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 * @author tomsons26
 *
 * @brief Templated class for representing lists of selectable options.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TLIST_H
#define TLIST_H

#include "always.h"
#include "controlc.h"
#include "dialog.h"
#include "gbuffer.h"
#include "language.h"
#include "mouse.h"
#include "shapebtn.h"
#include "slider.h"
#include "textprint.h"
#include "vector.h"
#include <algorithm>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

template<typename T>
class TListClass : public ControlClass
{
public:
    TListClass(int id, int x, int y, int w, int h, TextPrintType text_style, void *up_btn_shape, void *down_btn_shape);
    TListClass(TListClass &that);
    virtual ~TListClass();

    TListClass &operator=(TListClass &that);

    // LinkClass
    virtual LinkClass &Add(LinkClass &that) override;
    virtual LinkClass &Add_Tail(LinkClass &that) override;
    virtual LinkClass &Add_Head(LinkClass &that) override;

    // GadgetClass
    virtual GadgetClass *Remove() override;
    virtual void Flag_To_Redraw() override;
    virtual void Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer) override;
    virtual void Set_Position(int x, int y) override;
    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;

    virtual int Add_Item(T item);
    virtual BOOL Add_Scroll_Bar();
    virtual void Insert_Item(T item);
    virtual void Bump(BOOL bump_up);
    virtual int Count() const;
    virtual int Current_Index() const;
    virtual T Current_Item() const;
    virtual int Step_Selected_Index(int index);
    virtual T Get_Item(int index) const;
    virtual void Remove_Item(T item);
    virtual void Remove_Index(int index);
    virtual BOOL Remove_Scroll_Bar();
    virtual void Set_Selected_Index(int index);
    virtual void Set_Selected_Index(T item);
    virtual void Set_m_Tabs(int *tab_list);
    virtual BOOL Set_View_Index(int index);
    virtual void Step(BOOL step_up);

protected:
    TextPrintType m_TextStyle;
    int *m_Tabs;
    DynamicVectorClass<T> m_Entries;
    int m_YSpacing; // horziontal line spacing.
    int m_ThumbSize; // this visible is a line count/page size.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_HasScrollbar : 1; // & 1
#else
    bool m_HasScrollbar; // Does this List have a scroll bar for scrolling through the entries?
#endif
    ShapeButtonClass m_ScrollUpButton; // Scroll Up button instance for the scroll bar.
    ShapeButtonClass m_ScrollDownButton; // Scroll Down button instance for the scroll bar.
    SliderClass m_Scrollbar; // Scroll bar instance for the list.
    int m_CurrentIndex;
    int m_ViewIndex;
};

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 * 
 */
template<typename T>
TListClass<T>::TListClass(
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
    m_Scrollbar.Set_YPos(m_ScrollUpButton.Get_Height() + YPos);
    m_Scrollbar.Set_Height(m_Scrollbar.Get_Height() - m_ScrollDownButton.Get_Height() + m_ScrollUpButton.Get_Height());
    m_Scrollbar.Set_Width(std::max(m_ScrollUpButton.Get_Width(), m_ScrollDownButton.Get_Width()));

    // The nullptr Fancy_Text_Print sets the font pointer and the dimension globals
    // for the m_YSpacing and m_ThumbSize
    Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, m_TextStyle);
    m_YSpacing = g_fontm_YSpacing + g_fontHeight - 1;
    m_ThumbSize = (h - 1) / m_YSpacing;
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
TListClass<T>::TListClass(TListClass<T> &that) :
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

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
TListClass<T>::~TListClass()
{
    Remove_Scroll_Bar();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
LinkClass &TListClass<T>::Add(LinkClass &that)
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Add(that);
        m_ScrollDownButton.Add(that);
        m_Scrollbar.Add(that);
    }

    return LinkClass::Add(that);
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
LinkClass &TListClass<T>::Add_Tail(LinkClass &that)
{
    LinkClass::Add_Tail(that);

    if (m_HasScrollbar) {
        m_ScrollUpButton.Add_Tail(that);
        m_ScrollDownButton.Add_Tail(that);
        m_Scrollbar.Add_Tail(that);
    }

    return Head_Of_List();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
LinkClass &TListClass<T>::Add_Head(LinkClass &that)
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Add_Head(that);
        m_ScrollDownButton.Add_Head(that);
        m_Scrollbar.Add_Head(that);
    }

    return LinkClass::Add_Head(that);
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
GadgetClass *TListClass<T>::Remove()
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Remove();
        m_ScrollDownButton.Remove();
        m_Scrollbar.Remove();
    }

    return GadgetClass::Remove();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Flag_To_Redraw()
{
    if (m_HasScrollbar) {
        m_ScrollUpButton.Flag_To_Redraw();
        m_ScrollDownButton.Flag_To_Redraw();
        m_Scrollbar.Flag_To_Redraw();
    }

    GadgetClass::Flag_To_Redraw();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
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

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Set_Position(int x, int y)
{
    // Set scroll up button position.
    m_ScrollUpButton.Set_XPos(x + Width - m_ScrollUpButton.Get_Width());
    m_ScrollUpButton.Set_YPos(y);

    // Set scroll down button position.
    m_ScrollDownButton.Set_XPos(x + Width - m_ScrollDownButton.Get_Width());
    m_ScrollDownButton.Set_YPos(y + Height - m_ScrollDownButton.Get_Height());

    // Set scroll bar position.
    m_Scrollbar.Set_XPos(x + Width - std::max(m_ScrollUpButton.Get_Width(), m_ScrollDownButton.Get_Width()));
    m_Scrollbar.Set_YPos(y + m_ScrollUpButton.Get_Height());
    m_Scrollbar.Set_Width(std::max(m_ScrollUpButton.Get_Width(), m_ScrollDownButton.Get_Width()));
    m_Scrollbar.Set_Height(Height - (m_ScrollDownButton.Get_Height() + m_ScrollUpButton.Get_Height()));
}

/**
 * @brief
 *
 * @info Method implementation is the differs from ListClass
 *
 */
template<typename T>
BOOL TListClass<T>::Draw_Me(BOOL redraw)
{
    if (GadgetClass::Draw_Me(redraw)) {
        if (&g_seenBuff == g_logicPage) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, Width + XPos, Height + YPos);
        }

        Draw_Box(XPos, YPos, Width, Height, BOX_STYLE_4, 1);

        if (m_Entries.Count()) {
            for (int index = 0; index < m_ThumbSize; ++index) {
                if (m_Entries.Count() > index + m_ViewIndex) {
                    BOOL redraw_entry = index + m_ViewIndex == Current_Index();
                    m_Entries[index + m_ViewIndex]->Draw_It(index + m_ViewIndex,
                        XPos + 1,
                        YPos + index * m_YSpacing + 1,
                        Width - 2,
                        m_YSpacing,
                        redraw_entry,
                        m_TextStyle);
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

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
BOOL TListClass<T>::Action(unsigned flags, KeyNumType &key)
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
        m_CurrentIndex = m_ViewIndex + (g_mouse->Get_Mouse_Y() - YPos) / m_YSpacing;
        m_CurrentIndex = std::min(m_CurrentIndex, m_Entries.Count() - 1);

        if (m_CurrentIndex == -1) {
            m_CurrentIndex = 0;
        }
    }

    return ControlClass::Action(flags, key);
}

/**
 * @brief
 *
 * @info Method implementation is the differs from ListClass
 *
 */
template<typename T>
int TListClass<T>::Add_Item(T item)
{
    m_Entries.Add(item);
    Flag_To_Redraw();
    if (Count() > m_ThumbSize) {
        Add_Scroll_Bar();
    }
    if (m_HasScrollbar) {
        m_Scrollbar.Set_Maximum(Count());
    }
    return Count() - 1;
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
BOOL TListClass<T>::Add_Scroll_Bar()
{
    // Does this list instance have a scroll bar?
    // If not, lets add it!
    if (!m_HasScrollbar) {
        m_HasScrollbar = true;

        // Set the flag to redraw the gadget.
        Flag_To_Redraw();

        // Recalculate the list width to exclude the soon to be
        // added scroll bar's width.
        Width -= m_Scrollbar.Get_Width();
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

//new function
template<typename T>
void TListClass<T>::Insert_Item(T item)
{
    //rewrite to use dvc Insert?
    if (Current_Index() >= Count()) {
        m_Entries.Add(item);
    } else {
        m_Entries.Add(item);
        for (int i = m_Entries.Count(); i >= Current_Index(); --i)
        {
            m_Entries[i + 1] = m_Entries[i];
        }
        m_Entries[Current_Index()] = item;
    }
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Bump(BOOL bump_up)
{
    // the code actually does use Step(), is this a error? needs testing and maybe changing to Bump()
    if (m_HasScrollbar && m_Scrollbar.Step(bump_up)) {
        m_ViewIndex = m_Scrollbar.Get_Value();
        Flag_To_Redraw();
    }
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
int TListClass<T>::Count() const
{
    // Return the number of active entries in the vector array.
    return m_Entries.Count();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
int TListClass<T>::Current_Index() const
{
    return m_CurrentIndex;
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
T TListClass<T>::Current_Item() const
{
    if (Count() > Current_Index()) {
        return m_Entries[Current_Index()];
    }

    return nullptr;
}

/**
 * @brief
 *
 * @info Method implementation is the differs from ListClass
 *
 */
template<typename T>
T TListClass<T>::Get_Item(int index) const
{
    return m_Entries[index];
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
int TListClass<T>::Step_Selected_Index(int index)
{
    int curr_idx = m_CurrentIndex;
    Set_Selected_Index(index + m_CurrentIndex);

    return curr_idx;
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Remove_Index(int index)
{
    if (Count() > index) {
        m_Entries.Delete(index);

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

/**
 * @brief
 *
 * @info Method implementation is the differs from ListClass
 *
 */
template<typename T>
void TListClass<T>::Remove_Item(T item)
{
    int id = m_Entries.ID(item);
    Remove_Index(id);
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
BOOL TListClass<T>::Remove_Scroll_Bar()
{
    // Does this list instance have a scroll bar?
    // If so, lets remove it!
    if (m_HasScrollbar) {
        // Recalculate the list width to include the soon to be
        // removed scroll bar's width.
        Width += m_Scrollbar.Get_Width();

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

/**
 * @brief
 *
 * @info Method implementation is the differs from ListClass
 *
 */
template<typename T>
void TListClass<T>::Set_Selected_Index(T item)
{
    for (int index = 0; index < Count(); ++index) {
        if (Get_Item(index) == item) {
            Set_Selected_Index(index);
            return;
        }
    }
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Set_Selected_Index(int index)
{
    if ((unsigned)index >= (unsigned)m_Entries.Count()) {
        m_CurrentIndex = 0;
    } else {
        m_CurrentIndex = index;
        Flag_To_Redraw();

        if (m_CurrentIndex < m_ViewIndex) {
            Set_View_Index(m_CurrentIndex);
        }

        if (m_ThumbSize + m_ViewIndex <= m_CurrentIndex) {
            Set_View_Index(m_CurrentIndex - (m_ThumbSize - 1));
        }
    }
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Set_m_Tabs(int *tab_list)
{
    m_Tabs = tab_list;
}

/**
 * @brief
 *
 * @info Method implementation is the rom ListClass
 *
 */
template<typename T>
BOOL TListClass<T>::Set_View_Index(int index)
{
    int new_index = std::clamp(index, 0, m_Entries.Count() - m_ThumbSize);

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

/**
 * @brief
 *
 * @info Method implementation is the same as used in ListClass
 *
 */
template<typename T>
void TListClass<T>::Step(BOOL step_up)
{
    if (m_HasScrollbar && m_Scrollbar.Step(step_up)) {
        m_ViewIndex = m_Scrollbar.Get_Value();
        Flag_To_Redraw();
    }
}

#endif // TLIST_H
