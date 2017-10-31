/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Classes for resizable arrays ("vectors").
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "vector.h"
#include <string.h>

BooleanVectorClass::BooleanVectorClass(unsigned size, uint8_t *array) :
    BitCount(size),
    Copy(false),
    LastIndex(-1),
    BitArray(0, 0)
{
    BitArray.Resize(((size + (8 - 1)) / 8), array);
}

BooleanVectorClass::BooleanVectorClass(const BooleanVectorClass &vector)
{
    LastIndex = -1;
    *this = vector;
}

BooleanVectorClass &BooleanVectorClass::operator=(const BooleanVectorClass &vector)
{
    Fixup();
    Copy = vector.Copy;
    LastIndex = vector.LastIndex;
    BitArray = vector.BitArray;
    BitCount = vector.BitCount;

    return *this;
}

BOOL BooleanVectorClass::operator==(const BooleanVectorClass &vector) const
{
    Fixup(LastIndex);

    return BitCount == vector.BitCount && BitArray == vector.BitArray;
}

int BooleanVectorClass::Resize(unsigned size)
{
    Fixup();

    if (size > 0) {
        unsigned oldsize = BitCount;
        int success = BitArray.Resize(((size + (8 - 1)) / 8));
        BitCount = size;

        if (success && oldsize < size) {
            for (unsigned i = oldsize; i < size; i++) {
                (*this)[i] = 0;
            }
        }

        return success;
    }

    Clear();
    return 1;
}

void BooleanVectorClass::Clear()
{
    Fixup();
    BitCount = 0;
    BitArray.Clear();
}

void BooleanVectorClass::Reset()
{
    LastIndex = -1;

    if (BitArray.Length() > 0) {
        memset(&BitArray[0], 0, BitArray.Length());
    }
}

void BooleanVectorClass::Set()
{
    LastIndex = -1;

    if (BitArray.Length() > 0) {
        memset(&BitArray[0], 0xFF, BitArray.Length());
    }
}

void BooleanVectorClass::Fixup(int index) const
{
    if ((unsigned)index >= (unsigned)BitCount) {
        index = -1;
    }

    if (index != LastIndex) {
        if (LastIndex != -1) {
            DEBUG_ASSERT(unsigned(LastIndex) < unsigned(BitCount));
            Set_Bit((void *)&BitArray[0], LastIndex, Copy);
        }

        if (index != -1) {
            DEBUG_ASSERT(unsigned(index) < unsigned(BitCount));
            Copy = Get_Bit((void *)&BitArray[0], index);
        }

        LastIndex = index;
    }
}

void BooleanVectorClass::Init(unsigned size, uint8_t *array)
{
    Copy = false;
    LastIndex = -1;
    BitCount = size;
    BitArray.Resize(((size + (8 - 1)) / 8), array);
}

void BooleanVectorClass::Init(unsigned size)
{
    Copy = false;
    LastIndex = -1;
    BitCount = size;
    BitArray.Resize(((size + (8 - 1)) / 8));
}
