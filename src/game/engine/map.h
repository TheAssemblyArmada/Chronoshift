/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling map loading and logic.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MAP_H
#define MAP_H

#include "always.h"
#include "cell.h"
#include "crate.h"
#include "gscreen.h"
#include "object.h"
#include "pipe.h"
#include "straw.h"
#include "vector.h"

class MapClass : public GameScreenClass
{
public:
    MapClass();

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void Alloc_Cells() override;
    virtual void Free_Cells() override;
    virtual void Init_Cells() override;
    virtual void Detach(ObjectClass *object) override;
    virtual void Logic_AI() override;
    virtual void Set_Map_Dimensions(int x, int y, int w, int h) override;
    virtual void Code_Pointers() override {}
    virtual void Decode_Pointers() override {}

    int16_t Cell_Number(CellClass *cell) { return Array.ID(cell); }
private:
    int MapCellX;
    int MapCellY;
    int MapCellWidth;
    int MapCellHeight;
    int TotalValue;
    VectorClass<CellClass> Array;
    int XSize;
    int YSize;
    int TotalSize;
    CrateClass Crates[256];
    int16_t TiberiumGrowth[64];
    int TiberiumGrowthCount;
    int TiberiumGrowthExcess;
    int16_t TiberiumSpread[64];
    int TiberiumSpreadCount;
    int TiberiumSpreadExcess;
    int16_t ShortValue_CheckForCellTotal;
};

#endif // MAP_H
