/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding information regarding the current game session.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "session.h"

#ifndef CHRONOSHIFT_STANDALONE
SessionClass &Session = Make_Global<SessionClass>(0x0067F2B4);
#else
SessionClass Session;
#endif

SessionClass::SessionClass()
{

}
