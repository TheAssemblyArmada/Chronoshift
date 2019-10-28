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

#pragma once

#ifndef BENCH_H
#define BENCH_H

#include "always.h"

enum BenchType
{
    BENCH_MAIN,
    BENCH_FIND_PATH,
    BENCH_TARGET_SCAN,
    BENCH_AI,
    BENCH_CELL,
    BENCH_SIDEBAR,
    BENCH_RADAR,
    BENCH_TACTICAL,
    BENCH_PER_CELL_AI,
    BENCH_EVAL_OBJ,
    BENCH_EVAL_CELL,
    BENCH_EVAL_WALL,
    BENCH_POWER_BAR,
    BENCH_TABS,
    BENCH_SHROUD,
    BENCH_ANIMS,
    BENCH_OBJECTS,
    BENCH_PALETTE,
    BENCH_FULL_PROCESS,
    BENCH_BLIT,
    BENCH_RULES,
    BENCH_SCENARIO,
    BENCH_COUNT,
};

class CPUTimer
{
public:
    uint32_t inline operator()() const
    {
        return __rdtsc() >> 4; // TODO this needs checking
    }
};

class Benchmark
{
public:
    Benchmark();
    ~Benchmark() {}
    void Reset();
    void Start(BOOL reset = false);
    void End();
    uint32_t Value();

    static char *Bench_Time(BenchType type);

private:
    CPUTimer m_Timer;
    uint32_t m_Started;
    uint32_t m_Accumulated;
    uint32_t m_field_9; // overflow check? wraps to a lower precision format when 256?
    uint32_t m_field_D;
};

extern Benchmark *g_Benches;

#ifdef CHRONOSHIFT_DEBUG
    #define BENCHMARK_START(type) \
    if (g_Benches != nullptr) { \
        g_Benches[type].Start(); \
    }
    #define BENCHMARK_END(type) \
        if (g_Benches != nullptr) { \
            g_Benches[type].End(); \
        }
#else
    #define BENCHMARK_START(type)
    #define BENCHMARK_END(type)
#endif

#endif // BENCH_H
