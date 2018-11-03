/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Wrapper around sockets API.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef WSOCK_H
#define WSOCK_H

#include "always.h"
#include "vector.h"

#if defined(PLATFORM_WINDOWS) && !defined(_WINSOCKAPI_)
#include <winsock.h>
#else
#include <sys/types.h>
#include <unistd.h>

#ifndef SOCKET
#define SOCKET int
#endif

#ifndef HANDLE
#define HANDLE int
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE -1
#endif

#define closesocket close

#endif

// http://www.techpowerup.com/forums/threads/c-c-sockets-faq-and-how-to-win-linux.56901/
// http://stackoverflow.com/questions/2284428/in-c-networking-using-select-do-i-first-have-to-listen-and-accept

enum ProtocolEnum
{
    PROTOCOL_NONE = 0,
    PROTOCOL_IPX = 1,
    PROTOCOL_UDP = 2
};

#define COMM_BUFFER_SIZE 1024

class WinsockInterfaceClass
{
    enum
    {
        SOCKET_BUFFER_SIZE = 0x20000,
    };
protected:
    struct WinsockBufferType
    {
    public:
        char Header[64];
        int32_t Length;
        int32_t Broadcast;
        char Data[1024];
    };

public:
    WinsockInterfaceClass();
    virtual ~WinsockInterfaceClass();

    BOOL Init();
    void Close();

    virtual void Close_Socket();
    virtual int Read(void *dst, int &dst_read, void *dst_head, int &head_size);
    virtual void Write_To(void *src, int src_len, void *src_head);
    virtual void Broadcast(void *src, int src_len);
    virtual void Discard_In_Buffers();
    virtual void Discard_Out_Buffers();
    virtual BOOL Start_Listening();
    virtual void Stop_Listening();
    virtual void Clear_Socket_Error(unsigned socket);
    virtual BOOL Set_Socket_Options();
    virtual void Set_Broadcast_Address(void *address) {}
    virtual ProtocolEnum Get_Protocol() { return PROTOCOL_NONE; }
    virtual int Protocol_Event_Message() { return 0; }
    virtual int Open_Socket(unsigned port) { return 0; }
    virtual int Message_Handler(void *a1, unsigned a2, unsigned a3, int a4) { return 1; }

    // We return a copy of ReadSockets/WriteSockets that can be modified by select
#ifdef CHRONOSHIFT_SOCKETS_API
    fd_set *Get_Readset()
    {
        memcpy(&ReadyReadSockets, &ReadSockets, sizeof(fd_set));
        return &ReadyReadSockets;
    }

    fd_set *Get_Writeset()
    {
        memcpy(&ReadyWriteSockets, &WriteSockets, sizeof(fd_set));
        return &ReadyWriteSockets;
    }
#endif

protected:
    DynamicVectorClass<WinsockBufferType *> InBuffers;
    DynamicVectorClass<WinsockBufferType *> OutBuffers;
    BOOL IsOpen;
    SOCKET Socket;
    HANDLE TaskHandle;
    char RecvBuffer[COMM_BUFFER_SIZE];
#ifdef COMPILER_WATCOM
    char pad; // Line up struct as per original or buffer really is 1025 bytes rather than 1024.
#endif
// Some extra members for if we are using pure sockets api rather than windows sockets api
#ifdef CHRONOSHIFT_SOCKETS_API
    fd_set ReadSockets;
    fd_set WriteSockets;
    fd_set ReadyReadSockets;
    fd_set ReadyWriteSockets;
    static int SocketsMax;
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern WinsockInterfaceClass *&g_packetTransport;
#else
extern WinsockInterfaceClass *g_packetTransport;
#endif

#endif // WSOCK_H
