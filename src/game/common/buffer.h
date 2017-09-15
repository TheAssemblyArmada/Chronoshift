/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief Wrapper around memory allocation for simple buffers.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#include "always.h"

class BufferClass
{
public:
    BufferClass();
    BufferClass(int size);
    BufferClass(void *buff, int size = 0);
    BufferClass(BufferClass const &that);
    ~BufferClass() { Reset(); }

    BufferClass &operator=(BufferClass const &that);
    bool operator==(BufferClass const &that);
    bool operator!=(BufferClass const &that);

    uint8_t *Get_Buffer() const;
    int Get_Size() const;
    void Set_Size(int size) { m_size = size; }
    void Set_Buffer(void *buffer) { m_buffer = static_cast<uint8_t*>(buffer); }
    void Set_Allocated(BOOL allocated) { m_allocated = allocated; }
    BOOL Is_Allocated() { return m_allocated; }
    bool Is_Valid() { return m_buffer != nullptr && m_size > 0; }
    void Reset() { Free(); Clear(); }
    void Resize(int size = 0);
    void Resize(void *buff, int size = 0);

private:
    // These should only be called internally to control the buffer state
    void Clear();
    void Free();

protected:
    uint8_t *m_buffer;
    int m_size;
    BOOL m_allocated;
};

#endif // BUFFER_H
