/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Callback handler to do per loop callback processing.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "callback.h"
#include "theme.h"
#include "vox.h"
#include "session.h"

void IPX_Call_Back()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)() = reinterpret_cast<void (*)()>(0x004A7800);
    func();
#endif
}

void Call_Back()
{
    Theme.AI();
    Speak_AI();

    if (Session.Game_To_Play() == GAME_IPX || Session.Game_To_Play() == GAME_INTERNET) {
        IPX_Call_Back();
    }

    if ((Session.Game_To_Play() == GAME_1 || Session.Game_To_Play() == GAME_2) && Session.Modem_Service()) {
        // NullModem.Service();
    }

    /*if (g_pWolapi != nullptr) {
        // We won't be supporting WolAPI.
    }*/
}
