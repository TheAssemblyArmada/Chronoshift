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
#include "buffer.h"
#include "gamedebug.h"

BufferClass::BufferClass() :
    m_buffer(nullptr),
    m_size(0),
    m_allocated(false)
{
    //empty
}

BufferClass::BufferClass(int size) :
    m_buffer(nullptr),
    m_size(size),
    m_allocated(false)
{
    if ( size > 0 ) {
        m_buffer = new uint8_t[size];
        m_allocated = true;
    }
}

BufferClass::BufferClass(void *buff, int size) :
	m_buffer(static_cast<uint8_t*>(buff)),
	m_size(size),
	m_allocated(false)
{
	if ( buff == nullptr && size > 0 ) {
		m_buffer = new uint8_t[size];
		m_allocated = true;
	}
}

BufferClass::BufferClass(BufferClass const &that) : 
    m_buffer(that.m_buffer), 
    m_size(that.m_size), 
    m_allocated(false)
{
    //empty
}

BufferClass &BufferClass::operator=(BufferClass const &that)
{
    if ( this != &that ) {
        Free();
        m_allocated = false;
        m_buffer = that.m_buffer;
        m_size = that.m_size;
    }

    return *this;
}

bool BufferClass::operator==(BufferClass const &that)
{
    return m_buffer == that.m_buffer
        && m_size == that.m_size
        && m_allocated == that.m_allocated;
}

bool BufferClass::operator!=(BufferClass const &that)
{
    return m_buffer != that.m_buffer
        && m_size != that.m_size
        && m_allocated != that.m_allocated;
}

void BufferClass::Clear()
{
    m_size = 0;
    m_allocated = false;
}

void BufferClass::Free()
{
    if ( m_allocated ) {
        delete[] m_buffer;
    }

    m_buffer = nullptr;
}

void BufferClass::Resize(int size)
{
    Free();
    m_size = size;
    m_buffer = new uint8_t[size];
    m_allocated = true;
}

void BufferClass::Resize(void *buff, int size)
{
    //DEBUG_ASSERT(buff != nullptr);
    //DEBUG_ASSERT(size > 0);

    // Free existing buffer if we own it.
    Free();
    
    m_size = size;

    if ( buff ) {
        m_buffer = static_cast<uint8_t*>(buff);
        m_allocated = false;
    } else {
        m_buffer = new uint8_t[size];
        m_allocated = true;
    }
}

uint8_t *BufferClass::Get_Buffer() const
{ 
    return m_buffer;
}

int BufferClass::Get_Size() const
{ 
    return m_size;
}
