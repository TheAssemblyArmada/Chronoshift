/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for keeping track of various type counts and kills?.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "utracker.h"
#include "endiantype.h"
#include <cstring>

using std::memset;

UnitTrackerClass::UnitTrackerClass(int count)
{
    m_IsNetworkFormat = 0;
    m_Total = new unsigned int[count];
    m_Count = count;
    Clear_Unit_Total();
}

UnitTrackerClass::~UnitTrackerClass()
{
    delete[] m_Total;
}

void UnitTrackerClass::Increment_Unit_Total(int unit)
{
    ++m_Total[unit];
}

void UnitTrackerClass::Decrement_Unit_Total(int unit)
{
    --m_Total[unit];
}

unsigned int *UnitTrackerClass::Get_All_Totals()
{
    return m_Total;
}

void UnitTrackerClass::Clear_Unit_Total()
{
    memset(m_Total, 0, sizeof(*m_Total) * m_Count);
}

void UnitTrackerClass::To_Network_Format()
{
    if (!m_IsNetworkFormat) {
        for (int i = 0; i < m_Count; ++i) {
            m_Total[i] = htobe32(m_Total[i]);
        }
    }
    m_IsNetworkFormat = true;
}

void UnitTrackerClass::To_PC_Format()
{
    if (m_IsNetworkFormat) {
        for (int i = 0; i < m_Count; ++i) {
            m_Total[i] = be32toh(m_Total[i]);
        }
    }
    m_IsNetworkFormat = false;
}