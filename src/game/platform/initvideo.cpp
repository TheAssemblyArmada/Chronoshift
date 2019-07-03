/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Initialisation functions for graphics API.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "initvideo.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "globals.h"
#include "ostimer.h"
#include "rgb.h"
#include "surfacemonitor.h"

#ifndef CHRONOSHIFT_STANDALONE
#ifdef BUILD_WITH_DDRAW
LPDIRECTDRAW &g_directDrawObject = Make_Global<LPDIRECTDRAW>(0x006B1490);
LPDIRECTDRAWSURFACE &g_paletteSurface = Make_Global<LPDIRECTDRAWSURFACE>(0x006B18A4);
tagPALETTEENTRY *const g_paletteEntries = Make_Pointer<tagPALETTEENTRY>(0x006B149C);
LPDIRECTDRAWPALETTE &g_palettePtr = Make_Global<LPDIRECTDRAWPALETTE>(0x006B189C);
#endif
#else
#ifdef BUILD_WITH_DDRAW
LPDIRECTDRAW g_directDrawObject;
LPDIRECTDRAWSURFACE g_paletteSurface = nullptr;
tagPALETTEENTRY g_paletteEntries[256];
LPDIRECTDRAWPALETTE g_palettePtr;
#endif
#endif

enum VideoCaps
{
    VIDEO_BLITTER = 1 << 0, // Is blitting hardware accelerated?
    VIDEO_BLITTER_ASYNC = 1 << 1, // Is HW blitting asyncronous?
    VIDEO_SYNCPALETTE = 1 << 2, // Can palette changes sync to vertical update?
    VIDEO_BANK_SWITCHED = 1 << 3, // Is video memory bank switched?
    VIDEO_COLOR_FILL = 1 << 4, // Can blitter do filled rectangles?
    VIDEO_NO_HARDWARE_ASSIST = 1 << 5, // No hardware acceleration available.
};

#ifdef BUILD_WITH_DDRAW
/**
 * Processes the result of a DDraw call into something readable.
 *
 * 0x005C8A40
 */
static void Process_DD_Result(HRESULT result, BOOL show_success = false)
{
    // TODO? Popped up meesage box for various errors in original.
    return;
}
#endif

/**
 * Checks if the underlying implementation supports a blit on overlapping video memory.
 *
 * 0x005C9CA0
 */
static void Check_Overlapped_Blit_Capability()
{
    GraphicBufferClass buff;

    g_OverlappedVideoBlits = true;
    buff.Init(64, 64, 0, 0, GBC_VIDEO_MEM);
    buff.Clear();
    buff.Put_Pixel(0, 0, 255);
    buff.Blit(buff, 0, 0, 0, 1, buff.Get_Width(), buff.Get_Height() - 1);

    if (buff.Get_Pixel(0, 5) == 255) {
        g_OverlappedVideoBlits = false;
    }
}

/**
 * Queries the 2D features exposed by the underlying API.
 *
 * 0x005C9F20
 */
static uint32_t Get_Hardware_Caps()
{
#ifdef BUILD_WITH_DDRAW
    if (g_directDrawObject == nullptr) {
        return 0;
    }

    DDCAPS caps;
    caps.dwSize = sizeof(caps);
    HRESULT result = g_directDrawObject->GetCaps(&caps, nullptr);

    if (result != DD_OK) {
        Process_DD_Result(result);

        return 0;
    }

    uint32_t retval = 0;

    if (caps.dwCaps & DDCAPS_BLT) {
        retval |= VIDEO_BLITTER;
    }

    if (caps.dwCaps & DDCAPS_BLTQUEUE) {
        retval |= VIDEO_BLITTER_ASYNC;
    }

    if (caps.dwCaps & DDCAPS_PALETTEVSYNC) {
        retval |= VIDEO_SYNCPALETTE;
    }

    if (caps.dwCaps & DDCAPS_BANKSWITCHED) {
        retval |= VIDEO_BANK_SWITCHED;
    }

    if (caps.dwCaps & DDCAPS_BLTCOLORFILL) {
        retval |= VIDEO_COLOR_FILL;
    }

    if (caps.dwCaps & DDCAPS_NOHARDWARE) {
        retval |= VIDEO_NO_HARDWARE_ASSIST;
    }

    return retval;
#else
    return 0;
#endif
}

/**
 * Queries the amount of video memory still available from the underlying API.
 *
 * 0x005C9EB0
 */
static uint32_t Get_Video_Memory()
{
#ifdef BUILD_WITH_DDRAW
    if (g_directDrawObject == nullptr) {
        return 0;
    }

    DDCAPS caps;
    caps.dwSize = sizeof(caps);

    if (g_directDrawObject->GetCaps(&caps, nullptr) != DD_OK) {
        return 0;
    }

    DEBUG_LOG("Checking free video memory and returning %u bytes.\n", caps.dwVidMemFree);

    return caps.dwVidMemFree;
#else
    return 0;
#endif
}

/**
 * Sets the video mode for the underlying API.
 *
 * 0x005C9D60
 */
BOOL Set_Video_Mode(uintptr_t handle, int w, int h, int bpp)
{
#ifdef BUILD_WITH_DDRAW
    HRESULT result;

    if (g_directDrawObject == nullptr) {
        result = DirectDrawCreate(nullptr, &g_directDrawObject, nullptr);
        Process_DD_Result(result);

        if (result != DD_OK) {
            return false;
        }

        result = g_directDrawObject->SetCooperativeLevel(
            (HWND)handle, (w == 320 ? DDSCL_ALLOWMODEX : 0) | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
        Process_DD_Result(result);
    }

    if (g_directDrawObject->SetDisplayMode(w, h, bpp) != DD_OK) {
        g_directDrawObject->Release();
        g_directDrawObject = nullptr;

        return false;
    }

    // Fixes compiler error, looks like watcom directx sdk header has wrong prototype.
#ifdef COMPILER_WATCOM
    result = g_directDrawObject->CreatePalette(
        DDPCAPS_8BIT | DDPCAPS_ALLOW256, g_paletteEntries, (LPDIRECTDRAWPALETTE)&g_palettePtr, nullptr);
#else
    result = g_directDrawObject->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, g_paletteEntries, &g_palettePtr, nullptr);
#endif
    Process_DD_Result(result);

    if (result != DD_OK) {
        return false;
    }

    Check_Overlapped_Blit_Capability();

    return true;
#endif
}

/**
 * Resets the video mode to an unset state.
 *
 * 0x005C9E60
 */
void Reset_Video_Mode()
{
#ifdef BUILD_WITH_DDRAW
    if (g_directDrawObject) {
        HRESULT result = g_directDrawObject->RestoreDisplayMode();
        Process_DD_Result(result);
        result = g_directDrawObject->Release();
        Process_DD_Result(result);
        g_directDrawObject = nullptr;
    }
#endif
}

/**
 * Initialise the global GraphicBufferClass objects.
 *
 * 0x00552368
 */
BOOL Init_Video()
{
#ifdef BUILD_WITH_DDRAW
    BOOL set_mode =
        Set_Video_Mode((uintptr_t)MainWindow, GraphicViewPortClass::ScreenWidth, GraphicViewPortClass::ScreenHeight, 8);

    if (GraphicViewPortClass::ScreenHeight == 400 && !set_mode) {
        set_mode = Set_Video_Mode((uintptr_t)MainWindow, GraphicViewPortClass::ScreenWidth, 480, 8);

        if (set_mode) {
            GraphicViewPortClass::ScreenHeight = 480;
        }
    }

    if (set_mode) {
        if (GraphicViewPortClass::ScreenWidth == 320) {
            g_visiblePage.Init(GraphicViewPortClass::ScreenWidth, GraphicViewPortClass::ScreenHeight, nullptr, 0, GBC_NONE);
            g_modeXBuff.Init(GraphicViewPortClass::ScreenWidth, GraphicViewPortClass::ScreenHeight, nullptr, 0, GBC_VIDEO_MEM | GBC_VISIBLE);
        } else {
            g_visiblePage.Init(GraphicViewPortClass::ScreenWidth, GraphicViewPortClass::ScreenHeight, nullptr, 0, GBC_VIDEO_MEM | GBC_VISIBLE);
            DDSCAPS caps;
            memset(&caps, 0, sizeof(caps));
            g_visiblePage.Get_DD_Surface()->GetCaps(&caps);

            if (caps.dwCaps & DDSCAPS_SYSTEMMEMORY) {
#ifdef PLATFORM_WINDOWS
                MessageBoxA(MainWindow,
                    "Error - Unable to allocate primary video buffer - aborting.",
                    "Chronoshift",
                    MB_ICONWARNING);
#endif
                DEBUG_LOG("Error - Unable to allocate primary video buffer - aborting.\n");
                delete PlatformTimer;

                return false;
            }

            uint32_t hwcaps = Get_Hardware_Caps();

            if (Get_Video_Memory() < (GraphicViewPortClass::ScreenWidth * GraphicViewPortClass::ScreenHeight) || !(hwcaps & VIDEO_BLITTER)
                || (hwcaps & VIDEO_NO_HARDWARE_ASSIST) || !VideoBackBufferAllowed) {
                g_hiddenPage.Init(GraphicViewPortClass::ScreenWidth, GraphicViewPortClass::ScreenHeight, 0, 0, GBC_NONE);
            } else {
                g_hiddenPage.Init(GraphicViewPortClass::ScreenWidth, GraphicViewPortClass::ScreenHeight, 0, 0, GBC_VIDEO_MEM);
                DDSCAPS caps;
                memset(&caps, 0, sizeof(caps));
                g_hiddenPage.Get_DD_Surface()->GetCaps(&caps);

                if (!(caps.dwCaps & DDSCAPS_SYSTEMMEMORY)) {
                    g_visiblePage.Attach_DD_Surface(&g_hiddenPage);
                } else {
                    g_allSurfaces.Remove_Surface(g_hiddenPage.Get_DD_Surface());
                    g_hiddenPage.Get_DD_Surface()->Release();
                    g_hiddenPage.Init(GraphicViewPortClass::ScreenWidth, GraphicViewPortClass::ScreenHeight, 0, 0, GBC_NONE);
                }
            }
        }

        GraphicViewPortClass::ScreenHeight = 400;
        int y_offset = 0;

        if (g_visiblePage.Get_Height() == 480) {
            g_seenBuff.Attach(&g_visiblePage, 0, 80, 640, 400);
            y_offset = 80;
        } else {
            g_seenBuff.Attach(&g_visiblePage, 0, 0, 640, 400);
        }

        g_hidPage.Attach(&g_hiddenPage, 0, y_offset, 640, 400);

        return true;
    } else {
#ifdef PLATFORM_WINDOWS
        MessageBoxA(MainWindow, "Error - Unable to set the video mode.", "Chronoshift", MB_ICONWARNING);
#endif
        DEBUG_LOG("Error - Unable to set the video mode.\n");
        delete PlatformTimer;
    }

    return false;
#else
    return true;
#endif
}

/**
 * Set the API specific palette to use for blitting 8bit graphics to the display.
 *
 * 0x005CA070
 */
void Set_Video_Palette(void *pal)
{
#ifdef BUILD_WITH_DDRAW
    static BOOL _first_palette_set = false;

    if (pal == nullptr || g_directDrawObject == nullptr || g_paletteSurface == nullptr) {
        return;
    }

    RGBClass *rgb = static_cast<RGBClass *>(pal);

    for (int i = 0; i < 256; ++i) {
        g_paletteEntries[i].peRed = rgb[i].Get_Red() << 2;
        g_paletteEntries[i].peGreen = rgb[i].Get_Green() << 2;
        g_paletteEntries[i].peBlue = rgb[i].Get_Blue() << 2;
    }

    if (!_first_palette_set) {
        g_paletteSurface->SetPalette(g_palettePtr);
        _first_palette_set = true;
    }

    g_palettePtr->SetEntries(0, 0, 256, g_paletteEntries);
#endif
}
