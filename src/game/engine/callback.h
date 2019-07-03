/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Callback handler to do per loop callback processing.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CALLBACK_H
#define CALLBACK_H

class PaletteClass;

void Call_Back();
void Call_Back_Delay(int a1);
void Cycle_Call_Back_Delay(int a1, PaletteClass &pal);
void IPX_Call_Back();

#endif
