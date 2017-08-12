////////////////////////////////////////////////////////////////////////////////
//                      --  R E D  A L E R T  + +  --                         //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: RedAlert++
//
//          File:: buffer.h
//        Author:: CCHyper
//  Contributors:: OmniBlade
//       History:: 
//
//   Description:: Primitive Buffer Classes.
// 
// Function List:: BufferClass::BufferClass()
//                 BufferClass::~BufferClass()
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "buffer.h"
#include "gamedebug.h"

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    BufferClass::BufferClass()
//
//  DESCRIPTION:
//    Constructor for BufferClass class.
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
BufferClass::BufferClass() :
    Buffer(nullptr),
    Size(0),
    Allocated(false)
{
    //empty
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    BufferClass::BufferClass()
//
//  DESCRIPTION:
//    Constructor for BufferClass class.
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
BufferClass::BufferClass(int size) :
    Buffer(nullptr),
    Size(size),
    Allocated(false)
{
    if ( size > 0 ) {
        Buffer = new uint8_t[size];
        Allocated = true;
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
BufferClass::BufferClass(void *buff, int size) :
	Buffer(static_cast<uint8_t*>(buff)),
	Size(size),
	Allocated(false)
{
	//DEBUG_ASSERT(buff != nullptr);
	//DEBUG_ASSERT(size > 0);

	if ( buff != nullptr && size > 0 ) {
		Buffer = new uint8_t[size];
		Allocated = true;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
BufferClass::BufferClass(BufferClass const &that) : 
    Buffer(that.Buffer), 
    Size(that.Size), 
    Allocated(false)
{
    //empty
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
BufferClass &BufferClass::operator=(BufferClass const &that)
{
    if ( this != &that ) {
        Free();
        Allocated = false;
        Buffer = that.Buffer;
        Size = that.Size;
    }

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
bool BufferClass::operator==(BufferClass const &that)
{
    return Buffer == that.Buffer
        && Size == that.Size
        && Allocated == that.Allocated;
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
bool BufferClass::operator!=(BufferClass const &that)
{
    return Buffer != that.Buffer
        && Size != that.Size
        && Allocated != that.Allocated;
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
void BufferClass::Clear()
{
    //
    // 
    //
    Size = 0;
    Allocated = false;
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
void BufferClass::Free()
{
    //
    // 
    //
    if ( Allocated ) {
        delete[] Buffer;
    }

    Buffer = nullptr;
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
void BufferClass::Resize(int size)
{
    //
    // Free existing buffer if we own it.
    //
    Free();
    
    //
    // 
    //
    Size = size;

    //
    // 
    //
    Buffer = new uint8_t[size];

    //
    // 
    //
    Allocated = true;
}


////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    <func name>
//
//  DESCRIPTION:
//    <desc>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
void BufferClass::Resize(void *buff, int size)
{
    //DEBUG_ASSERT(buff != nullptr);
    //DEBUG_ASSERT(size > 0);

    //
    // Free existing buffer if we own it.
    //
    Free();
    
    //
    // 
    //
    Size = size;

    if ( buff ) {
        Buffer = static_cast<uint8_t*>(buff);
        Allocated = false;
    } else {
        Buffer = new uint8_t[size];
        Allocated = true;
    }
}

uint8_t *BufferClass::Get_Buffer()
{ 
    return Buffer;
}

int BufferClass::Get_Size()
{ 
    DEBUG_LOG("Getting size %d from buffer.\n", Size);
    return Size;
}
