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

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "always.h"
#include "aircrafttype.h"
#include "fly.h"
#include "foot.h"


class AircraftClass : public FootClass
{
public:
    AircraftClass(RTTIType type, int id, HousesType house);
    AircraftClass(const AircraftClass &that);
    AircraftClass(const NoInitClass &noinit);
    virtual ~AircraftClass();

    // ObjectClass
    virtual AircraftTypeClass &Class_Of() const override { return *m_Type; }

    AircraftType What_Type() const { return m_Type->What_Type(); }

private:
    FlyClass FlyControl;
    GamePtr<AircraftTypeClass> m_Type;
    FacingClass field_14A;
    void *field_14C;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Bit1 : 1; // 1
    BOOL m_Bit2 : 1; // 2
    BOOL m_Bit4 : 1; // 4
    BOOL m_Bit8 : 1; // 8
    BOOL m_Bit16 : 1; // 16
#else
    bool m_Bit1;
    bool m_Bit2;
    bool m_Bit4;
    bool m_Bit8;
    bool m_Bit16;
#endif
    uint8_t field_154;
    BasicTimerClass<FrameTimerClass> field_155;
    uint8_t field_15E;
};

#ifndef CHRONOSHIFT_STANDALONE
extern TFixedIHeapClass<AircraftClass> &g_Aircraft;
#else
extern TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

#endif // AIRCRAFT_H
