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
    CrateClass() : Cell(-1) {}

    BOOL Remove_It();
    BOOL Create_Crate(cell_t cell);
    cell_t Get_Cell() { return Cell; }
    BOOL Timer_Expired() { return CrateTimer.Time() <= 0; }
    void Init_Clear()
    {
        Cell = -1;
        CrateTimer.Stop();
    }

    static BOOL Put_Crate(cell_t &cell);
    static BOOL Get_Crate(cell_t cell);
    static CrateType From_Name(const char *name);
    static const char *Name_From(CrateType crate) { return CrateNames[crate]; }

private:
    TCountDownTimerClass<FrameTimerClass> CrateTimer;
    cell_t Cell;

public:
    static const char *CrateNames[];
#ifdef GAME_DLL
    static char *CrateAnims; // TODO should be AnimType *CrateAnims[CRATE_COUNT];
    static int *CrateShares;
    static int *CrateData;
#else
    static char CrateAnims[CRATE_COUNT]; // TODO should be AnimType CrateAnims[CRATE_COUNT];
    static int CrateShares[CRATE_COUNT];
    static int CrateData[CRATE_COUNT];
#endif
};

#endif // CRATE_H
