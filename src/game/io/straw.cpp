/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base interface for fetching a stream of data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "straw.h"
#include "gamedebug.h"

Straw::~Straw()
{
    if (m_ChainTo != nullptr) {
        m_ChainTo->m_ChainFrom = m_ChainFrom;
    }

    if (m_ChainFrom != nullptr) {
        m_ChainFrom->Get_From(m_ChainTo);
    }

    m_ChainTo = nullptr;
    m_ChainFrom = nullptr;
}

void Straw::Get_From(Straw *straw)
{
    if (m_ChainTo != straw) {
        if (straw != nullptr && straw->m_ChainFrom != nullptr) {
            straw->m_ChainFrom->Get_From(nullptr);
            straw->m_ChainFrom = nullptr;
        }

        if (m_ChainTo != nullptr) {
            m_ChainTo->m_ChainFrom = nullptr;
        }

        m_ChainTo = straw;

        if (straw) {
            straw->m_ChainFrom = this;
        }
    }
}

int Straw::Get(void *source, int length)
{
    if (m_ChainTo != nullptr) {
        return m_ChainTo->Get(source, length);
    }

    return 0;
}
