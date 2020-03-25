/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Classes for resizable arrays ("vectors").
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "vector.h"
#include <string.h>

BooleanVectorClass::BooleanVectorClass(unsigned size, uint8_t *array) :
    m_BitCount(size),
    m_Copy(false),
    m_LastIndex(-1),
    m_BitArray(0, 0)
{
    m_BitArray.Resize(((size + (8 - 1)) / 8), array);
}

BooleanVectorClass::BooleanVectorClass(const BooleanVectorClass &vector)
{
    m_LastIndex = -1;
    *this = vector;
}

BooleanVectorClass &BooleanVectorClass::operator=(const BooleanVectorClass &vector)
{
    Fixup();
    m_Copy = vector.m_Copy;
    m_LastIndex = vector.m_LastIndex;
    m_BitArray = vector.m_BitArray;
    m_BitCount = vector.m_BitCount;

    return *this;
}

BOOL BooleanVectorClass::operator==(const BooleanVectorClass &vector) const
{
    Fixup(m_LastIndex);

    return m_BitCount == vector.m_BitCount && m_BitArray == vector.m_BitArray;
}

int BooleanVectorClass::Resize(unsigned size)
{
    Fixup();

    if (size > 0) {
        unsigned oldsize = m_BitCount;
        int success = m_BitArray.Resize(((size + (8 - 1)) / 8));
        m_BitCount = size;

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
    m_BitCount = 0;
    m_BitArray.Clear();
}

void BooleanVectorClass::Reset()
{
    m_LastIndex = -1;

    if (m_BitArray.Length() > 0) {
        memset(&m_BitArray[0], 0, m_BitArray.Length());
    }
}

void BooleanVectorClass::Set()
{
    m_LastIndex = -1;

    if (m_BitArray.Length() > 0) {
        memset(&m_BitArray[0], 0xFF, m_BitArray.Length());
    }
}

void BooleanVectorClass::Fixup(int index) const
{
    if ((unsigned)index >= (unsigned)m_BitCount) {
        index = -1;
    }

    if (index != m_LastIndex) {
        if (m_LastIndex != -1) {
            captainslog_assert(unsigned(m_LastIndex) < unsigned(m_BitCount));
            Set_Bit((void *)&m_BitArray[0], m_LastIndex, m_Copy);
        }

        if (index != -1) {
            captainslog_assert(unsigned(index) < unsigned(m_BitCount));
            m_Copy = Get_Bit((void *)&m_BitArray[0], index);
        }

        m_LastIndex = index;
    }
}

void BooleanVectorClass::Init(unsigned size, uint8_t *array)
{
    m_Copy = false;
    m_LastIndex = -1;
    m_BitCount = size;
    m_BitArray.Resize(((size + (8 - 1)) / 8), array);
}

void BooleanVectorClass::Init(unsigned size)
{
    m_Copy = false;
    m_LastIndex = -1;
    m_BitCount = size;
    m_BitArray.Resize(((size + (8 - 1)) / 8));
}
