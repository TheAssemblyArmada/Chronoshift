/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for the remap color selector type UI controls.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "colrlist.h"
#include "gbuffer.h"

ColorListClass::ColorListClass(
    int id, int x, int y, int w, int h, TextPrintType text_style, void *up_btn_shape, void *down_btn_shape) :
    ListClass(id, x, y, w, h, text_style, up_btn_shape, down_btn_shape),
    m_RemapList(),
    m_SelectStyle(SELECT_1),
    m_RemapType(nullptr)
{
}

ColorListClass::ColorListClass(ColorListClass &that) :
    ListClass(that),
    m_RemapList(that.m_RemapList),
    m_SelectStyle(that.m_SelectStyle),
    m_RemapType(that.m_RemapType)
{
}

ColorListClass::~ColorListClass()
{
    m_RemapType = nullptr;
}

void ColorListClass::Remove_Item(const char *string)
{
    int index = m_Entries.ID(string);

    if (index != -1) {
        m_RemapList.Delete(index);
        Remove_Item(string);
    }
}

void ColorListClass::Draw_Entry(int index, int x, int y, int x_max, BOOL redraw)
{
    if (redraw) {
        RemapControlType *remapper = m_RemapType ? m_RemapType : m_RemapList[index];

        switch (m_SelectStyle) {
            case SELECT_0:
                Conquer_Clip_Text_Print(m_Entries[index], x, y, m_RemapList[index], 0, m_TextStyle, x_max, m_Tabs);
                break;

            case SELECT_1:
                if (m_TextStyle & TPF_6PT_GRAD) {
                    Conquer_Clip_Text_Print(m_Entries[index], x, y, remapper, 0, m_TextStyle | TPF_SHADOW, x_max, m_Tabs);
                } else {
                    Conquer_Clip_Text_Print(m_Entries[index], x, y, remapper, 0, m_TextStyle, x_max, m_Tabs);
                }
                break;

            case SELECT_2:
                g_logicPage->Draw_Rect(x, y, x_max + x - 2, m_YSpacing + y - 2, remapper->MediumColor);
                Conquer_Clip_Text_Print(m_Entries[index], x, y, m_RemapList[index], 0, m_TextStyle, x_max, m_Tabs);
                break;

            case SELECT_3:
                if (m_TextStyle & TPF_6PT_GRAD) {
                    g_logicPage->Fill_Rect(x, y, x_max + x - 2, m_YSpacing + y - 2, remapper->MediumColor);
                    Conquer_Clip_Text_Print(m_Entries[index], x, y, m_RemapList[index], 0, m_TextStyle | TPF_SHADOW, x_max, m_Tabs);
                } else {
                    g_logicPage->Fill_Rect(x, y, x_max + x - 2, m_YSpacing + y - 2, remapper->MediumColor);
                    Conquer_Clip_Text_Print(m_Entries[index], x, y, m_RemapList[index], 0, m_TextStyle, x_max, m_Tabs);
                }
                break;

            case SELECT_4:
                g_logicPage->Fill_Rect(x, y, x_max + x - 2, m_YSpacing + y - 2, remapper->MediumColor);
                break;

            default:
                break;
        }

    } else {
        Conquer_Clip_Text_Print(m_Entries[index], x, y, m_RemapList[index], 0, m_TextStyle, x_max, m_Tabs);
    }
}

int ColorListClass::Add_Item(const char *string, RemapControlType *remap)
{
    m_RemapList.Add(remap);

    return ListClass::Add_Item(string);
}

int ColorListClass::Add_Item(int str_id, RemapControlType *remap)
{
    m_RemapList.Add(remap);

    return ListClass::Add_Item(str_id);
}

void ColorListClass::Set_Selected_Style(SelectEnum select_style, RemapControlType *remap)
{
    m_SelectStyle = select_style;
    m_RemapType = remap;
}
