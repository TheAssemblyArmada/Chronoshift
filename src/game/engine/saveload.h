/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Functions used for Saving and Loading savegames.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SAVELOAD_H
#define SAVELOAD_H

#include "always.h"
#include "gametypes.h"

class Pipe;
class Straw;

enum
{
    //DOS
    RADOS_10X_MAGIC_ID = 0x10054DA, // 1.0X DOS, Retail
    RADOS_20X_MAGIC_ID = 0x10054DB, // 2.0X DOS
    //Windows 95
    RA95_10X_MAGIC_ID = 0x100616D, // 1.0X, Retail
    RA95_20X_MAGIC_ID = 0x100616E, // 2.0X, RA DEMO 1.19
    RA95_105_MAGIC_ID = 0x1006175, // 1.05, Special build for TEN MP Network
    RA95_2XX_MAGIC_ID = 0x100618A, // unknown, RA 3.0X checks for it
    RA95_30X_MAGIC_ID = 0x100618B, // 3.0X
};

void Put_All(Pipe &pipe, int skip_callback);
int Save_Game(int number, char *savename, int unknown);
int Load_Game(int unknown);
int Save_Misc_Values(Pipe &pipe);
int Load_Misc_Values(Straw &straw);
int Save_MPlayer_Values(Pipe &pipe);
int Load_MPlayer_Values(Straw &straw);
void Code_All_Pointers();
void Decode_All_Pointers();
int Get_Savefile_Info(int number, char *savename, unsigned *scenarioidx, HousesType *house);
int Reconcile_Players();
void MPlayer_Save_Message();

#endif // SAVELOAD_H
