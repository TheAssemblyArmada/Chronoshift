/**
 * @file
 *
 * @author OmniBlade
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

#ifndef DIAL8_H
#define DIAL8_H

#include "always.h"
#include "controlc.h"
#include "facing.h"
#include "tpoint.h"

class Dial8Class : public ControlClass
{
public:
    Dial8Class(unsigned id, int x, int y, int w, int h, DirType direction);
    Dial8Class(Dial8Class &that);
    virtual ~Dial8Class(void) {}

    Dial8Class &operator=(Dial8Class &that);

    // GadgetClass
    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;

    void Set_Direction(DirType direction);
    DirType const Get_Direction() const { return m_Direction; }

protected:
    TPoint2D<int> m_Center;
    TPoint2D<int> m_CompassPoints[FACING_COUNT];
    TPoint2D<int> m_LinePoints[FACING_COUNT];
    DirType m_Direction;
    FacingType m_Facing;
    FacingType m_LastFacing;
};

#endif // DIAL8_H
