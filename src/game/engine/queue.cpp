/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Message queue related functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "queue.h"

BOOL Queue_Options()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)() = reinterpret_cast<BOOL (*)()>(0x00528DCC);
    return func();
#else
    return false;
#endif
}
