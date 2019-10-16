/**
 * @file
 *
 * @author CCHyper
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TRICOLOR_H
#define TRICOLOR_H

#include "always.h"
#include "gauge.h"

class TriColorGaugeClass : public GaugeClass
{
public:
    TriColorGaugeClass(unsigned id, int x, int y, int w, int h) : GaugeClass(id, x, y, w, h), m_RedLimit(0), m_YellowLimit(0)
    {
    }
    TriColorGaugeClass(TriColorGaugeClass &that) :
        GaugeClass(that), m_RedLimit(that.m_RedLimit), m_YellowLimit(that.m_YellowLimit)
    {
    }
    virtual ~TriColorGaugeClass(void) {}

    TriColorGaugeClass &operator=(TriColorGaugeClass &that);

    // GadgetClass
    virtual BOOL Draw_Me(BOOL redraw) override;

    virtual bool Set_Red_Limit(int value);
    virtual bool Set_Yellow_Limit(int value);

protected:
    int m_RedLimit;
    int m_YellowLimit;
};

#endif // TRICOLOR_H
