/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief String table and text obfuscation functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef DIPTHONG_H
#define DIPTHONG_H

#include "always.h"

extern const char TXT_STRING_ERROR[];

void Fixup_Text(const char *source, char *dest);
int Dip_Text(const char *source, char *dest);
int UnDip_Text(const char *source, char *dest);
const char *Extract_String(char *file, int str_id);

#endif // DIPTHONG_H
