/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for getting and setting bits in an array of bytes.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BITARRAY_H
#define BITARRAY_H

#include	"always.h"

void Set_Bit(void *array, int bit, BOOL value);
BOOL Get_Bit(const void *array, int bit);
int First_True_Bit(const void *array, int size);
int First_False_Bit(const void *array, int size);

#endif // BITARRAY_H
