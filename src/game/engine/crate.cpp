/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles bonus crates.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "crate.h"
#include "gamedebug.h"
#include "scenario.h"

#ifndef RAPP_STANDALONE
static char *CrateAnims = Make_Pointer<char>(0x005FF24C); // TODO should be AnimType *CrateAnims[CRATE_COUNT];
static int *CrateShares = Make_Pointer<int>(0x005FF204);
static int *CrateData = Make_Pointer<int>(0x00655DA8);
#else
static char CrateAnims[CRATE_COUNT]; // TODO should be AnimType CrateAnims[CRATE_COUNT];
static int CrateShares[CRATE_COUNT] = { 50, 20, 3, 1, 3, 5, 5, 20, 1, 1, 3, 10, 10, 10, 1, 1, 3, 5 };
static int CrateData[CRATE_COUNT];
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
    if (Cell == -1) {
        return false;
    }

    Get_Crate(Cell);
    Cell = -1;
    CrateTimer.Stop();

    return true;
}

BOOL CrateClass::Create_Crate(int16_t cell)
{
    // TODO Requires ScenarioClass and RuleClass to actuall implement the functions.
#ifndef RAPP_STANDALONE
    BOOL (*func)(const CrateClass *, int16_t) = reinterpret_cast<BOOL (*)(const CrateClass *, int16_t)>(0x004AC91C);
    return func(this, cell);
#elif 0
    Remove_It();

    if (Put_Crate(cell)) {
    }

    return false;
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called.\n");
    return false;
#endif
}

BOOL CrateClass::Put_Crate(int16_t &cell)
{
    // TODO Requires MapClass, CellClass and ScenarioClass to actuall implement the functions.
#ifndef RAPP_STANDALONE
    BOOL (*func)(int16_t &) = reinterpret_cast<BOOL (*)(int16_t &)>(0x004AC9B4);
    return func(cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called.\n");
    return false;
#endif
}

BOOL CrateClass::Get_Crate(int16_t cell)
{
    // TODO Requires MapClass, CellClass and ScenarioClass to actuall implement the functions.
#ifndef RAPP_STANDALONE
    BOOL (*func)(int16_t) = reinterpret_cast<BOOL (*)(int16_t)>(0x004ACB1C);
    return func(cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called.\n");
    return false;
#endif
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
