/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for getting and setting bits in an array of bytes.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "bitarray.h"
#include "gamedebug.h"

/**
 * @brief Sets or clears a bit at a given position in an arbitrary array of data.
 */
void Set_Bit(void *array, int bit, BOOL value)
{
    // Don't try and handle a negative value
    if (bit < 0) {
        return;
    }

    uint8_t *byte_array = (uint8_t *)array;

    if (value) {
        byte_array[bit / 8] |= 1 << (bit % 8);
    } else {
        byte_array[bit / 8] &= ~(1 << (bit % 8));
    }
}

/**
 * @brief Tests the value of a given bit in an array.
 */
BOOL Get_Bit(const void *array, int bit)
{
    // If negative it is out of range so can't be set
    if (bit < 0) {
        return false;
    }

    const uint8_t *byte_array = (const uint8_t *)array;

    return byte_array[bit / 8] & (1 << (bit % 8)) ? true : false;
}

/**
 * @brief Scans through the data and returns the position of the first set bit found.
 */
int First_True_Bit(const void *array, int size)
{
    const uint8_t *byte_array = (const uint8_t *)array;

    int bytenum = 0;
    int bitnum = 0;

    // Find the first none zero byte as it must contain the first bit.
    for (bytenum = 0; bytenum < size; ++bytenum) {
        if (byte_array[bytenum] != 0) {
            break;
        }
    }

    if (bytenum >= size) {
        return 8 * bytenum;
    }

    // Scan through the bits of the byte until we find the first set bit.
    for (bitnum = 0; bitnum < 8; ++bitnum) {
        if (Get_Bit(&byte_array[bytenum], bitnum)) {
            break;
        }
    }

    return 8 * bytenum + bitnum;
}

/**
 * @brief Scans through the data and returns the position of the first clear bit found.
 */
int First_False_Bit(const void *array, int size)
{
    const uint8_t *byte_array = (const uint8_t *)array;

    int bytenum = 0;
    int bitnum = 0;

    // Find the first byte with an unset bit as it must contain the first bit.
    for (bytenum = 0; bytenum < size; ++bytenum) {
        if (byte_array[bytenum] != 0xFF) {
            break;
        }
    }

    if (bytenum >= size) {
        return 8 * bytenum;
    }

    // Scan through the bits until we find one that isn't set.
    for (bitnum = 0; bitnum < 8; ++bitnum) {
        if (!Get_Bit(&byte_array[bytenum], bitnum)) {
            break;
        }
    }

    return 8 * bytenum + bitnum;
}
