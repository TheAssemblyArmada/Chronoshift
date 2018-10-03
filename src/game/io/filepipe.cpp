/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe stream class implementation writing data to a file.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "filepipe.h"

FilePipe::~FilePipe()
{
    if (Valid_File() && m_hasOpened) {
        // Close the file if it is open.
        m_file->Close();

        // Reset members.
        m_hasOpened = false;
        m_file = nullptr;
    }
}

/**
* @brief Finishes writing any cached data from the stream chain and closes the file.
*/
int FilePipe::End()
{
    int retval = Flush();

    if (Valid_File() && m_hasOpened) {
        m_file->Close();
        m_hasOpened = false;
    }

    return retval;
}

/**
* @brief Writes data from the buffer to the underlying file class.
*/
int FilePipe::Put(const void *buffer, int length)
{
    if (Valid_File() && buffer != nullptr && length > 0) {
        if (!m_file->Is_Open()) {
            // If file doesn't exist open it?
            if (!m_file->Is_Available()) {
                m_file->Open(FM_WRITE);
            } else {
                // open it anyhow to overwrite?
                m_file->Open(FM_WRITE);
            }

            m_hasOpened = true;
        }

        return m_file->Write(buffer, length);
    }

    return 0;
}
