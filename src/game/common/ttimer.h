/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Templated timers intended to tick on either the system timer or a frame.
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

#ifndef TTIMER_H
#define TTIMER_H

#include "always.h"

class SystemTimerClass
{
public:
    int operator()() const;
};

class FrameTimerClass
{
public:
    int operator()() const;
};

template<typename T>
class TTimerClass
{
public:
    TTimerClass(int value = 0) : m_started(m_timer()), m_accumulated(value) { Set(value); }
    TTimerClass(TTimerClass<T> const &that) : m_started(that.m_started), m_accumulated(that.m_accumulated) {}

    TTimerClass<T> &operator=(TTimerClass<T> &that);
    TTimerClass<T> &operator=(int value);

    void Start();
    void Stop();

    void Set(int value) { m_started = m_timer() - value; }

    void Reset(int value = 0)
    {
        m_started = m_timer();
        m_accumulated = value;
    }

    int Time() { return m_timer() - m_started; }

private:
    int Get_Ticks()
    {
        if (m_started != -1) {
            m_accumulated += m_timer();
        }
        return m_accumulated;
    }

private:
    T m_timer;

protected:
    int m_started;
    int m_accumulated;
};

extern TTimerClass<SystemTimerClass> TickCountTimer;

template<typename T>
TTimerClass<T> &TTimerClass<T>::operator=(TTimerClass<T> &that)
{
    if (this != &that) {
        m_started = that.m_started;
        m_accumulated = that.m_accumulated;
    }

    return *this;
}

template<typename T>
TTimerClass<T> &TTimerClass<T>::operator=(int value)
{
    Set(value, true);

    return *this;
}

template<typename T>
void TTimerClass<T>::Start()
{
    if (m_started == -1) {
        m_started = m_timer();
    }
}

template<typename T>
void TTimerClass<T>::Stop()
{
    if (m_started != -1) {
        m_accumulated += Time();
        m_started = -1;
    }
}

template<typename T>
class TCountDownTimerClass
{
public:
    TCountDownTimerClass(int value = 0) : m_started(m_timer()), m_accumulated(value) {}
    TCountDownTimerClass(TCountDownTimerClass<T> const &that) : m_started(that.m_started), m_accumulated(that.m_accumulated) {}

    TCountDownTimerClass<T> &operator=(TCountDownTimerClass<T> &that);
    TCountDownTimerClass<T> &operator=(int value);

    operator int() { return Time(); }

    void Start();
    void Stop();
    int Time();
    bool Has_Started() const { return m_started != -1; }

private:
    T m_timer;

protected:
    int m_started;
    int m_accumulated;
};

// extern TCountDownTimerClass<SystemTimerClass> CountDownTimer;

template<typename T>
TCountDownTimerClass<T> &TCountDownTimerClass<T>::operator=(TCountDownTimerClass<T> &that)
{
    if (this != &that) {
        m_started = that.m_started;
        m_accumulated = that.m_accumulated;
    }

    return *this;
}

template<typename T>
TCountDownTimerClass<T> &TCountDownTimerClass<T>::operator=(int value)
{
    m_started = m_timer();
    m_accumulated = value;

    return *this;
}

template<typename T>
void TCountDownTimerClass<T>::Start()
{
    if (!Has_Started()) {
        m_started = m_timer();
    }
}

template<typename T>
void TCountDownTimerClass<T>::Stop()
{
    if (Has_Started()) {
        m_accumulated = Time();
        m_started = -1;
    }
}

template<typename T>
int TCountDownTimerClass<T>::Time()
{
    if (Has_Started()) {
        m_accumulated -= m_timer() - m_started;

        if (m_accumulated < 0) {
            m_accumulated = 0;
        }
    }

    return m_accumulated;
}

#endif
