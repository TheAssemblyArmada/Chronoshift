/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe stream class implementation writing data processed with the blowfish algorithm.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "blowpipe.h"
#include <cstring>
#include <algorithm>

using std::memset;

BlowPipe::~BlowPipe()
{
    if (m_BlowFish != nullptr) {
        delete m_BlowFish;
    }

    m_BlowFish = nullptr;
}

/**
 * @brief Flushes any remaining data that is waiting to be encrypted.
 */
int BlowPipe::Flush()
{
    int putcount = 0;

    // RA BlowPipe doesn't pad and encrypt the last block of data, but BlowStraw
    // code will keep fetching in blocks of 8 until it gets EOF so for data such
    // as the mix header, it needs to be padded for correct operation.
    if (m_Carryover != 0 && m_BlowFish != nullptr) {
        memset(&m_CurrentBlock[m_Carryover], 0, BF_BLOCKSIZE - m_Carryover);

        if (m_Mode == PIPE_DECRYPT) {
            m_BlowFish->Decrypt(m_CurrentBlock, BF_BLOCKSIZE, m_CurrentBlock);
        } else {
            m_BlowFish->Encrypt(m_CurrentBlock, BF_BLOCKSIZE, m_CurrentBlock);
        }

        putcount += Pipe::Put(m_CurrentBlock, BF_BLOCKSIZE);
    }

    // clear the carryover bytes
    m_Carryover = 0;

    if (m_ChainTo != nullptr) {
        return m_ChainTo->Flush() + putcount;
    }

    return putcount;
}

/**
 * @brief Writes data from the buffer to the next pipe in the chain, processing it with blowfish.
 */
int BlowPipe::Put(const void *buffer, int length)
{
    // If a blowfish instance hasn't been allocated yet, just pass it through to
    // the next Pipe in the chain (if any).
    if (buffer && length >= 1 && m_BlowFish != nullptr) {
        char *buf = static_cast<char *>(const_cast<void *>(buffer));
        int putcount = 0;

        // If there is carried over data that didn't exactly fill a Blowfish
        // block, then make it up to a full block with the new data and encrypt.
        if (m_Carryover) {
            int rsize = std::min(BF_BLOCKSIZE - m_Carryover, length);

            memmove(&m_CurrentBlock[m_Carryover], buf, rsize);
            buf += rsize;
            m_Carryover += rsize;
            length -= rsize;

            // Make sure the extra data was enough to get us to the block size.
            if (m_Carryover == BF_BLOCKSIZE) {
                if (m_Mode == PIPE_DECRYPT) {
                    m_BlowFish->Decrypt(m_CurrentBlock, BF_BLOCKSIZE, m_CurrentBlock);
                } else {
                    m_BlowFish->Encrypt(m_CurrentBlock, BF_BLOCKSIZE, m_CurrentBlock);
                }

                putcount = Pipe::Put(m_CurrentBlock, BF_BLOCKSIZE);
                m_Carryover = 0;
            }
        }

        // While we have more data than the block size, keep encrypting it in
        // blocks and writing it to the next Pipe in the chain.
        while (length >= BF_BLOCKSIZE) {
            if (m_Mode == PIPE_DECRYPT) {
                m_BlowFish->Decrypt(buf, BF_BLOCKSIZE, m_CurrentBlock);
            } else {
                m_BlowFish->Encrypt(buf, BF_BLOCKSIZE, m_CurrentBlock);
            }

            putcount += Pipe::Put(m_CurrentBlock, BF_BLOCKSIZE);

            buf += BF_BLOCKSIZE;
            length -= BF_BLOCKSIZE;
        }

        // If we have some data left over that isn't quite enough for a full
        // block, load it into the buffer and make a note of how much we have
        // for either the next Put call or for Flush.
        if (length > 0) {
            memmove(m_CurrentBlock, buf, length);
            m_Carryover = length;
        }

        return putcount;
    }

    return Pipe::Put(buffer, length);
}

/**
 * @brief Submit the key the blowfish engine should use during encryption/decryption.
 */
void BlowPipe::Key(void *key, int length)
{
    // If we haven't got a Blowfish instance, create one to accept the key.
    if (m_BlowFish == nullptr) {
        m_BlowFish = new BlowfishEngine();
    }

    // Once we have our Blowfish instance, submit the key to prepare it for
    // encrypt/decrypt operations.
    if (m_BlowFish != nullptr) {
        m_BlowFish->Submit_Key(key, length);
    }
}
