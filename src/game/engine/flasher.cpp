/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles the processing for a flashing object.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "flasher.h"

/**
 * @brief Processes the flash logic and returns true if the flasher is currently active.
 */
BOOL FlasherClass::Process()
{
    if (m_Frames > 0) {
        --m_Frames;
        m_Flashed = false;

        // Check if we are on an odd frame and if so, set the status to flashed.
        if ((unsigned)m_Frames % 2) {
            m_Flashed = true;
        }

        return true;
    }

    return false;
}
