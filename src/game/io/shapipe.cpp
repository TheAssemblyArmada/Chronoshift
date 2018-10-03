/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Output stream implementation that hashes data with SHA1 as it passes it through.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "shapipe.h"

/**
* @brief Write data to the pipe from the specified memory buffer.
*
* @return Returns the number of bytes written from the buffer.
*/
int SHAPipe::Put(const void *buffer, int length)
{
    m_sha1.Hash(buffer, length);

    return Pipe::Put(buffer, length);
}
