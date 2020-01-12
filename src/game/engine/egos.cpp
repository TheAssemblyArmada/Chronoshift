/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class and set of functions that handle and draw developer credits.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "egos.h"
#include "textprint.h"
#include "gadget.h"
#include "gbuffer.h"

#ifndef GAME_DLL
DynamicVectorClass<EgoClass *> g_EgoList;
#endif

EgoClass::EgoClass(int xpos, int ypos, char *string, TextPrintType style) : m_XPos(xpos), m_YPos(ypos), m_TextStyle(style)
{
    m_String = new char[strlen(string) + 1];
    strcpy(m_String, string);
}

EgoClass::~EgoClass()
{
    delete[] m_String;
}

/**
 *
 *
 */
BOOL EgoClass::Scroll(int amount)
{
    m_YPos -= amount;
    return m_YPos < -20;
}

/**
 *
 *
 */
void EgoClass::Render()
{
    if (m_YPos < g_LogicPage->Get_Height() && m_YPos > -16) {
        Fancy_Text_Print(m_String, m_XPos, m_YPos, GadgetClass::Get_Color_Scheme(), COLOR_TBLACK, m_TextStyle);
    }
}

/**
 *
 *
 */
void EgoClass::Wipe(GraphicBufferClass *buffer)
{
    int width = String_Pixel_Width(m_String);
    int xpos = m_XPos;

    if (m_TextStyle & TPF_RIGHT) {
        xpos -= width;
    } else if (m_TextStyle & TPF_CENTER) {
        xpos -= width / 2;
    }

    buffer->Blit(*g_LogicPage, xpos - 1, m_YPos, xpos - 1, m_YPos, width + 2, 15);
}

void Slide_Show(int slide, int value)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004BB848, void, int, int);
    func(slide, value);
#endif
}

void Show_Who_Was_Responsible()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004BBD78, void);
    func();
#endif
}
