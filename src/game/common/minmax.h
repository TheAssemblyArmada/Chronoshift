/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief Template functions for clamping data to minimum and maximum bounds.
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

#ifndef MINMAX_H
#define MINMAX_H

#include "always.h"

template<typename T>
T Min(T minval, T maxval)
{
    return minval < maxval ? minval : maxval;
}

template<typename T>
T Max(T minval, T maxval)
{
    return maxval < minval ? minval : maxval;
}

template<class T>
T Clamp(T original, T minval, T maxval)
{
    if (original < minval) {
        return minval;
    }

    if (original > maxval) {
        return maxval;
    }

    return original;
};

#endif // MINMAX_H
