/**
 * @file
 *
 * @Author OmniBlade
 *
 * @brief Function for getting absolute of a signed value.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ABS_H
#define ABS_H

#include "always.h"

/**
 * @brief Returns the absolute value of 'x', if it is representable.
 */
template<typename T>
T Abs(T const x)
{
    return x < 0 ? (T)(-x) : (T)x;
};

#endif // ABS_H
