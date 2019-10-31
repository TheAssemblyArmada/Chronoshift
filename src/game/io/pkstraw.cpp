/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw that processes the data with RSA and Blowfish as it reads it.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "pkstraw.h"
#include "gamedebug.h"
#include "pk.h"
#include <algorithm>

PKStraw::PKStraw(StrawControl mode, Straw &rstraw) :
    m_ChangeKey(true),
    m_CryptRandom(rstraw),
    m_BlowFish(mode),
    m_Mode(mode),
    m_RSAKey(nullptr),
    m_EncryptedKeyLength(0),
    m_Carryover(0)
{
    Get_From(&m_BlowFish);
}

/**
* @brief Reads data to the provided buffer from chained straws, processing it with RSA and Blowfish.
*/
int PKStraw::Get(void *buffer, int length)
{
    DEBUG_ASSERT(buffer != nullptr);
    DEBUG_ASSERT(length > 0);

    char byte_buff[256];
    char *outbuff = static_cast<char *>(buffer);
    int getcount = 0;

    if (buffer == nullptr || length < 1 || m_RSAKey == nullptr) {
        Straw::Get(buffer, length);
    }

    // If we have a key change, then we need to get a new key from the
    // stream and decrypt it, or else generate one depending on the mode.
    if (m_ChangeKey) {
        if (m_Mode == STRAW_DECRYPT) {
            // Retrieve the cypher text version of the blowfish key from the
            // stream.
            int bytes_retrieved = Straw::Get(byte_buff, Encrypted_Key_Length());

            // If we didn't get a full keyblock as expected we can't
            // decrypt anymore of the stream, so print an error and abort.
            if (bytes_retrieved != Encrypted_Key_Length()) {
                DEBUG_LOG("PKStraw::Get() - Failed to retrieve file key!\n");
                return 0;
            }

            m_RSAKey->Decrypt(byte_buff, bytes_retrieved, m_CryptoBuffer);

            // Set Blowfish key on the underlying straw.
            m_BlowFish.Key(m_CryptoBuffer, BF_MAX_KEY_LENGTH);
        } else {
            // This generates the blowfish key for making a mix file header
            memset(byte_buff, 0, sizeof(byte_buff));
            m_CryptRandom.Get(byte_buff, BF_MAX_KEY_LENGTH);

            // Encrypt the generated key with RSA.
            m_Carryover = m_RSAKey->Encrypt(byte_buff, Plain_Key_Length(), m_CryptoBuffer);
            m_EncryptedKeyLength = m_Carryover;

            // Set Blowfish key on the underlying straw.
            m_BlowFish.Key(byte_buff, BF_MAX_KEY_LENGTH);
        }

        m_ChangeKey = false;
    }

    // If there is data waiting to be sent to the output buffer, send it now.
    if (m_Carryover > 0) {
        int tocopy = std::min(length, m_Carryover); // length >= m_carryOver ? m_carryOver : length;

        memmove(outbuff, &m_CryptoBuffer[m_EncryptedKeyLength - m_Carryover], tocopy);

        outbuff += tocopy;
        m_Carryover -= tocopy;
        length -= tocopy;
        getcount += tocopy;
    }

    return getcount + Straw::Get(outbuff, length);
}

/**
* @brief Set the straw that will provide data for processing.
*/
void PKStraw::Get_From(Straw *straw)
{
    m_BlowFish.Get_From(straw);
    Straw::Get_From(&m_BlowFish);
}

/**
* @brief Sets the RSA key object that will be used to encrypt/decrypt the blowfish key for this stream.
*/
void PKStraw::Key(PKey *key)
{
    m_RSAKey = key;

    // Check if the Key is valid so we can flag for a key change.
    if (m_RSAKey != nullptr) {
        m_ChangeKey = true;
    }

    // Reset Blowfish.
    m_EncryptedKeyLength = 0;
    m_Carryover = 0;
}

/**
* @brief Provides the length in bytes that the RSA key can encrypt in a single block.
*/
int PKStraw::Encrypted_Key_Length() const
{
    if (m_RSAKey) {
        return (((m_RSAKey->Get_Key_Length() - 1) / 8 + 1) * (55 / ((m_RSAKey->Get_Key_Length() - 1) / 8) + 1));
    }

    return 0;
}

/**
* @brief Provides the length in bytes that the RSA key can decrypt a single block to.
*/
int PKStraw::Plain_Key_Length() const
{
    if (m_RSAKey) {
        return ((m_RSAKey->Get_Key_Length() - 1) / 8 * (55 / ((m_RSAKey->Get_Key_Length() - 1) / 8) + 1));
    }

    return 0;
}
