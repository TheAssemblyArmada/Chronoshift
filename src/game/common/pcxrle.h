/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief PCX RLE algorithm specialised for pipe/straw stream system.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef PCXRLE_H
#define PCXRLE_H

#include "always.h"

int PCX_Encode(void const **source, int srclength, void *dest, int destlength);
int PCX_Decode(void const **source, int srclength, void *dest, int destlength);

#endif // PCXRLE_H
