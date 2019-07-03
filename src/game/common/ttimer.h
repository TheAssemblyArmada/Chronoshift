/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Templated timers intended to tick on either the system timer or a frame.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
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
    uint32_t operator()() const;
};

// Wrapper around game's logical frame count.
class FrameTimerClass
{
public:
    uint32_t operator()() const;
};

template<class T>
class BasicTimerClass
{
public:
    BasicTimerClass() {}
    BasicTimerClass(const BasicTimerClass &that) : m_started(that.m_started), m_accumulated(that.m_accumulated) {}
    BasicTimerClass(const NoInitClass &noinit) {}
    ~BasicTimerClass() {}

    BasicTimerClass &operator=(const BasicTimerClass &that);

    void Reset(uint32_t value = 0);

    bool Started() const { return m_started != UINT32_MAX; }
    bool Stopped() const { return m_started == UINT32_MAX; }

protected:
    uint32_t Value() const { return m_timer() - m_started; }

protected:
    T m_timer;
    uint32_t m_started;
    uint32_t m_accumulated;
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
void BasicTimerClass<T>::Reset(uint32_t value)
{
    m_started = m_timer();
    m_accumulated = value;
}

template<typename T>
class TTimerClass : public BasicTimerClass<T>
{
#ifndef COMPILER_WATCOM
public:
    using BasicTimerClass<T>::m_timer;
    using BasicTimerClass<T>::m_started;
    using BasicTimerClass<T>::m_accumulated;
    using BasicTimerClass<T>::Value;
    using BasicTimerClass<T>::Started;
#endif

public:
    TTimerClass(uint32_t value = 0)
    {
        m_started = m_timer() - value;
        m_accumulated = value;
    }

    TTimerClass(const TTimerClass<T> &that) : BasicTimerClass<T>(that) {}
    TTimerClass(const NoInitClass &noinit) : BasicTimerClass<T>(noinit) {}

    TTimerClass<T> &operator=(TTimerClass<T> &that);
    TTimerClass<T> &operator=(uint32_t value);

    operator uint32_t() const { return Time(); }

    void Start();
    void Stop();
    uint32_t Time() const;
};

template<typename T>
TTimerClass<T> &TTimerClass<T>::operator=(TTimerClass<T> &that)
{
    BasicTimerClass<T>::operator=(that);

    return *this;
}

template<typename T>
TTimerClass<T> &TTimerClass<T>::operator=(uint32_t value)
{
    m_started = m_timer() - value;

    return *this;
}

template<typename T>
void TTimerClass<T>::Start()
{
    if (m_started == UINT32_MAX) {
        m_started = m_timer();
    }
}

template<typename T>
void TTimerClass<T>::Stop()
{
    if (m_started != UINT32_MAX) {
        m_accumulated += Time();
        m_started = UINT32_MAX;
    }
}

template<typename T>
uint32_t TTimerClass<T>::Time() const
{
    uint32_t ret = m_accumulated;

    if (Started()) {
        ret += Value();
    }

    return ret;
}

template<typename T>
class TCountDownTimerClass : public BasicTimerClass<T>
{
#ifndef COMPILER_WATCOM
public:
    using BasicTimerClass<T>::m_timer;
    using BasicTimerClass<T>::m_started;
    using BasicTimerClass<T>::m_accumulated;
    using BasicTimerClass<T>::Value;
    using BasicTimerClass<T>::Started;
#endif

public:
    TCountDownTimerClass(uint32_t value = 0)
    {
        m_started = m_timer();
        m_accumulated = value;
    }

    TCountDownTimerClass(const TCountDownTimerClass<T> &that) : BasicTimerClass<T>(that) {}
    TCountDownTimerClass(const NoInitClass &noinit) : BasicTimerClass<T>(noinit) {}

    TCountDownTimerClass<T> &operator=(TCountDownTimerClass<T> &that);
    TCountDownTimerClass<T> &operator=(uint32_t value);

    void Set(uint32_t value);

    operator uint32_t() const { return Time(); }

    bool Expired() const { return Time() == 0; }

    void Start();
    void Stop();
    uint32_t Time() const;
};

template<typename T>
TCountDownTimerClass<T> &TCountDownTimerClass<T>::operator=(TCountDownTimerClass<T> &that)
{
    BasicTimerClass<T>::operator=(that);

    return *this;
}

template<typename T>
TCountDownTimerClass<T> &TCountDownTimerClass<T>::operator=(uint32_t value)
{
    m_started = m_timer();
    m_accumulated = value;

    return *this;
}

template<typename T>
void TCountDownTimerClass<T>::Set(uint32_t value)
{
    m_started = m_timer();
    m_accumulated = value;
}

template<typename T>
void TCountDownTimerClass<T>::Start()
{
    if (!Started()) {
        m_started = m_timer();
    }
}

template<typename T>
void TCountDownTimerClass<T>::Stop()
{
    if (Started()) {
        m_accumulated = Time();
        m_started = UINT32_MAX;
    }
}

template<typename T>
uint32_t TCountDownTimerClass<T>::Time() const
{
    uint32_t accum = m_accumulated;

    if (Started()) {
        if (Value() < m_accumulated) {
            accum -= Value();
        } else {
            accum = 0;
        }
    }

    return accum;
}

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TTimerClass<SystemTimerClass> &TickCountTimer;
extern TCountDownTimerClass<SystemTimerClass> &FrameTimer;
#else
extern TTimerClass<SystemTimerClass> TickCountTimer;
extern TCountDownTimerClass<SystemTimerClass> FrameTimer;
#endif

#endif
