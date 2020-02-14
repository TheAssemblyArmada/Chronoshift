/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Input stream implementation that hashes data with SHA1 as it passes it through.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "shastraw.h"
#include <captainslog.h>

/**
 * @brief Read data from the straw into specified memory buffer.
 *
 * @return Returns the number of bytes read into the buffer.
 */
int SHAStraw::Get(void *buffer, int length)
{
    int retval = 0;

    if (buffer && length > 0) {
        retval = Straw::Get(buffer, length);
        m_SHA1.Hash(buffer, retval);
    }

    return retval;
}

/**
* @brief Read the SHA1 hash result into the specified memory buffer.
*
* @return Returns the number of bytes read into the buffer.
*/
int SHAStraw::Result(void *data)
{
    return m_SHA1.Result(data);
}
