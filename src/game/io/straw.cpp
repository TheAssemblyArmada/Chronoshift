////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: STRAW.CPP
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Base fetching stream class.
//
//       License:: Redalert++ is free software: you can redistribute it and/or
//                 modify it under the terms of the GNU General Public License
//                 as published by the Free Software Foundation, either version
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#include "straw.h"
#include "gamedebug.h"

Straw::~Straw()
{
    if (m_chainTo != nullptr) {
        m_chainTo->m_chainFrom = m_chainFrom;
    }

    m_chainTo = nullptr;

    if (m_chainFrom != nullptr) {
        m_chainFrom->Get_From(m_chainFrom);
    }

    m_chainFrom = nullptr;
}

void Straw::Get_From(Straw *straw)
{
    DEBUG_LOG("Calling Straw::Get_From.\n");
    if (m_chainTo != straw) {
        if (straw != nullptr && m_chainFrom != nullptr) {
            m_chainFrom->Get_From(nullptr);
            m_chainFrom = nullptr;
        }

        if (m_chainTo != nullptr) {
            m_chainFrom = nullptr;
        }

        m_chainTo = straw;

        if (straw) {
            straw->m_chainFrom = this;
        }
    }
}

int Straw::Get(void *source, int length)
{
    DEBUG_LOG("Calling Straw::Get.\n");
    if (m_chainTo != nullptr) {
        return m_chainTo->Get(source, length);
    }

    return 0;
}

#ifndef RAPP_STANDALONE
void Straw::Hook_Get_From(Straw *ptr, Straw *straw)
{
    ptr->Straw::Get_From(straw);
}

int Straw::Hook_Get(Straw *ptr, void *source, int length)
{
    return ptr->Straw::Get(source, length);
}
#endif
