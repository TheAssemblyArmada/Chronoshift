/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for managing surfaces and their states.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SURFACEMONITOR_H
#define SURFACEMONITOR_H
#include "always.h"

#ifdef BUILD_WITH_DDRAW
#include "ddraw.h"
#endif

class SurfaceMonitorClass
{
    enum
    {
        SURFACE_COUNT = 20,
    };

public:
    SurfaceMonitorClass();
    ~SurfaceMonitorClass();
#ifdef BUILD_WITH_DDRAW
    void Add_Surface(LPDIRECTDRAWSURFACE new_surface);
    void Remove_Surface(LPDIRECTDRAWSURFACE old_surface);
    bool Got_Surface_Already(LPDIRECTDRAWSURFACE test_surface);
#endif
    void Restore_Surfaces();
    void Set_Surface_Focus(bool focus);
    void Release();
    BOOL Surfaces_Restored() const { return m_surfacesRestored; }
    void Clear_Surfaces_Restored() { m_surfacesRestored = false; }

#ifdef GAME_DLL
    static void Hook_Me();
#endif

private:
    BOOL m_surfacesRestored;
#ifdef BUILD_WITH_DDRAW
    LPDIRECTDRAWSURFACE m_surface[SURFACE_COUNT];
#endif
    BOOL m_inFocus;
};

#ifdef GAME_DLL
#include "hooker.h"

extern void (*&Misc_Focus_Loss_Function)();
extern void (*&Misc_Focus_Restore_Function)();
extern SurfaceMonitorClass &g_allSurfaces;

inline void SurfaceMonitorClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
#ifdef BUILD_WITH_DDRAW
    Hook_Function(0x005CA1D0, *SurfaceMonitorClass::Add_Surface);
    Hook_Function(0x005CA230, *SurfaceMonitorClass::Remove_Surface);
    Hook_Function(0x005CA280, *SurfaceMonitorClass::Got_Surface_Already);
#endif
    Hook_Function(0x005CA2D0, *SurfaceMonitorClass::Restore_Surfaces);
    Hook_Function(0x005CA370, *SurfaceMonitorClass::Set_Surface_Focus);
    Hook_Function(0x005CA390, *SurfaceMonitorClass::Release);
#endif
}
#else
extern void (*Misc_Focus_Loss_Function)();
extern void (*Misc_Focus_Restore_Function)();
extern SurfaceMonitorClass g_allSurfaces;
#endif

#endif // SURFACEMONITOR_H
