/**
 * @file
 *
 * @author tomsons26
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
#include "fly.h"
#include "coord.h"
#include "fixed.h"

/**
 *
 *
 */
ImpactType FlyClass::Physics(coord_t &coord, DirType dir)
{
    if (m_Speed != MPH_MIN) {
        int numer = m_Speed + m_field_0;
        div_t divval = div(numer, 10);
        m_field_0 = divval.rem;
        numer -= divval.rem;
        coord_t org_coord = coord;
        if (divval.quot != 0) {
            //speed check above changes on 0xffff :/
            coord_t moved_coord = Coord_Move(coord, dir, numer & 0xffff);
            if (moved_coord == coord) {
                return IMPACT_0;
            }
            coord = moved_coord;
            if (moved_coord & 0x80008000) {
                coord = org_coord;
                return IMPACT_2;
            }
            return IMPACT_1;
        }
    }
    return IMPACT_0;
}

/**
 *
 *
 */
void FlyClass::Fly_Speed(int speed, MPHType mph)
{
    m_Speed = mph * fixed_t(speed, 256);
}
