/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief Macros for handling different platform endian formats.
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

#ifndef BASE_ENDIANTYPE_H
#define BASE_ENDIANTYPE_H

#include "bittype.h"

// Implement missing intrinsics for watcom build.
#if defined COMPILER_WATCOM
extern uint16_t _byteswap_ushort(uint16_t uInput);
#pragma aux _byteswap_ushort = "ror ax,8" parm[ax] value[ax] modify exact[ax] nomemory;

extern uint32_t _byteswap_ulong(uint32_t uInput);
#pragma aux _byteswap_ulong = "bswap eax" parm[eax] value[eax] modify exact[eax] nomemory;
#endif

#if defined(PLATFORM_LINUX)
#include <endian.h>

#elif defined(PLATFORM_OSX)
#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

//
// If building for OpenBSD...
//
#elif defined(__OpenBSD__)
#include <sys/endian.h>

//
// If building for NetBSD, FreeBSD or DragonFly...
//
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
#include <sys/endian.h>

#define be16toh(x) betoh16(x)
#define le16toh(x) letoh16(x)

#define be32toh(x) betoh32(x)
#define le32toh(x) letoh32(x)

#define be64toh(x) betoh64(x)
#define le64toh(x) letoh64(x)

//
// If building for Windows...
//
#elif defined(PLATFORM_WINDOWS)
#include <stdlib.h>

#if defined(SYSTEM_LITTLE_ENDIAN)
#define htobe16(x) _byteswap_ushort(x)
#define htole16(x) (x)
#define be16toh(x) _byteswap_ushort(x)
#define le16toh(x) (x)

#define htobe32(x) _byteswap_ulong(x)
#define htole32(x) (x)
#define be32toh(x) _byteswap_ulong(x)
#define le32toh(x) (x)

#define htobe64(x) _byteswap_uint64(x)
#define htole64(x) (x)
#define be64toh(x) _byteswap_uint64(x)
#define le64toh(x) (x)
#endif // SYSTEM_LITTLE_ENDIAN

#else
#error platform not supported
#endif // PLATFORM_LINUX || PLATFORM_OSX || PLATFORM_WINDOWS

// Byte access macros for different word sizes.
#if defined(SYSTEM_LITTLE_ENDIAN)
#define GETBYTE32(x, n) (*((uint8_t *)&(x) + n))
#define GETSBYTE32(x, n) (*((int8_t *)&(x) + n))
#define GETBYTE16(x, n) (*((uint8_t *)&(x) + n))
#define GETSBYTE16(x, n) (*((int8_t *)&(x) + n))
#define GETWORD32(x, n) (*((uint16_t *)&(x) + n))
#define GETSWORD32(x, n) (*((int16_t *)&(x) + n))

#define htolef(x) (x)
#define leftoh(x) (x)
#elif defined(SYSTEM_BIG_ENDIAN)
#define GETBYTE32(x, n) (*((uint8_t *)&(x) + (3 - n)))
#define GETSBYTE32(x, n) (*((int8_t *)&(x) + (3 - n)))
#define GETBYTE16(x, n) (*((uint8_t *)&(x) + (1 - n)))
#define GETSBYTE16(x, n) (*((int8_t *)&(x) + (1 - n)))
#define GETWORD32(x, n) (*((uint16_t *)&(x) + (1 - n)))
#define GETSWORD32(x, n) (*((int16_t *)&(x) + (1 - n)))

#define htolef(x) *(float *)(&(htole32(*(uint32_a *)(&x))))
#define leftoh(x) *(float *)(&(le32toh(*(uint32_a *)(&x))))

#else
#error byte order not supported
#endif

#define WRITE_LE_UINT16(p, value) ((p)[0] = ((value)&0xFF), (p)[1] = (((value) >> 8) & 0xFF))
#define WRITE_LE_UINT32(p, value) \
    ((p)[0] = ((value)&0xFF), \
        (p)[1] = (((value) >> 8) & 0xFF), \
        (p)[2] = (((value) >> 16) & 0xFF), \
        (p)[3] = (((value) >> 24) & 0xFF))
#define READ_LE_UINT16(p) ((uint16_t)(p)[0] | ((uint16_t)(p)[1] << 8))
#define READ_LE_UINT32(p) ((uint32_t)(p)[0] | ((uint32_t)(p)[1] << 8) | ((uint32_t)(p)[2] << 16) | ((uint32_t)(p)[3] << 24))
#define GETBYTE(x, n) (*((uint8_t *)&(x) + n))
#define GETSBYTE(x, n) (*((int8_t *)&(x) + n))

#endif // _ENDIAN_H
