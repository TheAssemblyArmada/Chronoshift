/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Base of the IOMap hierachy responsible for handling message list drawing.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gscreen.h"
#include "bench.h"
#include "gadget.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "globals.h"
#include "mouse.h"
#include "msglist.h"
#include "session.h"
#include "iomap.h"

#ifndef GAME_DLL
GadgetClass *GameScreenClass::g_Buttons = nullptr;
GraphicViewPortClass *GameScreenClass::g_ShadowPage = nullptr;
#endif

GameScreenClass::GameScreenClass() :
    m_RedrawFlag(REDRAW_FORCE | REDRAW_2)
{
}

GameScreenClass::~GameScreenClass()
{
}

void GameScreenClass::One_Time()
{
    g_Buttons = nullptr;
    g_ShadowPage = new GraphicBufferClass(320, 200, nullptr);
    g_ShadowPage->Clear();
    g_HidPage.Clear();
}

void GameScreenClass::Init(TheaterType theater)
{
    Init_Clear();
    Init_IO();
    Init_Theater(theater);
}

void GameScreenClass::Init_Clear()
{
    if (g_ShadowPage != nullptr) {
        g_ShadowPage->Clear();
        g_HidPage.Clear();
    }

    m_RedrawFlag |= REDRAW_FORCE;
}

void GameScreenClass::Init_IO()
{
    g_Buttons = nullptr;
}

void GameScreenClass::Flag_To_Redraw(BOOL redraw)
{
    m_RedrawFlag |= REDRAW_2;

    if (redraw) {
        m_RedrawFlag |= REDRAW_FORCE;
    }
}

void GameScreenClass::Input(KeyNumType &key, int &mouse_x, int &mouse_y)
{
    key = KeyNumType(g_Keyboard->Check());
    mouse_x = g_Mouse->Get_Mouse_X();
    mouse_y = g_Mouse->Get_Mouse_Y();

    if (g_Buttons != nullptr) {
        if (g_Buttons->Is_List_To_Redraw()) {
            Flag_To_Redraw();
        }

        GraphicViewPortClass *old = Set_Logic_Page(g_HidPage);
        key = g_Buttons->Input();
        Set_Logic_Page(old);

    } else if (key != KN_NONE) {
        key = KeyNumType(g_Keyboard->Get());
    }

    AI(key, mouse_x, mouse_y);
}

void GameScreenClass::Add_A_Button(GadgetClass &gadget)
{
    if (&gadget == g_Buttons) {
        Remove_A_Button(gadget);
    } else {
        gadget.Remove();
    }

    if (g_Buttons != nullptr) {
        gadget.Add_Tail(*g_Buttons);
    } else {
        g_Buttons = &gadget;
    }
}

void GameScreenClass::Remove_A_Button(GadgetClass &gadget)
{
    g_Buttons = gadget.Remove();
}

// TODO: This function should probably be bumped up to DisplayClass as it relies on DisplayClass members.
void GameScreenClass::Render()
{
    // This if is only done in EDWIN, so editor only?
    if (g_InMapEditor && g_Buttons != nullptr && g_Buttons->Is_List_To_Redraw()) {
        m_RedrawFlag |= REDRAW_2;
    }

    if ((m_RedrawFlag & REDRAW_FORCE) || (m_RedrawFlag & REDRAW_2)) {

        BENCHMARK_START(BENCH_FULL_PROCESS);

        GraphicViewPortClass *old = Set_Logic_Page(g_HidPage);

        BOOL to_redraw = (m_RedrawFlag & REDRAW_FORCE) != 0;

        if (g_InMapEditor && to_redraw) {
            g_HidPage.Clear();
        }

        Draw_It(to_redraw);

        if (g_Buttons != nullptr) {
            g_Buttons->Draw_All(false);
        }


        if (!g_InMapEditor) {
            if (g_Session.Get_Messages().Num_Messages() > 0) {
                // Calculate the width of the message buffer based on the current tactical width.
                g_Session.Get_Messages().Set_Width(CELL_PIXELS * Lepton_To_Cell_Coord(g_Map.Get_DisplayWidth()));
            }

            g_Session.Get_Messages().Draw();
        }

        Blit_Display();

        m_RedrawFlag &= ~(REDRAW_FORCE | REDRAW_2); // Clear any redraw flags.

        BENCHMARK_END(BENCH_FULL_PROCESS);

        Set_Logic_Page(old);
    }
}

void GameScreenClass::Draw_It(BOOL force_redraw)
{
}

void GameScreenClass::Blit_Display()
{
    BENCHMARK_START(BENCH_BLIT);

    g_Mouse->Draw_Mouse(g_HidPage);

    g_HidPage.Blit(g_SeenBuff,
        g_HidPage.Get_XPos(),
        g_HidPage.Get_YPos(),
        g_SeenBuff.Get_XPos(),
        g_SeenBuff.Get_YPos(),
        g_HidPage.Get_Width(),
        g_HidPage.Get_Height());

    g_Mouse->Erase_Mouse(g_HidPage);

    BENCHMARK_END(BENCH_BLIT);
}

void GameScreenClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
}
