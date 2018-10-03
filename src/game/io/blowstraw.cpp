/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw stream class implementation reading data processed with the blowfish algorithm.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "blowstraw.h"
#include "gamedebug.h"
#include "minmax.h"

BlowStraw::~BlowStraw()
{
    if (m_blowFish != nullptr) {
        delete m_blowFish;
    }

    m_blowFish = nullptr;
}

/**
 * @brief Reads data to the buffer from the next straw in the chain, processing it with blowfish.
 */
int BlowStraw::Get(void *buffer, int length)
{
    if (buffer == nullptr || length <= 0) {
        return 0;
    }

    // If no Blowfish instance has been allocated, just pass through to the
    // normal behaviour of the chained Straw.
    if (m_blowFish != nullptr) {
        int getcount = 0;
        char *buf = static_cast<char *>(buffer);

        // Keep doing rounds of fetch and decrypt until the underlying Straw
        // cannot provide any more data, or we have retrieved the requested
        // amount of data.
        while (length > 0) {
            // If data was carried over from the last round, then just pass it
            // to the buffer as it was either a full block or the data source
            // can't retrieve any more data.
            if (m_carryOver > 0) {
                int to_copy = Min(length, m_carryOver); // length >= m_carryOver ? m_carryOver : length;
                memmove(buf, &m_currentBlock[BF_BLOCKSIZE - m_carryOver], to_copy);
                m_carryOver -= to_copy;
                buf += to_copy;
                length -= to_copy;
                getcount += to_copy;
            }

            if (length == 0) {
                break;
            }

            // Fetch a block worth of data to process.
            int blocklen = Straw::Get(m_currentBlock, BF_BLOCKSIZE);

            if (blocklen == 0) {
                break;
            }

            // If we got a full block worth of data, then process it, if not
            // then the Chained Straw can't get any more data so we just prepare
            // to pass it unchanged into the destination.
            if (blocklen == BF_BLOCKSIZE) {
                if (m_mode == STRAW_DECRYPT) {
                    m_blowFish->Decrypt(m_currentBlock, BF_BLOCKSIZE, m_currentBlock);
                } else {
                    m_blowFish->Encrypt(m_currentBlock, BF_BLOCKSIZE, m_currentBlock);
                }
            } else {
                memmove(&m_currentBlock[8 - blocklen], m_currentBlock, blocklen);
            }

            m_carryOver = blocklen;
        }

        return getcount;
    }

    return Straw::Get(buffer, length);
}

/**
 * @brief Submit the key the blowfish engine should use during encryption/decryption.
 */
void BlowStraw::Key(void *key, int length)
{
    // If we haven't got a Blowfish instance, create one to accept the key.
    if (m_blowFish == nullptr) {
        m_blowFish = new BlowfishEngine();
    }

    // Once we have our Blowfish instance, submit the key to prepare it for
    // encrypt/decrypt operations.
    if (m_blowFish != nullptr) {
        m_blowFish->Submit_Key(key, length);
    }
}
