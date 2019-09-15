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

#ifdef CHRONOSHIFT_SOCKETS_API
int WinsockInterfaceClass::SocketsMax = 0;
#endif // !PLATFORM_WINDOWS

#ifndef GAME_DLL
WinsockInterfaceClass *g_packetTransport = nullptr;
#endif

/**
 * 0x005A7FB0
 */
WinsockInterfaceClass::WinsockInterfaceClass() :
    m_InBuffers(),
    m_OutBuffers(),
    m_IsOpen(false),
    m_Socket(INVALID_SOCKET)
#ifdef PLATFORM_WINDOWS
    ,
    m_TaskHandle(INVALID_HANDLE_VALUE)
#endif
{
}

/**
 * 0x005A7FF4
 */
WinsockInterfaceClass::~WinsockInterfaceClass()
{
    Close();
}

/**
 * Initialises the sockets interface if required and clears existing data.
 *
 * 0x005A7FF4
 */
BOOL WinsockInterfaceClass::Init()
{
    if (!m_IsOpen) {
        m_Socket = INVALID_SOCKET;
        Discard_In_Buffers();
        Discard_Out_Buffers();

// WSAStartup is only required on windows.
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms742213%28v=vs.85%29.aspx
#ifdef PLATFORM_WINDOWS
        m_TaskHandle = INVALID_HANDLE_VALUE;
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

        m_IsOpen = true;
    }

    return true;
}

/**
 * Stops listening for new connections and closes the socket as well as the socket interface.
 *
 * 0x005A804C
 */
void WinsockInterfaceClass::Close()
{
    if (m_IsOpen) {
        Stop_Listening();
        Close_Socket();

#ifdef PLATFORM_WINDOWS
        WSACleanup(); // Again only needed on windows to close the winsock stuff
#endif // PLATFORM_WINDOWS

        m_IsOpen = false;
    }
}

/**
 * Closes the socket if open.
 *
 * 0x005A8084
 */
void WinsockInterfaceClass::Close_Socket()
{
    if (m_Socket != INVALID_SOCKET) {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
}

/**
 * Reads data from the sockets interface.
 *
 * 0x005A8260
 */
int WinsockInterfaceClass::Read(void *dst, int &dst_read, void *dst_head, int &head_size)
{
    // Do we have any data in our in buffer?
    if (m_InBuffers.Count()) {
        // Get our data packet
        WinsockBufferType *in = m_InBuffers[0];

        // copy the data to the dst pointer, header to header pointer and set the amount of data we had in this packet
        memcpy(dst, in->m_Data, in->m_Length);
        memcpy(dst_head, in->m_Header, sizeof(in->m_Header));
        dst_read = in->m_Length;

        // Dequeue and delete the retrieved buffer.
        m_InBuffers.Delete(0);
        delete in;

        // return how much data we read.
        return dst_read;
    }

    // No data to retrieve
    return 0;
}

/**
 * Writes data to the sockets interface for a specific destination.
 *
 * 0x005A82E4
 */
void WinsockInterfaceClass::Write_To(void *src, int src_len, void *src_head)
{
    // Create a new buffer to hold our data until a free send socket comes up
    WinsockBufferType *out = new WinsockBufferType;

    // Copy data, length and header into our buffer struct
    memcpy(out->m_Data, src, src_len);
    out->m_Length = src_len;
    out->m_Broadcast = false;
    // Original does this as an 8byte copy then a two byte copy... header struct perhaps?
    memcpy(out->m_Header, src_head, 10);

    // Add it to the outgoing buffer queue
    m_OutBuffers.Add(out);

#ifdef PLATFORM_WINDOWS
    // Send a message to our event queue that sent data is pending. 
    // TODO Will need to change this when we port to SDL or other cross platform event model.
    SendMessageA(MainWindow, Protocol_Event_Message(), 0, FD_WRITE);
#endif

    // Handle events.
    g_keyboard->Check();
}

/**
 * Writes data to the sockets interface for broadcast.
 *
 * 0x005A8384
 */
void WinsockInterfaceClass::Broadcast(void *src, int src_len)
{
    // Create a new buffer to hold our data until a free send socket comes up
    WinsockBufferType *wsockbuffer = new WinsockBufferType;

    // Copy data and length into our buffer struct, going to all destinations
    // so no header needed?
    memcpy(wsockbuffer->m_Data, src, src_len);
    wsockbuffer->m_Length = src_len;
    wsockbuffer->m_Broadcast = true;

    m_OutBuffers.Add(wsockbuffer);

#ifdef PLATFORM_WINDOWS
    // Send a message to our event queue that sent data is pending.
    // TODO Will need to change this when we port to SDL or other cross platform event model.
    SendMessageA(MainWindow, Protocol_Event_Message(), 0, FD_WRITE);
#endif

    g_keyboard->Check();
}

/**
 * Discards buffers for incoming data.
 *
 * 0x005A812C
 */
void WinsockInterfaceClass::Discard_In_Buffers()
{
    for (int index = m_InBuffers.Count() - 1; index >= 0; --index) {
        delete m_InBuffers[index];
        m_InBuffers.Delete(index);
    }
}

/**
 * Discards buffers for outgoing data.
 *
 * 0x005A8154
 */
void WinsockInterfaceClass::Discard_Out_Buffers()
{
    for (int index = m_OutBuffers.Count() - 1; index >= 0; --index) {
        delete m_OutBuffers[index];
        m_OutBuffers.Delete(index);
    }
}

/**
 * Starts listening for connections.
 *
 * 0x005A80AC
 */
BOOL WinsockInterfaceClass::Start_Listening()
{
#ifdef CHRONOSHIFT_SOCKETS_API
    // Without the windows event loop, we have to fall back on non-blocking sockets and select from the look of things. This
    // sets up what sockets we want to listen for activity on.
    FD_SET(m_Socket, &ReadSockets);
    FD_SET(m_Socket, &WriteSockets);

    if (SocketsMax < m_Socket) {
        SocketsMax = m_Socket;
    }
#elif defined PLATFORM_WINDOWS
    if (WSAAsyncSelect(m_Socket, MainWindow, Protocol_Event_Message(), FD_READ | FD_WRITE) == -1) {
        DEBUG_LOG("WinsockInterface: Async select failed.\n");
        WSACancelAsyncRequest(m_TaskHandle);
        m_TaskHandle = INVALID_HANDLE_VALUE;

        return false;
    }
#endif
    return true;
}

/**
 * Stops listening for connections.
 *
 * 0x005A8104
 */
void WinsockInterfaceClass::Stop_Listening()
{
#ifdef CHRONOSHIFT_SOCKETS_API
    FD_CLR(m_Socket, &ReadSockets);
    FD_CLR(m_Socket, &WriteSockets);
#elif defined PLATFORM_WINDOWS
    if (m_TaskHandle != INVALID_HANDLE_VALUE) {
        WSACancelAsyncRequest(m_TaskHandle);
        m_TaskHandle = INVALID_HANDLE_VALUE;
    }
#endif // PLATFORM_WINDOWS
}

/**
 * Clears any errors on the socket.
 *
 * 0x005A8408
 */
void WinsockInterfaceClass::Clear_Socket_Error(unsigned socket)
{
    char optval[4];
    socklen_t optlen = sizeof(optval);

    getsockopt(socket, SOL_SOCKET, SO_ERROR, optval, &optlen);
    *reinterpret_cast<uint32_t *>(optval) = 0;
    setsockopt(socket, SOL_SOCKET, SO_ERROR, optval, optlen);
}

/**
 * Sets standard Chronoshift socket options.
 *
 * 0x005A8458
 */
BOOL WinsockInterfaceClass::Set_Socket_Options()
{
    static int32_t socket_recv_buffer_size = SOCKET_BUFFER_SIZE;
    static int32_t socket_tran_buffer_size = SOCKET_BUFFER_SIZE;

    if (setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (char *)&socket_recv_buffer_size, sizeof(socket_recv_buffer_size))
        == SOCKET_ERROR) {
        DEBUG_LOG("WinsockInterface: Failed to set SOL_SOCKET option SO_RCVBUF - error code %d.\n", LastSocketError);
    }

    if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, (char *)&socket_tran_buffer_size, sizeof(socket_tran_buffer_size))
        == SOCKET_ERROR) {
        DEBUG_LOG("WinsockInterface: Failed to set SOL_SOCKET option SO_SNDBUF - error code %d.\n", LastSocketError);
    }
    return true;
}
