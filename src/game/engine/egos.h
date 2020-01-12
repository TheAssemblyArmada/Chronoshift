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
#pragma once

#ifndef EGOS_H
#define EGOS_H
#include "always.h"
#include "dialog.h"
#include "vector.h"

#ifdef GAME_DLL
#include "hooker.h"
#endif

class GraphicBufferClass;

class EgoClass
{
public:
    EgoClass(int xpos, int ypos, char *string, TextPrintType style);
    ~EgoClass();

    BOOL Scroll(int amount);
    void Render();
    void Wipe(GraphicBufferClass *buffer);

private:
    char *m_String;
    int m_XPos;
    int m_YPos;
    TextPrintType m_TextStyle;
};

#ifdef GAME_DLL
extern DynamicVectorClass<EgoClass *> &g_EgoList;
#else
extern DynamicVectorClass<EgoClass *> g_EgoList;
#endif

void Slide_Show(int slide, int value);
void Show_Who_Was_Responsible();

#endif //EGOS_H
