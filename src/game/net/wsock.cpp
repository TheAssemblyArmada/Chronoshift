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
#include "wsock.h"
#include "gamedebug.h"
#include "globals.h"
#include "keyboard.h"

#ifdef PLATFORM_WINDOWS
// TODO when we can use winsock2, change this.
//#include <ws2tcpip.h> //socklen_t typedef apparently
typedef int socklen_t;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define SOCKET_ERROR -1
#endif

#ifdef CHRONOSHIFT_SOCKETS_API
int WinsockInterfaceClass::SocketsMax = 0;
#endif // !PLATFORM_WINDOWS

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
WinsockInterfaceClass *&g_packetTransport = Make_Global<WinsockInterfaceClass *>(0x0069172C);
#else
WinsockInterfaceClass *g_packetTransport = nullptr;
#endif

WinsockInterfaceClass::WinsockInterfaceClass() :
    InBuffers(),
    OutBuffers(),
    IsOpen(false),
    Socket(INVALID_SOCKET),
    TaskHandle(INVALID_HANDLE_VALUE)
{
}

WinsockInterfaceClass::~WinsockInterfaceClass()
{
    Close();
}

BOOL WinsockInterfaceClass::Init()
{
    if (!IsOpen) {
        Socket = INVALID_SOCKET;
        TaskHandle = INVALID_HANDLE_VALUE;
        Discard_In_Buffers();
        Discard_Out_Buffers();

// WSAStartup is only required on windows.
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms742213%28v=vs.85%29.aspx
#ifdef PLATFORM_WINDOWS
        WORD wVersionRequested;
        WSADATA wsaData;

        // TODO RA originally requested 1.1 or higher, we should use the newer version 2.2 is supported all the way back to
        // win98.
        wVersionRequested = MAKEWORD(1, 1); // RA, 0x101

        if (WSAStartup(wVersionRequested, &wsaData) != 0) { // need to check agaisnt error
            DWORD error = GetLastError();
            DEBUG_LOG("WinsockInterface: Winsock failed to initialise - error code %d.\n", error);
            return false;
        }

        // Confirm that linked winsock supports requested version.
        // TODO upgrade this to 2.2.
        if ((wsaData.wVersion & 0xFF) != 1 || (wsaData.wVersion >> 8) != 1) {
            DEBUG_LOG("WinsockInterface: Winsock version is less than 1.1\n");
            return false;
        }
#endif // PLATFORM_WINDOWS

        IsOpen = true;
    }

    return true;
}

void WinsockInterfaceClass::Close()
{
    if (IsOpen) {
        Stop_Listening();
        Close_Socket();

#ifdef PLATFORM_WINDOWS
        WSACleanup(); // Again only needed on windows to close the winsock stuff
#endif // PLATFORM_WINDOWS

        IsOpen = false;
    }
}

void WinsockInterfaceClass::Close_Socket()
{
    if (Socket != INVALID_SOCKET) {
        closesocket(Socket);
        Socket = INVALID_SOCKET;
    }
}

int WinsockInterfaceClass::Read(void *dst, int &dst_read, void *dst_head, int &head_size)
{
    // Do we have any data in our in buffer?
    if (InBuffers.Count()) {
        // Get our data packet
        WinsockBufferType *in = InBuffers[0];

        // copy the data to the dst pointer, header to header pointer and
        // set the amount of data we had in this packet
        memcpy(dst, in->Data, in->Length);
        memcpy(dst_head, in->Header, 64);
        dst_read = in->Length;

        // delete the retrieved buffer form our queue.
        InBuffers.Delete(0);
        delete in;

        // return how much data we read.
        return dst_read;
    }

    // No data to retrieve
    return 0;
}

void WinsockInterfaceClass::Write_To(void *src, int src_len, void *src_head)
{
    // Create a new buffer to hold our data until a free send socket comes up
    WinsockBufferType *out = new WinsockBufferType;

    // Copy data, length and header into our buffer struct
    memcpy(out->Data, src, src_len);
    out->Length = src_len;
    out->Broadcast = 0;
    memcpy(out->Header, src_head, 10);

    // Add it to the outgoing buffer queue
    OutBuffers.Add(out);

#ifdef PLATFORM_WINDOWS
    // Send a message to our event queue that sent data is pending. 
    // TODO Will need to change this when we port to SDL or other cross platform event model.
    SendMessageA(MainWindow, Protocol_Event_Message(), 0, 2);
#endif

    // Hnadle events.
    g_keyboard->Check();
}

void WinsockInterfaceClass::Broadcast(void *src, int src_len)
{
    // Create a new buffer to hold our data until a free send socket comes up
    WinsockBufferType *wsockbuffer = new WinsockBufferType;

    // Copy data and length into our buffer struct, going to all destinations
    // so no header needed?
    memcpy(wsockbuffer->Data, src, src_len);
    wsockbuffer->Length = src_len;
    wsockbuffer->Broadcast = 1;

    OutBuffers.Add(wsockbuffer);

#ifdef PLATFORM_WINDOWS
    // Send a message to our event queue that sent data is pending.
    // TODO Will need to change this when we port to SDL or other cross platform event model.
    SendMessageA(MainWindow, Protocol_Event_Message(), 0, 2);
#endif

    g_keyboard->Check();
}

void WinsockInterfaceClass::Discard_In_Buffers()
{
    for (int index = InBuffers.Count() - 1; index >= 0; --index) {
        delete InBuffers[index];
        InBuffers.Delete(index);
    }
}

void WinsockInterfaceClass::Discard_Out_Buffers()
{
    for (int index = OutBuffers.Count() - 1; index >= 0; --index) {
        delete OutBuffers[index];
        OutBuffers.Delete(index);
    }
}

BOOL WinsockInterfaceClass::Start_Listening()
{
#ifdef CHRONOSHIFT_SOCKETS_API
    // Without the windows event loop, we have to fall back on non-blocking sockets and select from the look of things. This
    // sets up what sockets we want to listen for activity on.
    FD_SET(Socket, &ReadSockets);
    FD_SET(Socket, &WriteSockets);

    if (SocketsMax < Socket) {
        ++SocketsMax;
    }
#elif defined(PLATFORM_WINDOWS)
    if (WSAAsyncSelect(Socket, MainWindow, Protocol_Event_Message(), FD_READ | FD_WRITE) == -1) {
        DEBUG_LOG("WinsockInterface: Async select failed.\n");
        WSACancelAsyncRequest(TaskHandle);
        TaskHandle = INVALID_HANDLE_VALUE;

        return false;
    }
#endif
    return true;
}

void WinsockInterfaceClass::Stop_Listening()
{
#ifdef CHRONOSHIFT_SOCKETS_API
    FD_CLR(Socket, &ReadSockets);
    FD_CLR(Socket, &WriteSockets);
#elif defined(PLATFORM_WINDOWS)
    if (TaskHandle != INVALID_HANDLE_VALUE) {
        WSACancelAsyncRequest(TaskHandle);
        TaskHandle = INVALID_HANDLE_VALUE;
    }
#endif // PLATFORM_WINDOWS
}

void WinsockInterfaceClass::Clear_Socket_Error(unsigned socket)
{
    char optval[4];
    socklen_t optlen = 4;

    getsockopt(socket, SOL_SOCKET, SO_ERROR, optval, &optlen);
    *reinterpret_cast<uint32_t *>(optval) = 0;
    setsockopt(socket, SOL_SOCKET, SO_ERROR, optval, optlen);
}

BOOL WinsockInterfaceClass::Set_Socket_Options()
{
    static int socket_recv_buffer_size = SOCKET_BUFFER_SIZE;
    static int socket_tran_buffer_size = SOCKET_BUFFER_SIZE;

    if (setsockopt(Socket, SOL_SOCKET, SO_RCVBUF, (char *)&socket_recv_buffer_size, sizeof(int)) == SOCKET_ERROR) {
#if defined(PLATFORM_WINDOWS)
        int error = WSAGetLastError();
#else // !PLATFORM_WINDOWS
        int error = errno;
#endif // PLATFORM_WINDOWS
        DEBUG_LOG("WinsockInterface: Failed to set socket option SO_RCVBUF - error code %d.\n", error);
    }

    if (setsockopt(Socket, SOL_SOCKET, SO_SNDBUF, (char *)&socket_tran_buffer_size, sizeof(int)) == SOCKET_ERROR) {
#if defined(PLATFORM_WINDOWS)
        int error = WSAGetLastError();
#else // !PLATFORM_WINDOWS
        int error = errno;
#endif // PLATFORM_WINDOWS
        DEBUG_LOG("WinsockInterface: Failed to set socket option SO_SNDBUF - error code %d.\n", error);
    }
    return true;
}
