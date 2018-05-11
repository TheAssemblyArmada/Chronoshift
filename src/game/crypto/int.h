/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief C++ class wrapper around the C style API of MPMath.
 *
 * @copyright RedAlert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
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
    Int() { MPMath::Init(m_number, 0, N); }
    explicit Int(int value) { MPMath::Init(m_number, value, N); }
    Int(const Int &that) { MPMath::Move(m_number, that.m_number, N); }

    operator mp_digit_u *() { return m_number; }
    operator mp_digit_u const *() const { return m_number; }
    operator void const *() const { return m_number; }

    Int &operator+=(const Int &number)
    {
        Carry = MPMath::Add(m_number, m_number, number.m_number, false, N);
        return *this;
    }

    Int &operator-=(const Int &number)
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

    bool operator>(const Int &number) { return MPMath::Compare(m_number, number.m_number, N) > 0; }
    bool operator<(const Int &number) { return MPMath::Compare(m_number, number.m_number, N) < 0; }

    Int Inverse(const Int &modulus)
    {
        Int result;
        MPMath::Inverse_A_Mod_B(result.m_number, m_number, modulus.m_number, N);
        return result;
    }

    Int &operator=(const Int &that)
    {
        if (this != &that) {
            MPMath::Move(m_number, that.m_number, N);
        }

        return *this;
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

    Int operator*(const Int &that) const
    {
        Int result;
        Error = MPMath::Signed_Mul(result.m_number, m_number, that.m_number, N);
        return result;
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

    friend Int operator-(const Int &lhs, const int &rhs)
    {
        Int result;
        Borrow = MPMath::Sub_Int(result.m_number, lhs.m_number, rhs, false, N);
        return result;
    }

    friend Int operator+(const Int &lhs, const int &rhs)
    {
        Int result;
        Carry = MPMath::Add_Int(result.m_number, lhs.m_number, rhs, false, N);
        return result;
    }

    friend Int operator*(const Int &lhs, const int &rhs)
    {
        Int result;
        Error = MPMath::Unsigned_Mul_Int(result.m_number, lhs.m_number, rhs, N);
        return result;
    }

    friend Int operator/(const Int &lhs, const int &rhs)
    {
        Int result;
        Error = MPMath::Unsigned_Div_Int(result.m_number, lhs.m_number, rhs, N);
        return result;
    }

    void Negate()
    {
        // TODO
    }

    Int Abs()
    {
        return *this; // TEMP, TODO
    }

    Int Exponential_Modulation(const Int &exponent, const Int &modulus)
    {
        Int result;

        MPMath::Exponent_Mod(result, *this, exponent, modulus, N);

        return result;
    }

    int DER_Encode(uint8_t *dst) const { return MPMath::DER_Encode(m_number, dst, N); }

    void DER_Decode(const uint8_t *src) { MPMath::DER_Decode(m_number, src, N); }

    int Bit_Count() { return MPMath::Count_Bits(m_number, N); }

    bool Is_Negative()
    {
        return false; // TODO
    }

    BOOL Is_Prime() { return MPMath::Is_Prime(*this, N); }

    void Randomize(Straw &rng, const Int &minval, const Int &maxval)
    {
        MPMath::Randomize_Bounded(*this, &rng, minval, maxval, N);
        m_number[0] |= 1;
    }

    void Host_To_LE() { MPMath::Move_LETOH(m_number, m_number, N); }
    void LE_To_Host() { MPMath::Move_HTOLE(m_number, m_number, N); }

    static Int Decode_ASCII(char *);
    static int Precision() { return N; }

private:
    mp_digit_u m_number[N];

public:
    static bool Carry;
    static bool Borrow;
    static int Error;
    static Int Remainder;
};

template<int T>
bool Int<T>::Carry = false;

template<int T>
bool Int<T>::Borrow = false;

template<int T>
int Int<T>::Error = 0;

template<int T>
Int<T> Int<T>::Remainder = Int<T>();

template<int N>
Int<N> Int<N>::Decode_ASCII(char *)
{
    return Remainder; // TODO, TEMP
}

template<class T>
class RemainderTable
{
public:
    RemainderTable(const T &p);
    BOOL Has_Zero() { return m_hasZeroEntry; }
    void Increment(uint16_t increment);

protected:
    BOOL m_hasZeroEntry;
    uint16_t m_table[MAX_PRIME_TABLE_SIZE];
};

template<class T>
RemainderTable<T>::RemainderTable(const T &p) : m_hasZeroEntry(false)
{
    int table_size = MPMath::Fetch_Prime_Size();
    const uint16_t *table = MPMath::Fetch_Prime_Table();

    for (int i = 0; i < table_size; ++i) {
        m_table[i] = MPMath::Unsigned_Div_Int(T::Remainder, p, table[i], T::Precision());
    }
}

template<class T>
void RemainderTable<T>::Increment(uint16_t increment)
{
    m_hasZeroEntry = false;

    int table_size = MPMath::Fetch_Prime_Size();
    const uint16_t *table = MPMath::Fetch_Prime_Table();

    for (int i = 0; i < table_size; ++i) {
        m_table[i + 1] += increment;

        while (m_table[i + 1] >= table[i]) {
            m_table[i + 1] -= table[i];
        }

        m_hasZeroEntry = m_hasZeroEntry || m_table[i + 1] == 0;
    }
}

#endif // INT_H
