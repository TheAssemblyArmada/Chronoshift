/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe stream class implementation writing data to a memory buffer.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "buffpipe.h"
#include <string.h>
#include <algorithm>

/**
* @brief Writes data from the provided buffer to the underlying buffer class.
*/
int BufferPipe::Put(const void *buffer, int length)
{
    if (!m_Buffer.Get_Buffer() || !buffer || length <= 0) {
        return 0;
    }

    int buff_size = m_Buffer.Get_Size();
    int size = length;

    if (buff_size) {
        buff_size -= m_Index;
        size = std::min(size, buff_size);
    }

    if (size > 0) {
        memmove(m_Buffer.Get_Buffer() + m_Index, buffer, size);
    }

    m_Index += size;

    return size;
}
