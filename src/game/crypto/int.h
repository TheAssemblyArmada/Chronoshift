/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief C++ class wrapper around the C style API of MPMath.
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

#ifndef INT_H
#define INT_H

#include "always.h"
#include "mpmath.h"

template<int N>
class Int
{
public:
    Int() { MPMath::Init(m_number, 0, sizeof(N)); }
    Int(int value) { MPMath::Init(m_number, value, N); }
    Int(Int const &that) { MPMath::Move(m_number, that.m_number, N); }

    operator mp_digit_u *() { return m_number; }
    operator mp_digit_u const *() const { return m_number; }
    operator void const *()const { return m_number; }

    Int operator-(mp_digit_s value)
    {
        Int result;
        Borrow = MPMath::Sub_Int(result.m_number, m_number, value, false, N);
        return result;
    }

    Int operator+(mp_digit_s value)
    {
        Int result;
        Carry = MPMath::Add_Int(result.m_number, m_number, value, false, N);
        return result;
    }

    Int operator*(mp_digit_s value)
    {
        Int result;
        Error = MPMath::Unsigned_Mul_Int(result.m_number, m_number, value, false, N);
        return result;
    }

    Int operator/(mp_digit_s value)
    {
        Int result;
        Error = MPMath::Unsigned_Div_Int(result.m_number, m_number, value, false, N);
        return result;
    }

    Int &operator+=(Int const &number)
    {
        Carry = MPMath::Add(m_number, m_number, number.m_number, false, N);
        return *this;
    }

    Int &operator-=(Int const &number)
    {
        Borrow = MPMath::Sub(m_number, m_number, number.m_number, false, N);
        return *this;
    }

    Int operator<<(int value)
    {
        Int result(*this);
        MPMath::Shift_Left_Bits(result.m_number, value, N);
        return result;
    }

    bool operator>(Int const &number) { return MPMath::Compare(m_number, number.m_number, N) > 0; }
    bool operator<(Int const &number) { return MPMath::Compare(m_number, number.m_number, N) < 0; }

    Int Inverse(Int const &modulus)
    {
        Int result;
        MPMath::Inverse_A_Mod_B(result.m_number, m_number, modulus.m_number, N);
        return result;
    }

    Int &operator=(Int &)
    {
        return *this; // TEMP, TODO
    }

    Int &operator<<=(int)
    {
        return *this; // TEMP, TODO
    }

    Int &operator>>=(int)
    {
        return *this; // TEMP, TODO
    }

    Int operator+(Int &)
    {
        return *this; // TEMP, TODO
    }

    Int operator*(Int &)
    {
        return *this; // TEMP, TODO
    }

    Int operator/(uint16_t)
    {
        return *this; // TEMP, TODO
    }

    Int operator/(uint32_t)
    {
        return *this; // TEMP, TODO
    }

    Int operator/(Int &)
    {
        return *this; // TEMP, TODO
    }

    Int operator>>(int)
    {
        return *this; // TEMP, TODO
    }

    int operator==(Int &)
    {
        return *this; // TEMP, TODO
    }

    int operator!=(Int &)
    {
        return *this; // TEMP, TODO
    }

    int operator>(Int &number)
    {
        return 0; // TEMP, TODO
    }

    int operator>=(Int &)
    {
        return 0; // TEMP, TODO
    }

    int operator<(Int &)
    {
        return 0; // TEMP, TODO
    }

    int operator<=(Int &)
    {
        return 0; // TEMP, TODO
    }

    Int &operator*=(Int &number)
    {
        return *this; // TEMP, TODO
    }

    Int &operator/=(Int &number)
    {
        return *this; // TEMP, TODO
    }

    void Negate()
    {
        // TODO
    }

    Int Abs()
    {
        return *this; // TEMP, TODO
    }

    Int Exp_B_Mod_C(Int &, Int &)
    {
        return *this; // TEMP, TODO
    }

    void Set_Bit(int)
    {
        // TODO
    }

    int Encode(unsigned char *, unsigned int)
    {
        return 0; // TODO
    }

    int Encode(unsigned char *)
    {
        return 0; // TODO
    }

    void Signed_Decode(unsigned char *, int)
    {
        // TODO
    }

    void Unsigned_Decode(unsigned char *, int)
    {
        // TODO
    }

    int DER_Encode(uint8_t *dst) const
    {
        return MPMath::DER_Encode(m_number, dst, N);
    }

    void DER_Decode(const uint8_t *src)
    {
        MPMath::DER_Decode(m_number, src, N);
    }

    int Byte_Count()
    {
        return 0; // TODO
    }

    int Bit_Count()
    {
        return 0; // TODO
    }

    bool Is_Negative()
    {
        return false; // TODO
    }

    unsigned int Max_Bit_Precision()
    {
        return 0; // TODO
    }

    bool Is_Small_Prime()
    {
        return false; // TODO
    }

    bool Small_Divisors_Test()
    {
        return false; // TODO
    }

    bool Fermat_Test(unsigned int)
    {
        return false; // TODO
    }

    bool Is_Prime()
    {
        return false; // TODO
    }

    bool Rabin_Miller_Test(Straw &, unsigned int)
    {
        return false; // TODO
    }

    void Randomize(Straw &rng, Int const &minval, Int const &maxval)
    {
        // TODO
    }

    void Randomize(Straw &rng, int)
    {
        // TODO
    }

    static Int Unsigned_Mult(Int &, Int &);
    static void Unsigned_Divide(Int &, Int &, Int &, Int &);
    static void Signed_Divide(Int &, Int &, Int &, Int &);
    static Int Decode_ASCII(char *);

private:
    mp_digit_u m_number[N];

public:
    static bool Carry;
    static bool Borrow;
    static int Error;
    static Int Remainder;
};

template<class T>
class RemainderTable
{
public:
    RemainderTable(RemainderTable &p);
    bool Has_Zero();
    void Increment(RemainderTable &);
    void Increment(uint16_t increment);

protected:
    bool m_hasZeroEntry;
    uint16_t m_table[MAX_PRIME_TABLE_SIZE];
};

template<int T>
bool Int<T>::Carry = false;

template<int T>
bool Int<T>::Borrow = false;

template<int T>
int Int<T>::Error = 0;

template<int T>
Int<T> Int<T>::Remainder = Int<T>();

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<int N>
Int<N> Int<N>::Unsigned_Mult(Int<N> &, Int<N> &)
{
    return Remainder; // TEMP, TODO
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<int N>
void Int<N>::Unsigned_Divide(Int<N> &, Int<N> &, Int<N> &, Int<N> &)
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<int N>
void Int<N>::Signed_Divide(Int<N> &, Int<N> &, Int<N> &, Int<N> &)
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<int N>
Int<N> Int<N>::Decode_ASCII(char *)
{
    return Remainder; // TODO, TEMP
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<class T>
RemainderTable<T>::RemainderTable(RemainderTable<T> &p) : m_hasZeroEntry(false)
{
    int v3 = MPMath::Fetch_Prime_Size();
    uint16_t const *v5 = MPMath::Fetch_Prime_Table();

    if (v3 > 0) {

    }
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<class T>
bool RemainderTable<T>::Has_Zero()
{
    return false; // TODO, returns m_hasZeroEntry?
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<class T>
void RemainderTable<T>::Increment(RemainderTable &)
{
}

////////////////////////////////////////////////////////////////////////////////
//
//  NAME:
//    ()
//
//  DESCRIPTION:
//    <function overview>
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
template<class T>
void RemainderTable<T>::Increment(uint16_t increment)
{
    m_hasZeroEntry = false;

    int psize = MPMath::Fetch_Prime_Size();
    uint16_t const *ptable = MPMath::Fetch_Prime_Table();
}

#endif // INT_H
