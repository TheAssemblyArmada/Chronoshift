/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for UI elements that generate events themselves.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CONTROLC_H
#define CONTROLC_H

#include "always.h"
#include "gadget.h"

class ControlClass : public GadgetClass
{
public:
    ControlClass(unsigned input_flag, BOOL sticky = false) : GadgetClass(input_flag, sticky), m_ID(0), m_Peer(nullptr) {}
    ControlClass(unsigned id, int x, int y, int w, int h, unsigned input_flag, BOOL is_sticky = false);
    ControlClass(ControlClass &that);
    virtual ~ControlClass() {}

    virtual unsigned Get_ID() const override { return m_ID; }
    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    virtual void Make_Peer(GadgetClass const &peer) { m_Peer = (GadgetClass *)&peer; }

    ControlClass &operator=(ControlClass &that);

    void Set_ID(unsigned id) { m_ID = id; }

protected:
    unsigned m_ID;
    GadgetClass *m_Peer;
};

inline ControlClass &ControlClass::operator=(ControlClass &that)
{
    if (this != &that) {
        GadgetClass::operator=(that);
        m_ID = that.m_ID;
        m_Peer = that.m_Peer;
    }

    return *this;
}

#endif // CONTROLC_H
