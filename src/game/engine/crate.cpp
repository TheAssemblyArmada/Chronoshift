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
#include "crate.h"
#include "cell.h"
#include "iomap.h"
#include "gamedebug.h"
#include "scenario.h"

#ifndef GAME_DLL
AnimType CrateClass::CrateAnims[CRATE_COUNT];
int CrateClass::CrateShares[CRATE_COUNT] = { 50, 20, 3, 1, 3, 5, 5, 20, 1, 1, 3, 10, 10, 10, 1, 1, 3, 5 };
int CrateClass::CrateData[CRATE_COUNT];
#endif

const char *CrateClass::CrateNames[] = { "Money",
    "Unit",
    "ParaBomb",
    "HealBase",
    "Cloak",
    "Explosion",
    "Napalm",
    "Squad",
    "Darkness",
    "Reveal",
    "Sonar",
    "Armor",
    "Speed",
    "Firepower",
    "ICBM",
    "TimeQuake",
    "Invulnerability",
    "ChronalVortex",
    nullptr };

BOOL CrateClass::Remove_It()
{
    if (m_Cell == -1) {
        return false;
    }

    Get_Crate(m_Cell);
    Init_Clear();

    return true;
}

BOOL CrateClass::Create_Crate(cell_t cell)
{
    // TODO Requires ScenarioClass and RuleClass to actuall implement the functions.
#ifdef GAME_DLL
    BOOL (*func)(const CrateClass *, cell_t) = reinterpret_cast<BOOL (*)(const CrateClass *, cell_t)>(0x004AC91C);
    return func(this, cell);
#elif 0
    Remove_It();

    if (Put_Crate(cell)) {
    }

    return false;
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

BOOL CrateClass::Put_Crate(cell_t &cell)
{
    // TODO Requires MapClass, CellClass and ScenarioClass to actuall implement the functions.
#ifdef GAME_DLL
    BOOL (*func)(cell_t &) = reinterpret_cast<BOOL (*)(cell_t &)>(0x004AC9B4);
    return func(cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

BOOL CrateClass::Get_Crate(cell_t cell)
{
    if (Map.In_Radar(cell)) {
        CellClass &cptr = Map[cell];
        OverlayType overlay = cptr.Get_Overlay();
        if (overlay == OVERLAY_WOOD_CRATE || overlay == OVERLAY_STEEL_CRATE || overlay == OVERLAY_WATER_WOOD_CRATE) {
            cptr.Set_Overlay(OVERLAY_NONE);
            cptr.Set_Overlay_Frame(0);
            cptr.Redraw_Objects();
            return true;
        }
    }
    return false;
}

CrateType CrateClass::From_Name(const char *name)
{
    if (name != nullptr) {
        for (CrateType i = CRATE_MONEY; i < CRATE_COUNT; ++i) {
            if (strcasecmp(name, CrateNames[i]) == 0) {
                return i;
            }
        }
    }

    return CRATE_MONEY;
}
