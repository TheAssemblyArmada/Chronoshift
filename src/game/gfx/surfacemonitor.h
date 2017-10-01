/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for managing surfaces and their states.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
#include "ddraw.h"

#ifndef RAPP_STANDALONE
#include "hooker.h"
#endif

#ifndef RAPP_STANDALONE
extern void (*&Misc_Focus_Loss_Function)();
extern void (*&Misc_Focus_Restore_Function)();
#else
extern void (*Misc_Focus_Loss_Function)();
extern void (*Misc_Focus_Restore_Function)();
#endif

class SurfaceMonitorClass
{
public:
    SurfaceMonitorClass();
    ~SurfaceMonitorClass();

    void Add_Surface(LPDIRECTDRAWSURFACE new_surface);
    void Remove_Surface(LPDIRECTDRAWSURFACE old_surface);
    bool Got_Surface_Already(LPDIRECTDRAWSURFACE test_surface);
    void Restore_Surfaces();
    void Set_Surface_Focus(bool focus);
    void Release();

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

    BOOL m_surfacesRestored;

private:
    LPDIRECTDRAWSURFACE m_surface[20];
    BOOL m_inFocus;

#ifndef RAPP_STANDALONE
    static void Hook_Add_Surface(SurfaceMonitorClass *ptr, LPDIRECTDRAWSURFACE new_surface);
    static void Hook_Remove_Surface(SurfaceMonitorClass *ptr, LPDIRECTDRAWSURFACE old_surface);
    static bool Hook_Got_Surface_Already(SurfaceMonitorClass *ptr, LPDIRECTDRAWSURFACE test_surface);
    static void Hook_Restore_Surfaces(SurfaceMonitorClass *ptr);
    static void Hook_Set_Surface_Focus(SurfaceMonitorClass *ptr, bool focus);
    static void Hook_Release(SurfaceMonitorClass *ptr);
#endif
};

#ifndef RAPP_STANDALONE
inline void SurfaceMonitorClass::Hook_Me()
{
    Hook_Function(0x005CA1D0, &Hook_Add_Surface);
    Hook_Function(0x005CA230, &Hook_Remove_Surface);
    Hook_Function(0x005CA280, &Hook_Got_Surface_Already);
    Hook_Function(0x005CA2D0, &Hook_Restore_Surfaces);
    Hook_Function(0x005CA370, &Hook_Set_Surface_Focus);
    Hook_Function(0x005CA390, &Hook_Release);
}
#endif

#endif // SURFACEMONITOR_H