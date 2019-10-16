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
#include "dial8.h"
#include "dialog.h"
#include "gbuffer.h"
#include "mouse.h"
#include "remap.h"
#include <cstring>

using std::memcpy;
/**
 * @brief
 *
 * @address 0x004AD050
 */
Dial8Class::Dial8Class(unsigned id, int x, int y, int w, int h, DirType direction) :
    ControlClass(id, x, y, w, h, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE, true)
{
    m_Center.x = Width / 2 + XPos;
    m_Center.y = Height / 2 + YPos;

    // Set the initial direction.
    Set_Direction(direction);

    // Some integer maths to get offsets for points on a circle.
    int x_straight = 8 * w / 2 / 10;
    int y_straight = 8 * h / 2 / 10;
    int x_diag = 56 * w / 2 / 100;
    int y_diag = 56 * h / 2 / 100;

    // North
    m_CompassPoints[FACING_NORTH].x = m_Center.x;
    m_CompassPoints[FACING_NORTH].y = m_Center.y - y_straight;

    // North East
    m_CompassPoints[FACING_NORTH_EAST].x = x_diag + m_Center.x;
    m_CompassPoints[FACING_NORTH_EAST].y = m_Center.y - y_diag;

    // East
    m_CompassPoints[FACING_EAST].x = x_straight + m_Center.x;
    m_CompassPoints[FACING_EAST].y = m_Center.y;

    // South East
    m_CompassPoints[FACING_SOUTH_EAST].x = x_diag + m_Center.x;
    m_CompassPoints[FACING_SOUTH_EAST].y = y_diag + m_Center.y;

    // South
    m_CompassPoints[FACING_SOUTH].x = m_Center.x;
    m_CompassPoints[FACING_SOUTH].y = y_straight + m_Center.y;

    // South West
    m_CompassPoints[FACING_SOUTH_WEST].x = m_Center.x - x_diag;
    m_CompassPoints[FACING_SOUTH_WEST].y = y_diag + m_Center.y;

    // West
    m_CompassPoints[FACING_WEST].x = m_Center.x - x_straight;
    m_CompassPoints[FACING_WEST].y = m_Center.y;

    // North West
    m_CompassPoints[FACING_NORTH_WEST].x = m_Center.x - x_diag;
    m_CompassPoints[FACING_NORTH_WEST].y = m_Center.y - y_diag;

    // Some integer maths to get offsets for points on a different circle.
    x_straight = 6 * w / 2 / 10;
    y_straight = 6 * h / 2 / 10;
    x_diag = 42 * w / 2 / 100;
    y_diag = 42 * h / 2 / 100;

    // North
    m_LinePoints[FACING_NORTH].x = m_Center.x;
    m_LinePoints[FACING_NORTH].y = m_Center.y - y_straight;

    // North East
    m_LinePoints[FACING_NORTH_EAST].x = x_diag + m_Center.x;
    m_LinePoints[FACING_NORTH_EAST].y = m_Center.y - y_diag;

    // East
    m_LinePoints[FACING_EAST].x = x_straight + m_Center.x;
    m_LinePoints[FACING_EAST].y = m_Center.y;

    // South East
    m_LinePoints[FACING_SOUTH_EAST].x = x_diag + m_Center.x;
    m_LinePoints[FACING_SOUTH_EAST].y = y_diag + m_Center.y;

    // South
    m_LinePoints[FACING_SOUTH].x = m_Center.x;
    m_LinePoints[FACING_SOUTH].y = y_straight + m_Center.y;

    // South West
    m_LinePoints[FACING_SOUTH_WEST].x = m_Center.x - x_diag;
    m_LinePoints[FACING_SOUTH_WEST].y = y_diag + m_Center.y;

    // West
    m_LinePoints[FACING_WEST].x = m_Center.x - x_straight;
    m_LinePoints[FACING_WEST].y = m_Center.y;

    // North West
    m_LinePoints[FACING_NORTH_WEST].x = m_Center.x - x_diag;
    m_LinePoints[FACING_NORTH_WEST].y = m_Center.y - y_diag;
}

/**
 * @brief
 *
 */
Dial8Class::Dial8Class(Dial8Class &that) :
    ControlClass(that),
    m_Center(that.m_Center),
    m_Direction(that.m_Direction),
    m_Facing(that.m_Facing),
    m_LastFacing(that.m_LastFacing)
{
    memcpy(m_CompassPoints, that.m_CompassPoints, sizeof(m_CompassPoints));
    memcpy(m_LinePoints, that.m_LinePoints, sizeof(m_LinePoints));
}

/**
 * @brief
 *
 */
Dial8Class &Dial8Class::operator=(Dial8Class &that)
{
    if (this != &that) {
        ControlClass::operator=(that);
        m_Center = that.m_Center;
        memcpy(m_CompassPoints, that.m_CompassPoints, sizeof(m_CompassPoints));
        memcpy(m_LinePoints, that.m_LinePoints, sizeof(m_LinePoints));
        m_Direction = that.m_Direction;
        m_Facing = that.m_Facing;
        m_LastFacing = that.m_LastFacing;
    }

    return *this;
}

/**
 * @brief
 *
 * @address 0x004AD3EC
 */
BOOL Dial8Class::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Hide_Mouse();
        }

        // Draw bevelled box.
        Draw_Box(XPos, YPos, Width, Height, BOX_STYLE_0, true);

        // Draw 3x3 boxes at compass points.
        for (int i = 0; i < FACING_COUNT; ++i) {
            Draw_Box(m_CompassPoints[i].x - 1, m_CompassPoints[i].y - 1, 3, 3, BOX_STYLE_1, false);
        }

        g_logicPage->Draw_Line(m_Center.x + 1,
            m_Center.y + 1,
            m_LinePoints[m_Facing].x + 1,
            m_LinePoints[m_Facing].y + 1,
            GadgetClass::Get_Color_Scheme()->WindowPalette[0]);

        g_logicPage->Draw_Line(m_Center.x,
            m_Center.y,
            m_LinePoints[m_Facing].x,
            m_LinePoints[m_Facing].y,
            GadgetClass::Get_Color_Scheme()->WindowPalette[3]);

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Show_Mouse();
        }

        return true;
    }

    return false;
}

/**
 * @brief
 *
 * @address 0x004AD314
 */
BOOL Dial8Class::Action(unsigned flags, KeyNumType &key)
{
    static bool _is_sel = false;

    Sticky_Process(flags);
    if (flags & MOUSE_LEFT_PRESS) {
        _is_sel = true;
    }
    if (flags & MOUSE_LEFT_PRESS || flags & MOUSE_LEFT_HELD && _is_sel) {
        m_Direction = Desired_Facing8(m_Center.x, m_Center.y, g_mouse->Get_Mouse_X(), g_mouse->Get_Mouse_Y());
        m_Facing = Direction_To_Facing(m_Direction);

        if (m_Facing != m_LastFacing) {
            m_LastFacing = m_Facing;
            ControlClass::Action(flags, key);
        } else {
            key = KN_NONE;
            ControlClass::Action(INPUT_NONE, key);
        }
        return true;
    }
    if (flags & MOUSE_LEFT_RLSE) {
        key = KN_NONE;
        _is_sel = false;
    }
    return ControlClass::Action(INPUT_NONE, key);
}

/**
 * @brief
 *
 * @address 0x004AD5F8
 */
void Dial8Class::Set_Direction(DirType direction)
{
    // Set the initial direction.
    m_Direction = direction;

    // This converts desired DirType to a FacingType.
    m_Facing = Direction_To_Facing(direction);
    m_LastFacing = Direction_To_Facing(direction);

    Flag_To_Redraw();
}
