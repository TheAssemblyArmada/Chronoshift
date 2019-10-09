/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 * @author tomsons26
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
#pragma once

#ifndef TDROPLIST_H
#define TDROPLIST_H

#include "always.h"
#include "edit.h"
#include "tlist.h"
#include "shapebtn.h"
#include "dialog.h"
#include "drawshape.h"

template<typename T>
class TDropListClass : public EditClass
{
public:
    TDropListClass(int id, char *text_buff, int text_size, TextPrintType style, int x, int y, int w, int h,
        void *up_btn_shape, void *down_btn_shape);
    TDropListClass(TDropListClass<T> &that);
    virtual ~TDropListClass() {}

    TDropListClass &operator=(TDropListClass &that);

    // LinkClass
    virtual TDropListClass<T> &Add(LinkClass &link) override;
    virtual TDropListClass<T> &Add_Tail(LinkClass &link) override;
    virtual TDropListClass<T> &Add_Head(LinkClass &link) override;
    virtual void Zap() override;

    // GadgetClass
    virtual TDropListClass<T> *Remove() override;
    virtual void Flag_To_Redraw() override;
    virtual void Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer) override;
    virtual void Clear_Focus() override;
    virtual void Set_Position(int x, int y) override;

    virtual int Add_Item(T item);
    virtual T Current_Item() const;
    virtual int Current_Index();
    virtual void Set_Selected_Index(int index);
    virtual void Set_Selected_Index(T item);
    virtual int Count() const;
    virtual T Get_Item(int const string_index) const;

    void Expand();
    void Collapse();

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsExpanded : 1; // & 1
#else
    bool m_IsExpanded;
#endif
    int m_DropHeight;
    ShapeButtonClass m_DropButton;
    TListClass<T> m_DropList;
};

template<typename T>
TDropListClass<T> &TDropListClass<T>::operator=(TDropListClass<T> &that)
{
    if (this != &that) {
        EditClass::operator=(that);
        m_IsExpanded = that.m_IsExpanded;
        m_DropHeight = that.m_DropHeight;
        m_DropButton = that.m_DropButton;
        m_DropList = that.m_DropList;
        m_DropButton.Make_Peer(*this);
        m_DropList.Make_Peer(*this);
    }

    return *this;
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
TDropListClass<T>::TDropListClass(int id, char *text_buff, int text_size, TextPrintType style, int x, int y, int w, int h,
    void *up_btn_shape, void *down_btn_shape) :
    EditClass(id, text_buff, text_size, style, x, y, w, 18, EDIT_TEXT | EDIT_NUMS | EDIT_SYMS),
    m_IsExpanded(false),
    m_DropHeight(h),
    m_DropButton(0, down_btn_shape, w + x, y),
    m_DropList(0, x, y + Get_Build_Frame_Height(down_btn_shape), w + Get_Build_Frame_Width(down_btn_shape), h, style,
        up_btn_shape, down_btn_shape)
{
    Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, TextStyle); // EDWIN doesnt do this...
    m_DropButton.Make_Peer(*this);
    m_DropList.Make_Peer(*this);
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
TDropListClass<T>::TDropListClass(TDropListClass<T> &that) :
    EditClass(that),
    m_IsExpanded(that.m_IsExpanded),
    m_DropHeight(that.m_DropHeight),
    m_DropButton(that.m_DropButton),
    m_DropList(that.m_DropList)
{
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
TDropListClass<T> &TDropListClass<T>::Add(LinkClass &link)
{
    m_DropButton.Add(link);

    return reinterpret_cast<TDropListClass<T> &>(LinkClass::Add(link));
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
TDropListClass<T> &TDropListClass<T>::Add_Tail(LinkClass & link)
{
    m_DropButton.Add_Tail(link);

    return reinterpret_cast<TDropListClass<T> &>(LinkClass::Add_Tail(link));
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
TDropListClass<T> &TDropListClass<T>::Add_Head(LinkClass &link)
{
    m_DropButton.Add_Head(link);

    return reinterpret_cast<TDropListClass<T> &>(LinkClass::Add_Head(link));
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Zap()
{
    Collapse();
    m_DropList.Zap();
    m_DropButton.Zap();
    LinkClass::Zap();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
TDropListClass<T> *TDropListClass<T>::Remove()
{
    if (m_IsExpanded) {
        Collapse();
    }

    m_DropButton.Remove();

    return reinterpret_cast<TDropListClass<T> *>(GadgetClass::Remove());
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Flag_To_Redraw()
{
    if (m_IsExpanded) {
        m_DropList.Flag_To_Redraw();
    }

    GadgetClass::Flag_To_Redraw();
}

/**
 * @brief
 *
 * @info Method implementation is the differs from DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    if (&peer == &m_DropButton && flags & MOUSE_LEFT_RLSE) {
        if (m_IsExpanded) {
            Collapse();
            key = (KeyNumType)(ID | KN_BUTTON);
        } else {
            Expand();
        }
    }

    if (&peer == &m_DropList) {
        T item = m_DropList.Get_Item(Current_Index());
        strncpy(Text, item->Get_Name(), MaxTextLength);
        Flag_To_Redraw();
        key = (KeyNumType)(ID | KN_BUTTON);
    }
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Clear_Focus()
{
    Collapse();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Set_Position(int x, int y)
{
    GadgetClass::Set_Position(x, y);
    m_DropList.Set_Position(x, y + Get_Build_Frame_Height(m_DropButton.Get_Shape()));
    m_DropButton.Set_Position(x + Width, y);
}

/**
 * @brief
 *
 * @info Method implementation is the differs from DropListClass
 *
 */
template<typename T>
int TDropListClass<T>::Add_Item(T item)
{
    T it = m_DropList.Get_Item(Current_Index());
    strncpy(Text, it->Get_Name(), MaxTextLength);
    Flag_To_Redraw();

    return m_DropList.Add_Item(it);
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
T TDropListClass<T>::Current_Item() const
{
    return m_DropList.Current_Item();
}

/**
 * @brief
 *
 * @info Method implementation is the differs from DropListClass
 *
 */
template<typename T>
int TDropListClass<T>::Current_Index()
{
    return m_DropList.Current_Index();
}

//different
template<typename T>
void TDropListClass<T>::Set_Selected_Index(int index)
{
    if (index > m_DropList.Count()) {
        *Text = '\0';
    } else {
        m_DropList.Set_Selected_Index(index);
        T item = m_DropList.Get_Item(Current_Index());
        strncpy(Text, item->Get_Name(), MaxTextLength);
    }
}

/**
 * @brief
 *
 * @info Method implementation is the differs from DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Set_Selected_Index(T item)
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
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
int TDropListClass<T>::Count() const
{
    return m_DropList.Count();
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
T TDropListClass<T>::Get_Item(int const index) const
{
    return m_DropList.Get_Item(index);
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Expand()
{
    if (!m_IsExpanded) {
        m_DropList.Set_XPos(XPos);
        m_DropList.Set_YPos(YPos + 18);
        m_DropList.Set_Width(Width);
        m_DropList.Set_Height(m_DropHeight);
        m_DropList.Add(Head_Of_List());
        m_DropList.Flag_To_Redraw();
        m_IsExpanded = true;
    }
}

/**
 * @brief
 *
 * @info Method implementation is the same as used in DropListClass
 *
 */
template<typename T>
void TDropListClass<T>::Collapse()
{
    if (m_IsExpanded) {
        m_DropList.Remove();
        m_IsExpanded = false;
    }
}

#endif // TDROPLIST_H
