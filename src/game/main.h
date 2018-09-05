/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Entry point and associated low level init code.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MAIN_H
#define MAIN_H

#include "always.h"

#ifndef CHRONOSHIFT_STANDALONE
// This will eventually be replaced by a standard int main(int arc, char *argv[]) function
int __stdcall Main_Func(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// Watcom doesn't like main defined in a dll project it seems.
int not_main(int argc, char **argv);
#else
int main(int argc, char **argv);
#endif

#endif // MAIN_H