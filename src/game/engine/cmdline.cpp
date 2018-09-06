/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Command line parser.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cmdline.h"
#include "globals.h"
#include "special.h"

/**
 * Parses the command line for any arguments we act upon.
 *
 * 0x004F5880
 */
BOOL Parse_Command_Line(int argc, char **argv)
{
    Special.Init();
    Whom = HOUSES_GOODGUY;
    g_inMapEditor = false;
    DebugUnshroud = false;

	// TODO decide what command line options we need to parse, original stuff might not make sense anymore.

    return true;
}
