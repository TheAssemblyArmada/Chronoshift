/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Interface for network communication management
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#pragma once

#ifndef CONNMAN_H
#define CONNMAN_H

class ConnManClass
{
public:
    ConnManClass() {}
    virtual ~ConnManClass() {}

    virtual int32_t Service() = 0; // loop function, handles reading, writing, queueing and sending
    virtual int32_t Send_Private_Message(void *data, int32_t datalen, int32_t flag,
        int32_t id) = 0; // sends private message to a specific connection
    virtual int32_t Get_Private_Message(
        void *data, int32_t *datalen, int32_t *id) = 0; // gets private message of a specific connection?
    virtual int32_t Num_Connections() = 0; // returns number of connections
    virtual int32_t Connection_ID(int32_t index) = 0; // gets id from a connection index
    virtual int32_t Connection_Index(int32_t id) = 0; // gets index from a connection id
    virtual int32_t Global_Num_Send() = 0; // send count of all connections
    virtual int32_t Global_Num_Receive() = 0; // receive count of all connections
    virtual int32_t Private_Num_Send(int32_t id) = 0; // send count of a specific connection
    virtual int32_t Private_Num_Receive(int32_t id) = 0; // receive count of a specific connection
    virtual void Reset_Response_Time() = 0; // reset response times of all connections
    virtual uint32_t Response_Time() = 0; // get the average response time of all connections
    virtual void Set_Timing(uint32_t retry_delta, uint32_t max_retries, uint32_t retry_timeout) = 0;
    virtual void Configure_Debug(int32_t arg1, int32_t arg2, int32_t arg3, char **arg4, int32_t arg5,
        int32_t arg6) = 0; // configures mono debug output?
    virtual void Mono_Debug_Print(int32_t arg1, int32_t arg1) = 0; // debug printing
};

#endif // CONNMAN_H
