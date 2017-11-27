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

    int Time()
    {
        int ret = m_accumulated;

        if (m_started != -1) {
            ret += Get_Ticks();
        }

        return ret;
    }

private:
    int Get_Ticks() { return m_timer() - m_started; }

private:
    T m_timer;
    int m_started;
    int m_accumulated;
};

#ifndef RAPP_STANDALONE
extern TTimerClass<SystemTimerClass> &TickCountTimer;
#else
extern TTimerClass<SystemTimerClass> TickCountTimer;
#endif

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
    TCountDownTimerClass(TCountDownTimerClass<T> const &that) : m_started(that.m_started), m_accumulated(that.m_accumulated)
    {
    }

    TCountDownTimerClass<T> &operator=(TCountDownTimerClass<T> &that);
    TCountDownTimerClass<T> &operator=(int value);

    operator int() { return Time(); }

    void Start();
    void Stop();
    int Time();
    bool Has_Started() const { return m_started != -1; }

private:
    T m_timer;
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

class NoInitClass;

template<class T>
class BasicTimerClass
{
public:
    BasicTimerClass(unsigned int value = 0) : m_started(m_timer()), m_accumulated(value) {}
    BasicTimerClass(BasicTimerClass const &that) : m_started(that.m_started), m_accumulated(that.m_accumulated) {}
    BasicTimerClass(NoInitClass const &noinit) {}
    ~BasicTimerClass() {}

    BasicTimerClass &operator=(BasicTimerClass const &that)
    {
        if (this != &that) {
            m_timer = that.m_timer;
            m_started = that.m_started;
            m_accumulated = that.m_accumulated;
        }

        return *this;
    }

    //BasicTimerClass &operator=(unsigned int value)
    //{
    //    Set(value);
    //    return *this;
    //}

    bool Has_Started() { return m_started != -1; }

    void Start()
    {
        if (!Has_Started()) {
            m_started = m_timer();
        }
    }

    void Stop()
    {
        if (Has_Started()) {
            m_accumulated = Value();
            m_started = -1;
        }
    }

    bool Has_Expired() { return Time() <= 0; }
    void Set(int value) { m_started = m_timer() - value; }

    // Clear the accumulated time. The value to reset to is
    // optional, otherwise it is reset to zero.
    void Reset(unsigned int value = 0)
    {
        m_started = m_timer();
        m_accumulated = value;
    }

    //
    int Value() const { return m_timer() - m_started; }

    // Get the time left until the countdown reaches 0.
    int Time() // Advance?
    {
        int accum = m_accumulated;
        if (Has_Started()) {
            accum -= Value();

            if (accum < 0) {
                accum = 0;
            }
        }

        return accum;
    }

    unsigned int const Func() // TODO, need to work out name.
    {
        unsigned int accum = m_accumulated;

        if (Has_Started()) {
            accum += Value();
        }

        return accum;
    }

private:
    T m_timer;
    int m_started;
    int m_accumulated;
};

#endif
