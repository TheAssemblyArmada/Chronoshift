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

#ifndef INFANTRY_H
#define INFANTRY_H

#include "always.h"
#include "foot.h"
#include "infantrytype.h"

enum FearType
{
    FEAR_NONE = -1,        // TODO, is this permanent state of fear?
    FEAR_MIN = 0,
    FEAR_5 = 5,
    FEAR_10 = 10,
    FEAR_100 = 100,
    FEAR_200 = 200,
    FEAR_MAX = 255
};

DEFINE_ENUMERATION_OPERATORS(FearType);

class InfantryClass : public FootClass
{
public:
    InfantryClass(InfantryType type, HousesType house);
    InfantryClass(const InfantryClass &that);
    InfantryClass(const NoInitClass &noinit);
    virtual ~InfantryClass();

    // ObjectClass
    virtual const InfantryTypeClass &Class_Of() const override { return *m_Class; }

    BOOL Is_Technician() { return m_Technician; }

    InfantryType What_Type() const { return m_Class->What_Type(); }

private:
    GamePtr<InfantryTypeClass> m_Class;
    DoType m_Doing;
    TCountDownTimerClass<FrameTimerClass> m_StokeTimer;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Technician : 1; // 1
    BOOL m_Stoked : 1; // 2
    BOOL m_Prone : 1; // 4
    BOOL m_Bit8 : 1; // 8
    BOOL m_Bit16 : 1; // 16
#else
    bool m_Technician;
    bool m_Stoked;
    bool m_Prone;
    bool m_Bit8;
    bool m_Bit16;
#endif
    unsigned char /*FearType*/ m_Fear;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<InfantryClass> &g_Infantry;
#else
extern TFixedIHeapClass<InfantryClass> g_Infantry;
#endif

#endif // INFANTRY_H
