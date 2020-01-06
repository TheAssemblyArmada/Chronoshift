/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Fixed point multiplication implementation found in TD and still occasionally used in RA.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FIXEDMULT_H
#define FIXEDMULT_H

inline static int fixed_mult(int a, int b)
{
    int res = (a * b) + 128;

    if ( res & 0xFF000000 ){
        res = 0xFFFFFF;
    }

    return res >> 8;
}

#endif //FIXEDMULT_H
