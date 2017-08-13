////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: PIPE.CPP
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Base putting stream class.
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
#include "pipe.h"
#include "gamedebug.h"

Pipe::~Pipe()
{
    if (m_chainTo != nullptr) {
        m_chainTo->m_chainFrom = m_chainFrom;
    }

    m_chainTo = nullptr;

    if (m_chainFrom != nullptr) {
        m_chainFrom->Put_To(m_chainTo);
    }

    m_chainFrom = nullptr;
}

void Pipe::Put_To(Pipe *pipe)
{
    if (m_chainTo != pipe) {
        if (pipe != nullptr && m_chainFrom != nullptr) {
            m_chainFrom->Put_To(nullptr);
            m_chainFrom = nullptr;
        }

        if (m_chainTo != nullptr) {
            m_chainFrom = nullptr;
            m_chainTo->Flush();
        }

        m_chainTo = pipe;

        if (pipe != nullptr) {
            pipe->m_chainFrom = this;
        }
    }
}

int Pipe::Put(const void *source, int length)
{
    DEBUG_ASSERT(source != nullptr);
    DEBUG_ASSERT(length > 0);

    if (m_chainTo != nullptr) {
        return m_chainTo->Put(source, length);
    }

    return length;
}

int Pipe::Flush()
{
    if (m_chainTo != nullptr) {
        return m_chainTo->Flush();
    }

    return 0;
}

int Pipe::End()
{
    return Flush();
}
