/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief 
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef UNIT_H
#define UNIT_H

#include "always.h"
#include "foot.h"
#include "unittype.h"

class UnitClass : public FootClass
{
public:
    UnitClass(RTTIType type, int id, HousesType house);
    UnitClass(const UnitClass &that);
    UnitClass(const NoInitClass &noinit);
    virtual ~UnitClass();

    // ObjectClass
    virtual UnitTypeClass &Class_Of() const override { return *m_Type; }

    UnitType What_Type() const { return m_Type->What_Type(); }

private:
    GamePtr<UnitTypeClass> m_Type;
    HousesType m_FlagOwner;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_Bit1 : 1; // 1
            bool m_Bit2 : 1; // 2
            bool m_Bit4 : 1; // 4
            bool m_Bit8 : 1; // 8
            bool m_Bit16 : 1; // 16
    };
        int m_Bitfield;
};
#else
    bool m_Bit1;
    bool m_Bit2;
    bool m_Bit4;
    bool m_Bit8;
    bool m_Bit16;
#endif
    unsigned int m_BailCount;
    unsigned int m_GapGenCellTracker;
    cell_t m_GapGenCenterCell;
    TCountDownTimerClass<FrameTimerClass> m_V2RearmDelayTimer;
    FacingClass m_TurretFacing;
};

#ifndef CHRONOSHIFT_STANDALONE
extern TFixedIHeapClass<UnitClass> &g_Units;
#else
extern TFixedIHeapClass<UnitClass> g_Units;
#endif

#endif // UNIT_H
