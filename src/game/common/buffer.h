////////////////////////////////////////////////////////////////////////////////
//                        --  C H R O N O  S T L --                           //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: ChronoSTL
//
//			File:: TBUFFER.H
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Primitive buffer classes.
//
//       License:: Distributed under the GNU License, Version 2.0.
//			       (See accompanying file LICENSE.TXT)
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#include	"always.h"

class BufferClass
{
    public:
        ////////////////////////////////////////////////////////////////////////
        //  Public Constructors / Destructors
        ////////////////////////////////////////////////////////////////////////
        BufferClass();
        BufferClass(int size);
        BufferClass(void *buff, int size = 0);
        BufferClass(BufferClass const &that);
        ~BufferClass() { Reset(); }
        
        ////////////////////////////////////////////////////////////////////////
        //  Overloaded Operators
        ////////////////////////////////////////////////////////////////////////
        BufferClass &operator=(BufferClass const &that);
        bool operator==(BufferClass const &that);
        bool operator!=(BufferClass const &that);
        
        ////////////////////////////////////////////////////////////////////////
        //  Public Methods
        ////////////////////////////////////////////////////////////////////////
        uint8_t *Get_Buffer();
        int Get_Size();
        bool Is_Allocated() { return Allocated; }

        // 
        bool Is_Valid()
        {
            return Get_Buffer() != nullptr;
        }
        
        // 
        void Reset() { Free(); Clear(); };
        
        // 
        void Resize(int size = 0);
        void Resize(void *buff = nullptr, int size = 0);

    private:

        // These should only be called internally to control the buffer state
        void Clear();

        // 
        void Free();

    protected:

        // This is a pointer to the allocated data.
        uint8_t *Buffer;

        // This is the size of the allocated space in the buffer.
        int Size;

        // Does the buffer data pointer refer to memory that this class has manually
        // allocated? If so, then this class is responsible for deleting it.
        bool Allocated;
};

#endif // BUFFER_H
