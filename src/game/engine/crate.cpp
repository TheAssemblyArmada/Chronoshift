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
#include "gametypes.h"
#include "globals.h"
#include "iomap.h"
#include "overlay.h"
#include "rules.h"
#include "scenario.h"

#ifndef GAME_DLL
AnimType CrateClass::s_CrateAnims[CRATE_COUNT];
int CrateClass::s_CrateShares[CRATE_COUNT] = { 50, 20, 3, 1, 3, 5, 5, 20, 1, 1, 3, 10, 10, 10, 1, 1, 3, 5 };
int CrateClass::s_CrateData[CRATE_COUNT];
#endif

const char *CrateClass::s_CrateNames[] = { "Money",
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

/**
 *
 *
 */
BOOL CrateClass::Remove_It()
{
    if (m_Cell == -1) {
        return false;
    }

    Get_Crate(m_Cell);
    m_Cell = -1;
    m_CrateTimer.Stop();
    return true;
}

/**
 *
 *
 */
BOOL CrateClass::Create_It(cell_t cell)
{
    //remove existing crate if there is one
    Remove_It();

    if (Put_Crate(cell)) {
        m_Cell = cell;
        m_CrateTimer = g_Scen.Get_Random_Value(450 * g_Rule.Crate_Regen(), 1800 * g_Rule.Crate_Regen());
        m_CrateTimer.Start();
        return true;
    }

    return false;
}

/**
 * Place crate on cell
 *
 */
BOOL CrateClass::Put_Crate(cell_t &cell)
{
    int stored_init = g_ScenarioInit;
    g_ScenarioInit = 0;

    if (!g_Map.In_Radar(cell)) {
        g_ScenarioInit = stored_init;
        return false;
    }

    CellClass *cptr = &g_Map[cell];
    if (cptr != nullptr && cptr->Get_Overlay() != OVERLAY_NONE) {
        while (cptr->Get_Overlay() != OVERLAY_NONE) {
            if (cptr->Is_Clear_To_Build(SPEED_FLOAT) || cptr->Is_Clear_To_Build(SPEED_FOOT)) {
                break;
            }
            cell = g_Map.Pick_Random_Location();
            int chance = 100 * g_Rule.Water_Crate_Chance();
            cell = g_Map.Nearby_Location(cell, (g_Scen.Get_Random_Value(0, 99) < chance) ? SPEED_FLOAT : SPEED_TRACK, -1);
            cptr = &g_Map[cell];
        }
    }
    OverlayClass *optr = new OverlayClass(cptr->Is_Clear_To_Build(SPEED_FLOAT) ? OVERLAY_WATER_WOOD_CRATE : OVERLAY_WOOD_CRATE, cell);
    DEBUG_ASSERT(optr != nullptr);

    g_ScenarioInit = stored_init;
    return true;
}

/**
 * "Pick up" crate from cell
 *
 */
BOOL CrateClass::Get_Crate(cell_t cell)
{
    if (g_Map.In_Radar(cell)) {
        CellClass &cptr = g_Map[cell];
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

/**
 *
 *
 */
CrateType CrateClass::From_Name(const char *name)
{
    if (name != nullptr) {
        for (CrateType i = CRATE_MONEY; i < CRATE_COUNT; ++i) {
            if (strcasecmp(name, s_CrateNames[i]) == 0) {
                return i;
            }
        }
    }

    return CRATE_MONEY;
}
