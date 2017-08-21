/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief Multiple precision arithmetic library for cryptographic functions.
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

#ifndef MPMATH_H
#define MPMATH_H

#include "always.h"
#include "gamedebug.h"
#include "straw.h"

typedef uint32_t mp_digit_u;
typedef int32_t mp_digit_s;
typedef uint64_t mp_digit_du;
typedef int64_t mp_digit_ds;

#define htolexmp htole32
#define lexmptoh le32toh
#define htobexmp htobe32
#define bexmptoh be32toh
#define BYTES_PER_UNIT sizeof(mp_digit_u) // number of bytes in a unit
#define DIGITSIZE (BYTES_PER_UNIT * 8) // number of bits in a unit
#define LOG_DIGITSIZE 5
#define SIGN_BIT 0x80000000
#define XMPD_SIG_BIT (1LL << (2 * DIGITSIZE - 1))
#define MIN_KEY_BITS 384
#define MAX_KEY_BITS 2048
#define MAX_BIT_PRECISION (MAX_KEY_BITS)
#define MAX_BYTE_PRECISION (MAX_BIT_PRECISION / 8)
#define MAX_UNIT_PRECISION (MAX_BIT_PRECISION / DIGITSIZE)
#define MAX_XMPU 0xFFFFFFFF
#define MAX_PRIME_TABLE_SIZE 3511

namespace MPMath
{
///////////////////////////////////////////////////////////////////////////////
//  Precision Check Routines
///////////////////////////////////////////////////////////////////////////////
int32_t Byte_Precision(int32_t length);
int32_t Significance(mp_digit_u const *number, int32_t precision);
int32_t Count_Bits(mp_digit_u const *number, int32_t precision);
int32_t Count_Bytes(mp_digit_u *number, int32_t precision);

////////////////////////////////////////////////////////////////////////////////
//
//  Utility Routines
//      Host format for endian conversion purposes is an array of mp_digit_u ordered
//      least significant to most significant (LE) with the mp_digit_u ints themselves
//      in the host order.
//
////////////////////////////////////////////////////////////////////////////////
void Init(mp_digit_u *number, mp_digit_u value, int32_t precision);
void Move(mp_digit_u *dst, mp_digit_u const *src, int32_t precision);
void Move_HTOLE(mp_digit_u *dst, mp_digit_u const *src, int32_t precision);
void Move_LETOH(mp_digit_u *dst, mp_digit_u const *src, int32_t precision);
void Move_HTOBE(mp_digit_u *dst, mp_digit_u const *src, int32_t precision);
void Move_BETOH(mp_digit_u *dst, mp_digit_u const *src, int32_t precision);

////////////////////////////////////////////////////////////////////////////////
//
//  Encoding and Decoding Routines
//      See here for information on DER encoding of an integer.
//      http://luca.ntop.org/Teaching/Appunti/asn1.html
//
////////////////////////////////////////////////////////////////////////////////
int32_t DER_Length_Encode(uint32_t length, uint8_t *dst);
uint32_t Encode(uint8_t *dst, const mp_digit_u *number, int32_t precision);
uint32_t Encode_Bounded(uint8_t *dst, uint32_t bytes, const mp_digit_u *number, int32_t precision);
int32_t DER_Encode(const mp_digit_u *number, uint8_t *dst, int32_t precision);
void Unsigned_Decode(mp_digit_u *number, const uint8_t *src, int32_t bytes, int32_t precision);
void Signed_Decode(mp_digit_u *number, const uint8_t *src, int32_t bytes, int32_t precision);
void DER_Decode(mp_digit_u *number, const uint8_t *src, int32_t precision);

////////////////////////////////////////////////////////////////////////////////
//
//  Unary Operators
//
////////////////////////////////////////////////////////////////////////////////
void Not(mp_digit_u *number, int32_t precision);
void Inc(mp_digit_u *number, int32_t precision);
void Dec(mp_digit_u *number, int32_t precision);
void Neg(mp_digit_u *number, int32_t precision);
void Abs(mp_digit_u *number, int32_t precision);
void Shift_Left_Bits(mp_digit_u *number, uint32_t bits, int32_t precision);
void Shift_Right_Bits(mp_digit_u *number, uint32_t bits, int32_t precision);
mp_digit_u Rotate_Left(mp_digit_u *number, mp_digit_u carry, int32_t precision);
mp_digit_u Rotate_Right(mp_digit_u *number, mp_digit_u carry, int32_t precision);

////////////////////////////////////////////////////////////////////////////////
//
//  Comparison Operators
//
////////////////////////////////////////////////////////////////////////////////
mp_digit_u Test_Eq_Int(mp_digit_u const *number, int32_t value, int32_t precision);
int32_t Compare(mp_digit_u const *num1, mp_digit_u const *num2, int32_t precision);

////////////////////////////////////////////////////////////////////////////////
//
//  Arithmetic Operators
//
///////////////////////////////////////////////////////////////////////////////
mp_digit_u Add(mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, mp_digit_u carry, int32_t precision);
mp_digit_u Add_Int(mp_digit_u *result, mp_digit_u *num1, mp_digit_u num2, mp_digit_u carry, int32_t precision);
mp_digit_u Sub(mp_digit_u *result, mp_digit_u *left_number, mp_digit_u *right_number, mp_digit_u borrow, int32_t precision);
mp_digit_u Sub_Int(mp_digit_u *result, mp_digit_u *num1, mp_digit_u num2, mp_digit_u borrow, int32_t precision);
int32_t Unsigned_Mul(mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, int32_t precision);
int32_t Unsigned_Mul_Int(mp_digit_u *prod, mp_digit_u *multiplicand, mp_digit_s multiplier, int32_t precision);
int32_t Unsigned_Mul_Int16(mp_digit_u *result, mp_digit_u *num1, int16_t num2, int32_t precision);
int32_t Signed_Mul(mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, int32_t precision);
int32_t Signed_Mul_Int(mp_digit_u *prod, mp_digit_u *multiplicand, mp_digit_u multiplier, int32_t precision);
void Hybrid_Mul(mp_digit_u *result, mp_digit_u *num1, mp_digit_u num2, int32_t precision);
void Double_Mul(mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, int32_t precision);
int32_t Unsigned_Div(mp_digit_u *remainder, mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, int32_t precision);
uint32_t Unsigned_Div_Int(mp_digit_u *result, mp_digit_u *num1, mp_digit_u num2, int32_t precision);
void Signed_Div(mp_digit_u *remainder, mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, int32_t precision);
void Modulo(mp_digit_u *remainder, mp_digit_u *number, mp_digit_u *modulus, int32_t precision);

////////////////////////////////////////////////////////////////////////////////
//
//  Number Theory stuff for RSA
//
////////////////////////////////////////////////////////////////////////////////
void Inverse_A_Mod_B(mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, int32_t precision);
int32_t Reciprocal(mp_digit_u *result, mp_digit_u const *number, int32_t precision);
int32_t Mod_Mul_Upton(mp_digit_u *result, mp_digit_u *num1, mp_digit_u *num2, int32_t precision);
void Prepare_Modulus_Upton(mp_digit_u const *modulus, int32_t precision);
void Mod_Mult_Clear_Upton(int32_t precision);
int Exponent_Mod(
    mp_digit_u *result, mp_digit_u *number, mp_digit_u const *exponent, mp_digit_u const *modulus, int32_t precision);

////////////////////////////////////////////////////////////////////////////////
//
//  Number Theory stuff for prime generation
//
////////////////////////////////////////////////////////////////////////////////
uint16_t const *Fetch_Prime_Table(void);
int Fetch_Prime_Size(void);
void Decode_ASCII(char const *str, unsigned int *mpn, int precision);
mp_digit_u Rabin_Miller_Test(Straw *rng, unsigned int const *w, int rounds, int precision);
mp_digit_u Fermat_Test(unsigned int const *candidate_prime, unsigned int rounds, int precision);
mp_digit_u Is_Small_Prime(unsigned int const *candidate, int precision);
mp_digit_u Is_Prime(unsigned int const *prime, int precision);
void Randomize_Bounded(
    unsigned int *result, Straw *rng, unsigned int const *minval, unsigned int const *maxval, int precision);
void Randomize(unsigned int *result, Straw *rng, int total_bits, int precision);
mp_digit_u Small_Divisors_Test(mp_digit_u const *candidate, int precision);

inline mp_digit_u Add_Carry(mp_digit_u a, mp_digit_u b, mp_digit_u c)
{
    mp_digit_du r = (mp_digit_du)a + b + c;
    return r > MAX_XMPU;
}

inline mp_digit_u Sub_Carry(mp_digit_u a, mp_digit_u b, mp_digit_u c)
{
    return (b + c) > a;
}

inline int32_t Bits_To_Units(int32_t bits)
{
    return (bits + DIGITSIZE - 1) / DIGITSIZE;
}

inline int32_t Units_To_Bytes(int32_t units)
{
    return units * BYTES_PER_UNIT;
}

inline void Set_Bit(mp_digit_u *number, int32_t bits)
{
    number[bits / DIGITSIZE] |= 1 << (bits % DIGITSIZE);
}

inline uint32_t Unit_Sig_Bit(int32_t bits)
{
    return bits != 0 ? 1 << (bits - 1) % DIGITSIZE : 0;
}

inline void Print(mp_digit_u *number, int precision)
{
    DEBUG_LOG("\n");

    for (int i = precision - 1; i >= 0; i--) {
        DEBUG_LOG("%08x", number[i]);
    }

    DEBUG_LOG("\n");
}

} // end of MPMath namespace

#endif // MPMATH_H
