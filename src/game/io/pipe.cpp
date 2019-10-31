/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for output stream classes.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "pipe.h"
#include "gamedebug.h"

Pipe::~Pipe()
{
    if (m_ChainTo != nullptr) {
        m_ChainTo->m_ChainFrom = m_ChainFrom;
    }

    if (m_ChainFrom != nullptr) {
        m_ChainFrom->Put_To(m_ChainTo);
    }

    m_ChainTo = nullptr;
    m_ChainFrom = nullptr;
}

void Pipe::Put_To(Pipe *pipe)
{
    if (m_ChainTo != pipe) {
        if (pipe != nullptr && pipe->m_ChainFrom != nullptr) {
            pipe->m_ChainFrom->Put_To(nullptr);
            pipe->m_ChainFrom = nullptr;
        }

        if (m_ChainTo != nullptr) {
            m_ChainTo->m_ChainFrom = nullptr;
            m_ChainTo->Flush();
        }

        m_ChainTo = pipe;

        if (pipe != nullptr) {
            pipe->m_ChainFrom = this;
        }
    }
}

int Pipe::Put(const void *source, int length)
{
    DEBUG_ASSERT(source != nullptr);
    DEBUG_ASSERT(length > 0);

    if (m_ChainTo != nullptr) {
        return m_ChainTo->Put(source, length);
    }

    return length;
}

int Pipe::Flush()
{
    if (m_ChainTo != nullptr) {
        return m_ChainTo->Flush();
    }

    return 0;
}

int Pipe::End()
{
    return Flush();
}
