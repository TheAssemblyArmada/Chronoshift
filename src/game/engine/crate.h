/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles bonus crates.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CRATE_H
#define CRATE_H

#include "always.h"
#include "gametypes.h"
#include "ttimer.h"

enum CrateType
{
    CRATE_NONE = -1,
    CRATE_FIRST = 0,
    CRATE_MONEY = 0,
    CRATE_UNIT,
    CRATE_PARA_BOMB,
    CRATE_HEAL_BASE,
    CRATE_CLOAK,
    CRATE_EXPLOSION,
    CRATE_NAPALM,
    CRATE_SQUAD,
    CRATE_DARKNESS,
    CRATE_REVEAL,
    CRATE_SONAR,
    CRATE_ARMOR,
    CRATE_SPEED,
    CRATE_FIREPOWER,
    CRATE_ICBM,
    CRATE_TIME_QUAKE,
    CRATE_INVULNERABILITY,
    CRATE_CHRONALVORTEX,
    CRATE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(CrateType);

class CrateClass
{
public:
    CrateClass() : m_Cell(-1) {}

    BOOL Remove_It();
    BOOL Create_It(cell_t cell);
    cell_t Get_Cell() { return m_Cell; }
    BOOL Timer_Expired() { return m_CrateTimer.Time() <= 0; }
    void Init_Clear()
    {
        m_Cell = -1;
        m_CrateTimer.Stop();
    }

    static BOOL Put_Crate(cell_t &cell);
    static BOOL Get_Crate(cell_t cell);
    static CrateType From_Name(const char *name);
    static const char *Name_From(CrateType crate) { return s_CrateNames[crate]; }

private:
    TCountDownTimerClass<FrameTimerClass> m_CrateTimer;
    cell_t m_Cell;

public:
    static const char *s_CrateNames[];
#ifdef GAME_DLL
    static char *s_CrateAnims;
    static int *s_CrateShares;
    static int *s_CrateData;
#else
    static AnimType s_CrateAnims[CRATE_COUNT];
    static int s_CrateShares[CRATE_COUNT];
    static int s_CrateData[CRATE_COUNT];
#endif
};

#endif // CRATE_H
