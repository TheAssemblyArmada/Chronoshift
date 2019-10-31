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
    m_ChangeKey(true),
    m_CryptRandom(rstraw),
    m_BlowFish(PIPE_ENCRYPT),
    m_Mode(mode),
    m_RSAKey(nullptr),
    m_EncryptedKeyLength(0),
    m_Carryover(0)
{
}

/**
 * @brief Writes data from the provided buffer to chained pipes, processing it with RSA and Blowfish.
 */
int PKPipe::Put(const void *buffer, int length)
{
    // If we don't have a valid RSA key, just pass the data through to the next
    // pipe unchanged.
    if (buffer == nullptr || length < 1 || m_RSAKey == nullptr) {
        return Pipe::Put(buffer, length);
    }

    const char *inbuff = static_cast<const char *>(buffer);
    int putcount = 0;
    char byte_buff[256];

    // If a key change has been flagged, we either need to generate and
    // encrypt a new Blowfish key or decrypt the existing one depending on
    // the mode we are using.
    if (m_ChangeKey) {
        if (m_Mode == PIPE_DECRYPT) {
            int to_copy = std::min(length, m_Carryover); // m_carryOver >= length ? length : m_carryOver;

            memcpy(&m_CryptoBuffer[m_EncryptedKeyLength - m_Carryover], inbuff, to_copy);
            inbuff += to_copy;

            // This can't be right?
            bool bytes_remain = m_Carryover == to_copy;

            length -= to_copy;
            m_Carryover -= to_copy;

            if (!bytes_remain) {
                return putcount + Pipe::Put(inbuff, length);
            }

            m_RSAKey->Decrypt(m_CryptoBuffer, m_EncryptedKeyLength, byte_buff);
        } else {
            memset(byte_buff, 0, sizeof(byte_buff));
            m_CryptRandom.Get(byte_buff, BF_MAX_KEY_LENGTH);

            putcount += Pipe::Put(m_CryptoBuffer, m_RSAKey->Encrypt(byte_buff, BF_MAX_KEY_LENGTH, m_CryptoBuffer));
        }

        // Set the Blowfish key and then flag we have done the key change.
        m_BlowFish.Key(byte_buff, BF_MAX_KEY_LENGTH);
        m_ChangeKey = false;
    }

    return putcount + Pipe::Put(inbuff, length);
}

/**
* @brief Set the pipe that will further handle processed data.
*/
void PKPipe::Put_To(Pipe *pipe)
{
    m_BlowFish.Put_To(pipe);
    Pipe::Put_To(&m_BlowFish);
}

/**
* @brief Sets the RSA key object that will be used to encrypt/decrypt the blowfish key for this stream.
*/
void PKPipe::Key(PKey *key)
{
    if (!m_RSAKey) {
        Flush();
        m_ChangeKey = false;
    }

    m_RSAKey = key;

    // Check if the Key is valid so we can flag for a key change.
    if (key) {
        m_ChangeKey = true;

        // Why is m_carryOver set like this? Its not used in the encryption code.
        if (m_Mode == PIPE_ENCRYPT) {
            m_Carryover = Encrypted_Key_Length();
        } else {
            m_Carryover = 1;
        }

        m_EncryptedKeyLength = m_Carryover;
    }
}

/**
* @brief Provides the length in bytes that the RSA key can encrypt in a single block.
*/
int PKPipe::Encrypted_Key_Length() const
{
    if (m_RSAKey) {
        return m_RSAKey->Cipher_Text_Block_Size() * (55 / m_RSAKey->Cipher_Text_Block_Size());
    }

    return 0;
}

/**
* @brief Provides the length in bytes that the RSA key can decrypt a single block to.
*/
int PKPipe::Plain_Key_Length() const
{
    if (m_RSAKey) {
        return m_RSAKey->Clear_Text_Block_Size() * (55 / m_RSAKey->Cipher_Text_Block_Size());
    }

    return 0;
}
