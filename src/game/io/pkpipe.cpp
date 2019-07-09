/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe that processes the data with RSA and Blowfish as it writes it.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "pkpipe.h"
#include "gamedebug.h"
#include "pk.h"
#include "straw.h"
#include <algorithm>

PKPipe::PKPipe(PipeControl mode, Straw &rstraw) :
    m_changeKey(true),
    m_cryptRandom(rstraw),
    m_blowFish(PIPE_ENCRYPT),
    m_mode(mode),
    m_rsaKey(nullptr),
    m_encryptedKeyLength(0),
    m_carryOver(0)
{
}

/**
 * @brief Writes data from the provided buffer to chained pipes, processing it with RSA and Blowfish.
 */
int PKPipe::Put(const void *buffer, int length)
{
    // If we don't have a valid RSA key, just pass the data through to the next
    // pipe unchanged.
    if (buffer == nullptr || length < 1 || m_rsaKey == nullptr) {
        return Pipe::Put(buffer, length);
    }

    const char *inbuff = static_cast<const char *>(buffer);
    int putcount = 0;
    char byte_buff[256];

    // If a key change has been flagged, we either need to generate and
    // encrypt a new Blowfish key or decrypt the existing one depending on
    // the mode we are using.
    if (m_changeKey) {
        if (m_mode == PIPE_DECRYPT) {
            int to_copy = std::min(length, m_carryOver); // m_carryOver >= length ? length : m_carryOver;

            memcpy(&m_cryptoBuffer[m_encryptedKeyLength - m_carryOver], inbuff, to_copy);
            inbuff += to_copy;

            // This can't be right?
            bool bytes_remain = m_carryOver == to_copy;

            length -= to_copy;
            m_carryOver -= to_copy;

            if (!bytes_remain) {
                return putcount + Pipe::Put(inbuff, length);
            }

            m_rsaKey->Decrypt(m_cryptoBuffer, m_encryptedKeyLength, byte_buff);
        } else {
            memset(byte_buff, 0, sizeof(byte_buff));
            m_cryptRandom.Get(byte_buff, BF_MAX_KEY_LENGTH);

            putcount += Pipe::Put(m_cryptoBuffer, m_rsaKey->Encrypt(byte_buff, BF_MAX_KEY_LENGTH, m_cryptoBuffer));
        }

        // Set the Blowfish key and then flag we have done the key change.
        m_blowFish.Key(byte_buff, BF_MAX_KEY_LENGTH);
        m_changeKey = false;
    }

    return putcount + Pipe::Put(inbuff, length);
}

/**
* @brief Set the pipe that will further handle processed data.
*/
void PKPipe::Put_To(Pipe *pipe)
{
    m_blowFish.Put_To(pipe);
    Pipe::Put_To(&m_blowFish);
}

/**
* @brief Sets the RSA key object that will be used to encrypt/decrypt the blowfish key for this stream.
*/
void PKPipe::Key(PKey *key)
{
    if (!m_rsaKey) {
        Flush();
        m_changeKey = false;
    }

    m_rsaKey = key;

    // Check if the Key is valid so we can flag for a key change.
    if (key) {
        m_changeKey = true;

        // Why is m_carryOver set like this? Its not used in the encryption code.
        if (m_mode == PIPE_ENCRYPT) {
            m_carryOver = Encrypted_Key_Length();
        } else {
            m_carryOver = 1;
        }

        m_encryptedKeyLength = m_carryOver;
    }
}

/**
* @brief Provides the length in bytes that the RSA key can encrypt in a single block.
*/
int PKPipe::Encrypted_Key_Length() const
{
    if (m_rsaKey) {
        return m_rsaKey->Cipher_Text_Block_Size() * (55 / m_rsaKey->Cipher_Text_Block_Size());
    }

    return 0;
}

/**
* @brief Provides the length in bytes that the RSA key can decrypt a single block to.
*/
int PKPipe::Plain_Key_Length() const
{
    if (m_rsaKey) {
        return m_rsaKey->Clear_Text_Block_Size() * (55 / m_rsaKey->Cipher_Text_Block_Size());
    }

    return 0;
}
