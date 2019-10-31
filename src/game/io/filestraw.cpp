/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw stream class implementation reading data from a file.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "filestraw.h"
#include "basefile.h"

FileStraw::~FileStraw()
{
    if (Valid_File() && m_HasOpened) {
        m_File->Close();
        m_HasOpened = false;
        m_File = nullptr;
    }
}

/**
 * @brief Reads data to the buffer from the underlying file class.
 */
int FileStraw::Get(void *buffer, int length)
{
    if (Valid_File() && buffer && length > 0) {
        if (!m_File->Is_Open()) {
            if (m_File->Is_Available()) {
                m_HasOpened = m_File->Open(FM_READ);
            }
        }

        return m_File->Read(buffer, length);
    }

    return 0;
}
