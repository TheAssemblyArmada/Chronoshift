/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for keeping track of various type counts and kills?.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "utracker.h"
#include "endiantype.h"

UnitTrackerClass::UnitTrackerClass(int count)
{
    IsNetworkFormat = 0;
    Total = new unsigned int[count];
    Count = count;
    Clear_Unit_Total();
}

UnitTrackerClass::~UnitTrackerClass()
{
    delete[] Total;
}

void UnitTrackerClass::Increment_Unit_Total(int unit)
{
    ++Total[unit];
}

void UnitTrackerClass::Decrement_Unit_Total(int unit)
{
    --Total[unit];
}

unsigned int *UnitTrackerClass::Get_All_Totals()
{
    return Total;
}

void UnitTrackerClass::Clear_Unit_Total()
{
    memset(Total, 0, sizeof(*Total) * Count);
}

void UnitTrackerClass::To_Network_Format()
{
    if (!IsNetworkFormat) {
        for (int i = 0; i < Count; ++i) {
            Total[i] = htobe32(Total[i]);
        }
    }
    IsNetworkFormat = true;
}

void UnitTrackerClass::To_PC_Format()
{
    if (IsNetworkFormat) {
        for (int i = 0; i < Count; ++i) {
            Total[i] = be32toh(Total[i]);
        }
    }
    IsNetworkFormat = false;
}