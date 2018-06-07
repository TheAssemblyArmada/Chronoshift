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

class NoInitClass;

// Wrapper around the platform timer.
class SystemTimerClass
{
public:
    int operator()() const;
};

// Wrapper around game's logical frame count.
class FrameTimerClass
{
public:
    int operator()() const;
};

template<class T>
class BasicTimerClass
{
public:
    BasicTimerClass() {}
    BasicTimerClass(const BasicTimerClass &that) : m_started(that.m_started), m_accumulated(that.m_accumulated) {}
    BasicTimerClass(NoInitClass const &noinit) {}
    ~BasicTimerClass() {}

    BasicTimerClass &operator=(const BasicTimerClass &that);

    void Reset(int value = 0);

protected:
    int Value() const { return m_timer() - m_started; }
    bool Has_Started() const { return m_started != -1; }

protected:
    T m_timer;
    int m_started;
    int m_accumulated;
};

template<typename T>
BasicTimerClass<T> &BasicTimerClass<T>::operator=(const BasicTimerClass &that)
{
    if (this != &that) {
        m_started = that.m_started;
        m_accumulated = that.m_accumulated;
    }

    return *this;
}

template<typename T>
void BasicTimerClass<T>::Reset(int value)
{
    m_started = m_timer();
    m_accumulated = value;
}

template<typename T>
class TTimerClass : public BasicTimerClass<T>
{
public:
    TTimerClass(int value = 0)
    {
        m_started = m_timer() - value;
        m_accumulated = value;
    }

    TTimerClass(TTimerClass<T> const &that) : BasicTimerClass(that) {}
    TTimerClass(NoInitClass const &noinit) : BasicTimerClass(noinit) {}

    TTimerClass<T> &operator=(TTimerClass<T> &that);
    TTimerClass<T> &operator=(int value);

    operator int() const { return Time(); }

    void Start();
    void Stop();
    int Time() const;

private:
    void Set(int value) { m_started = m_timer() - value; }
};

template<typename T>
TTimerClass<T> &TTimerClass<T>::operator=(TTimerClass<T> &that)
{
    BasicTimerClass::operator=(that);

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
int TTimerClass<T>::Time() const
{
    int ret = m_accumulated;

    if (Has_Started()) {
        ret += Value();
    }

    return ret;
}

template<typename T>
class TCountDownTimerClass : public BasicTimerClass<T>
{
public:
    TCountDownTimerClass(int value = 0)
    {
        m_started = m_timer();
        m_accumulated = value;
    }

    TCountDownTimerClass(TCountDownTimerClass<T> const &that) : BasicTimerClass(that) {}
    TCountDownTimerClass(NoInitClass const &noinit) : BasicTimerClass(noinit) {}

    TCountDownTimerClass<T> &operator=(TCountDownTimerClass<T> &that);
    TCountDownTimerClass<T> &operator=(int value);

    operator int() const { return Time(); }

    void Start();
    void Stop();
    int Time() const;
};

template<typename T>
TCountDownTimerClass<T> &TCountDownTimerClass<T>::operator=(TCountDownTimerClass<T> &that)
{
    BasicTimerClass::operator=(that);

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
int TCountDownTimerClass<T>::Time() const
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

#ifndef RAPP_STANDALONE
#include "hooker.h"
extern TTimerClass<SystemTimerClass> &TickCountTimer;
#else
extern TTimerClass<SystemTimerClass> TickCountTimer;
#endif

#endif
