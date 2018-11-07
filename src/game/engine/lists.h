/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Lists of coord adjustments for overlap and occupier calculations.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LISTS_H
#define LISTS_H

#include "always.h"

#define    LIST_START    INT16_MAX - 1
#define    LIST_END    INT16_MAX

//http://nyerguds.arsaneus-design.com/cnc95upd/inirules/grids.ini

void List_Copy(int16_t *dst, const int16_t *src, int size);


////////////////////////////////////////////////////////////////////////////////
//  Occupy Lists
////////////////////////////////////////////////////////////////////////////////
extern int16_t const ListFix[];
extern int16_t const ListWeap[];
extern int16_t const ListWestwood[];
extern int16_t const ListSPen[];
extern int16_t const ListFactory[];
extern int16_t const ListSilo[];
extern int16_t const ListCom[];

extern int16_t const List1[];
extern int16_t const List10[];
extern int16_t const List11[];
extern int16_t const List1111[];
extern int16_t const List1010[];
extern int16_t const List0010[];
extern int16_t const List0011[];
extern int16_t const List0111[];
extern int16_t const List1101[];
extern int16_t const List000110[];
extern int16_t const List111111[];
extern int16_t const List010110[];
extern int16_t const List110110[];
extern int16_t const List000111111[];
extern int16_t const List010111100[];
extern int16_t const List001011100110[];
extern int16_t const List000011101000[];


////////////////////////////////////////////////////////////////////////////////
//  Overlap Lists
////////////////////////////////////////////////////////////////////////////////
extern int16_t const OListCom[];
extern int16_t const OListTmpl[];
extern int16_t const OListFix[];
extern int16_t const OListWeap[];
extern int16_t const OListSPen[];
extern int16_t const OListWestwood[];
extern int16_t const OListSAM[];

extern int16_t const OList12[];
extern int16_t const OList01[];
extern int16_t const OList001[];
extern int16_t const OList1001[];
extern int16_t const OList1100[];
extern int16_t const OList1101[];
extern int16_t const OList0010[];
extern int16_t const OList1000[];
extern int16_t const OList101001[];
extern int16_t const OList110001[];
extern int16_t const OList101000011[];
extern int16_t const OList110000011001[];
extern int16_t const OList111000010110[];


////////////////////////////////////////////////////////////////////////////////
//  Exit Lists
////////////////////////////////////////////////////////////////////////////////
extern int16_t const ExitPyle[];
extern int16_t const ExitSub[];
extern int16_t const ExitWeap[];

#endif // LISTS_H
