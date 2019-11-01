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
#include "surfacemonitor.h"
#include "pal.h"

#ifndef GAME_DLL
void (*MiscFocusLoss)() = nullptr;
void (*MiscFocusRestore)() = nullptr;
SurfaceMonitorClass g_allSurfaces;
#endif

SurfaceMonitorClass::SurfaceMonitorClass()
{
#ifdef BUILD_WITH_DDRAW
    for (int i = 0; i < SURFACE_COUNT; ++i) {
        m_Surface[i] = nullptr;
    }
#endif
    m_InFocus = false;
    m_SurfacesRestored = false;
}

SurfaceMonitorClass::~SurfaceMonitorClass()
{
    Release();
}

#ifdef BUILD_WITH_DDRAW
void SurfaceMonitorClass::Add_Surface(LPDIRECTDRAWSURFACE new_surface)
{
    if (!Got_Surface_Already(new_surface)) {
        for (int i = 0; i < ARRAY_SIZE(m_Surface); ++i) {
            if (m_Surface[i] == nullptr) {
                m_Surface[i] = new_surface;
                return;
            }
        }
    }
}

void SurfaceMonitorClass::Remove_Surface(LPDIRECTDRAWSURFACE old_surface)
{
    for (int i = 0; i < ARRAY_SIZE(m_Surface); ++i) {
        if (m_Surface[i] == old_surface) {
            m_Surface[i] = nullptr;
            return;
        }
    }
}

bool SurfaceMonitorClass::Got_Surface_Already(LPDIRECTDRAWSURFACE test_surface)
{
    for (int i = 0; i < ARRAY_SIZE(m_Surface); ++i) {
        if (m_Surface[i] == test_surface) {
            return true;
        }
    }
    return false;
}
#endif

void SurfaceMonitorClass::Restore_Surfaces()
{
    if (m_InFocus) {
#ifdef BUILD_WITH_DDRAW
        for (int i = 0; i < SURFACE_COUNT; ++i) {
            if (m_Surface[i] && m_Surface[i]->Restore()) {
                if (MiscFocusLoss != nullptr) {
                    MiscFocusLoss();
                }
                return;
            }
        }
#endif
        if (MiscFocusRestore != nullptr) {
            MiscFocusRestore();
        }
        m_SurfacesRestored = true;
        Set_Palette(g_currentPalette);
    }
}

void SurfaceMonitorClass::Set_Surface_Focus(bool in_focus)
{
    m_InFocus = in_focus;
}

void SurfaceMonitorClass::Release()
{
#ifdef BUILD_WITH_DDRAW
    for (int i = 0; i < SURFACE_COUNT; ++i) {
        if (m_Surface[i]) {
            m_Surface[i]->Release();
            m_Surface[i] = nullptr;
        }
    }
#endif
}
