/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for keeping track of various type counts and kills,
 * collected crate count and other counts.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef UTRACKER_H
#define UTRACKER_H

#include "always.h"

class UnitTrackerClass
{
public:
    UnitTrackerClass(int count);
    ~UnitTrackerClass(); //dtor blank in YR
    void Increment_Unit_Total(int unit);
    void Decrement_Unit_Total(int unit);
    //New function in YR at 00749060 appears to populate unit count
    //New function in YR at 007490A0 appears to get populated unit count
    unsigned int *Get_All_Totals();
    void Clear_Unit_Total();
    void To_Network_Format();
    void To_PC_Format();

private:
    unsigned int *m_Total; //unsigned int Total[512]; in YR
    int m_Count;
    BOOL m_IsNetworkFormat;
};

#endif // UTRACKER_H