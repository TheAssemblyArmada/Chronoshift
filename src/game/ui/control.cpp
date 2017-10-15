/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for UI elements that generate events themselves.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "control.h"

ControlClass::ControlClass(unsigned id, int x, int y, int w, int h, unsigned input_flag, BOOL is_sticky) :
    GadgetClass(x, y, w, h, input_flag, is_sticky),
    ID(id),
    Peer(nullptr)
{
}

ControlClass::ControlClass(ControlClass &that) : GadgetClass(that), ID(that.ID), Peer(that.Peer) {}

BOOL ControlClass::Draw_Me(BOOL redraw)
{
    if (Peer) {
        Peer->Draw_Me(false);
    }

    return GadgetClass::Draw_Me(redraw);
}

BOOL ControlClass::Action(unsigned flags, KeyNumType &key)
{
    if (flags) {
        if (ID) {
            key = (KeyNumType)(ID | KN_BUTTON);
        } else {
            key = KN_NONE;
        }
    }

    if (Peer) {
        Peer->Peer_To_Peer(flags, key, *this);
    }

    return GadgetClass::Action(flags, key);
}
