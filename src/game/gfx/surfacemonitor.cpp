#include "surfacemonitor.h"
#include "pal.h"

#ifndef CHRONOSHIFT_STANDALONE
void (*&Misc_Focus_Loss_Function)() = Make_Global<void(*)()>(0x006B190C);
void (*&Misc_Focus_Restore_Function)() = Make_Global<void(*)()>(0x006B1910);
#else
void (*Misc_Focus_Loss_Function)() = nullptr;
void (*Misc_Focus_Restore_Function)() = nullptr;
#endif

SurfaceMonitorClass::SurfaceMonitorClass()
{
    for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
        m_surface[i] = nullptr;
    }
    m_inFocus = false;
    m_surfacesRestored = false;
}

SurfaceMonitorClass::~SurfaceMonitorClass()
{
    Release();
}

void SurfaceMonitorClass::Add_Surface(LPDIRECTDRAWSURFACE new_surface)
{
    if (!Got_Surface_Already(new_surface)) {
        for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
            if (m_surface[i] == nullptr) {
                m_surface[i] = new_surface;
                return;
            }
        }
    }
}

void SurfaceMonitorClass::Remove_Surface(LPDIRECTDRAWSURFACE old_surface)
{
    for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
        if (m_surface[i] == old_surface) {
            m_surface[i] = nullptr;
            return;
        }
    }
}

bool SurfaceMonitorClass::Got_Surface_Already(LPDIRECTDRAWSURFACE test_surface)
{
    for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
        if (m_surface[i] == test_surface) {
            return true;
        }
    }
    return false;
}

void SurfaceMonitorClass::Restore_Surfaces()
{
    if (m_inFocus) {
        for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
            if (m_surface[i] && m_surface[i]->Restore()) {
                if (Misc_Focus_Loss_Function) {
                    Misc_Focus_Loss_Function();
                }
                return;
            }
        }
        if (Misc_Focus_Restore_Function) {
            Misc_Focus_Restore_Function();
        }
        m_surfacesRestored = true;
        Set_Palette(g_currentPalette);
    }
}

void SurfaceMonitorClass::Set_Surface_Focus(bool in_focus)
{
    m_inFocus = in_focus;
}

void SurfaceMonitorClass::Release()
{
    for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
        if (m_surface[i]) {
            m_surface[i]->Release();
            m_surface[i] = nullptr;
        }
    }
}

#ifndef CHRONOSHIFT_STANDALONE
void SurfaceMonitorClass::Hook_Add_Surface(SurfaceMonitorClass *ptr, LPDIRECTDRAWSURFACE new_surface)
{
    return ptr->SurfaceMonitorClass::Add_Surface(new_surface);
}
void SurfaceMonitorClass::Hook_Remove_Surface(SurfaceMonitorClass *ptr, LPDIRECTDRAWSURFACE old_surface)
{
    return ptr->SurfaceMonitorClass::Remove_Surface(old_surface);
}
bool SurfaceMonitorClass::Hook_Got_Surface_Already(SurfaceMonitorClass *ptr, LPDIRECTDRAWSURFACE test_surface)
{
    return ptr->SurfaceMonitorClass::Got_Surface_Already(test_surface);
}
void SurfaceMonitorClass::Hook_Restore_Surfaces(SurfaceMonitorClass *ptr)
{
    return ptr->SurfaceMonitorClass::Restore_Surfaces();
}
void SurfaceMonitorClass::Hook_Set_Surface_Focus(SurfaceMonitorClass *ptr, bool focus)
{
    return ptr->SurfaceMonitorClass::Set_Surface_Focus(focus);
}
void SurfaceMonitorClass::Hook_Release(SurfaceMonitorClass *ptr)
{
    return ptr->SurfaceMonitorClass::Release();
}
#endif