/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for benchmarking functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "bench.h"
#include <stdio.h>

Benchmark *g_Benches = nullptr;

/**
 * @brief
 *
 * @address 0x005D53D0 (beta)
 */
Benchmark::Benchmark()
{
    m_Accumulated = 0;
    m_field_9 = 0;
    m_field_D = 0;
    m_Started = m_Timer();
}

/**
 * @brief
 *
 * @address 0x005D5414 (beta)
 */
void Benchmark::Reset()
{
    m_Accumulated = 0;
    m_field_9 = 0;
    m_field_D = 0;
}

/**
 * @brief
 *
 * @address 0x005D5430 (beta)
 */
void Benchmark::Start(BOOL reset)
{
    if (reset) {
        Reset();
    }
    m_Started = m_Timer();
}

/**
 * @brief
 *
 * @address 0x005D5468 (beta)
 */
void Benchmark::End()
{
    uint32_t time = m_Timer() - m_Started;
    if (m_field_9 == 256) {
        m_Accumulated -= (m_Accumulated / m_field_9);
        m_Accumulated += time;
    } else {
        m_Accumulated += time;
        ++m_field_9;
    }
    ++m_field_D;
}

/**
 * @brief
 *
 * @address 0x005D54CC (beta)
 */
uint32_t Benchmark::Value()
{
    if (m_field_9 > 0) {
        return m_Accumulated / m_field_9;
    }
    return 0;
}

/**
 * @brief
 *
 * @address 0x004C0F24 (beta)
 */
char *Benchmark::Bench_Time(BenchType type)
{
    static char _buffer[32];
    int32_t main_dval = g_Benches[BENCH_MAIN].m_field_D;
    if (main_dval == 0) {
        main_dval = 1;
    }
    int32_t main_val = g_Benches[BENCH_MAIN].Value();

    Benchmark *bench = &g_Benches[type];
    int32_t dval = bench->m_field_D;
    int32_t val = bench->Value();
    if ((dval * val) > (main_dval * main_val)) {
        val = main_val / dval;
    }
    int percent = 0;
    if (main_val && main_dval) {
        percent = (99 * val * dval) / (main_val * main_dval);
    }
    if (percent > 99) {
        percent = 99;
    }
    sprintf(_buffer, "%-2d%% %7d", percent, val);
    return _buffer;
}
