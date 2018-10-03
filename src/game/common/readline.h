/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Reads a line of text from a file or straw.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef READLINE_H
#define READLINE_H

#include "always.h"
#include "fileclass.h"
#include "straw.h"

int Read_Line(FileClass &file, char *buffer, int length, BOOL &eof);
int Read_Line(Straw &straw, char *buffer, int length, BOOL &eof);

#endif // READLINE_H
