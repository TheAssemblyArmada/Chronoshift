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
GadgetClass *GameScreenClass::Buttons = nullptr;
GraphicViewPortClass *GameScreenClass::ShadowPage = nullptr;
#endif

GameScreenClass::GameScreenClass() :
    RedrawFlag(REDRAW_FORCE | REDRAW_2)
{
}

GameScreenClass::~GameScreenClass()
{
}

void GameScreenClass::One_Time()
{
    Buttons = nullptr;
    ShadowPage = new GraphicBufferClass(320, 200, nullptr);
    ShadowPage->Clear();
    g_hidPage.Clear();
}

void GameScreenClass::Init(TheaterType theater)
{
    Init_Clear();
    Init_IO();
    Init_Theater(theater);
}

void GameScreenClass::Init_Clear()
{
    if (ShadowPage != nullptr) {
        ShadowPage->Clear();
        g_hidPage.Clear();
    }

    RedrawFlag |= REDRAW_FORCE;
}

void GameScreenClass::Init_IO()
{
    Buttons = nullptr;
}

void GameScreenClass::Flag_To_Redraw(BOOL redraw)
{
    RedrawFlag |= REDRAW_2;

    if (redraw) {
        RedrawFlag |= REDRAW_FORCE;
    }
}

void GameScreenClass::Input(KeyNumType &key, int &mouse_x, int &mouse_y)
{
    key = KeyNumType(g_keyboard->Check());
    mouse_x = g_mouse->Get_Mouse_X();
    mouse_y = g_mouse->Get_Mouse_Y();

    if (Buttons != nullptr) {
        if (Buttons->Is_List_To_Redraw()) {
            Flag_To_Redraw();
        }

        GraphicViewPortClass *old = Set_Logic_Page(g_hidPage);
        key = Buttons->Input();
        Set_Logic_Page(old);

    } else if (key != KN_NONE) {
        key = KeyNumType(g_keyboard->Get());
    }

    AI(key, mouse_x, mouse_y);
}

void GameScreenClass::Add_A_Button(GadgetClass &gadget)
{
    if (&gadget == Buttons) {
        Remove_A_Button(gadget);
    } else {
        gadget.Remove();
    }

    if (Buttons != nullptr) {
        gadget.Add_Tail(*Buttons);
    } else {
        Buttons = &gadget;
    }
}

void GameScreenClass::Remove_A_Button(GadgetClass &gadget)
{
    Buttons = gadget.Remove();
}

// TODO: This function should probably be bumped up to DisplayClass as it relies on DisplayClass members.
void GameScreenClass::Render()
{
    // This if is only done in EDWIN, so editor only?
    if (g_InMapEditor && Buttons != nullptr && Buttons->Is_List_To_Redraw()) {
        RedrawFlag |= REDRAW_2;
    }

    if ((RedrawFlag & REDRAW_FORCE) || (RedrawFlag & REDRAW_2)) {

        BENCHMARK_START(BENCH_FULL_PROCESS);

        GraphicViewPortClass *old = Set_Logic_Page(g_hidPage);

        BOOL to_redraw = (RedrawFlag & REDRAW_FORCE) != 0;

        if (g_InMapEditor && to_redraw) {
            g_hidPage.Clear();
        }

        Draw_It(to_redraw);

        if (Buttons != nullptr) {
            Buttons->Draw_All(false);
        }


        if (!g_InMapEditor) {
            if (Session.Get_Messages().Num_Messages() > 0) {
                // Calculate the width of the message buffer based on the current tactical width.
                Session.Get_Messages().Set_Width(CELL_PIXELS * Lepton_To_Cell_Coord(Map.Get_DisplayWidth()));
            }

            Session.Get_Messages().Draw();
        }

        Blit_Display();

        RedrawFlag &= ~(REDRAW_FORCE | REDRAW_2); // Clear any redraw flags.

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

    g_mouse->Draw_Mouse(g_hidPage);

    g_hidPage.Blit(g_seenBuff,
        g_hidPage.Get_XPos(),
        g_hidPage.Get_YPos(),
        g_seenBuff.Get_XPos(),
        g_seenBuff.Get_YPos(),
        g_hidPage.Get_Width(),
        g_hidPage.Get_Height());

    g_mouse->Erase_Mouse(g_hidPage);

    BENCHMARK_END(BENCH_BLIT);
}

void GameScreenClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
}
