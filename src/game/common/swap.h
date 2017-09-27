/**
* @file
*
* @author OmniBlade
*
* @brief Utility function to swap two variables.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef SWAP_H
#define SWAP_H

#include	"always.h"

/**
 * @brief Swaps the values held by two variables.
 */
template<class T>
void Swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

#endif // SWAP_H
