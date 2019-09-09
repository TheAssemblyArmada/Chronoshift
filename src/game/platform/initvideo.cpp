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

#ifdef GAME_DLL
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
    const char *errorstr = nullptr;

    switch (result) {
        case DDERR_NODIRECTDRAWSUPPORT:
            errorstr = "No DirectDraw support possible with current display driver.";
            break;
        case DDERR_CANTLOCKSURFACE:
            errorstr = "Unable to lock surface because no driver exists which can supply a pointer to the surface.";
            break;
        case DDERR_INVALIDSURFACETYPE:
            errorstr = "The requested action could not be performed because the surface was of the wrong type.";
            break;
        case DDERR_ALREADYINITIALIZED:
            errorstr = "This object is already initialized.";
            break;
        case DDERR_BLTFASTCANTCLIP:
            errorstr = "Return if a clipper object is attached to the source surface passed into a BltFast call.";
            break;
        case DDERR_CANNOTATTACHSURFACE:
            errorstr = "This surface can not be attached to the requested surface.";
            break;
        case DDERR_CANNOTDETACHSURFACE:
            errorstr = "This surface can not be detached from the requested surface.";
            break;
        case DDERR_CANTCREATEDC:
            errorstr = "Windows can not create any more DCs.";
            break;
        case DDERR_CANTDUPLICATE:
            errorstr = "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.";
            break;
        case DDERR_CLIPPERISUSINGHWND:
            errorstr = "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.";
            break;
        case DDERR_COLORKEYNOTSET:
            errorstr = "No src color key specified for this operation.";
            break;
        case DDERR_CURRENTLYNOTAVAIL:
            errorstr = "Support is currently not available.";
            break;
        case DDERR_DIRECTDRAWALREADYCREATED:
            errorstr = "A DirectDraw object representing this driver has already been created for this process.";
            break;
        case DDERR_EXCEPTION:
            errorstr = "An exception was encountered while performing the requested operation.";
            break;
        case DDERR_EXCLUSIVEMODEALREADYSET:
            errorstr = "An attempt was made to set the cooperative level when it was already set to exclusive.";
            break;
        case DDERR_GENERIC:
            errorstr = "Generic failure.";
            break;
        case DDERR_HEIGHTALIGN:
            errorstr = "Height of rectangle provided is not a multiple of reqd alignment.";
            break;
        case DDERR_HWNDALREADYSET:
            errorstr =
                "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or "
                "palettes created.";
            break;
        case DDERR_HWNDSUBCLASSED:
            errorstr =
                "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring "
                "state.";
            break;
        case DDERR_IMPLICITLYCREATED:
            errorstr = "This surface can not be restored because it is an implicitly created surface.";
            break;
        case DDERR_INCOMPATIBLEPRIMARY:
            errorstr = "Unable to match primary surface creation request with existing primary surface.";
            break;
        case DDERR_INVALIDCAPS:
            errorstr = "One or more of the caps bits passed to the callback are incorrect.";
            break;
        case DDERR_INVALIDCLIPLIST:
            errorstr = "DirectDraw does not support the provided cliplist.";
            break;
        case DDERR_INVALIDDIRECTDRAWGUID:
            errorstr = "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.";
            break;
        case DDERR_INVALIDMODE:
            errorstr = "DirectDraw does not support the requested mode.";
            break;
        case DDERR_INVALIDOBJECT:
            errorstr = "DirectDraw received a pointer that was an invalid DIRECTDRAW object.";
            break;
        case DDERR_INVALIDPARAMS:
            errorstr = "One or more of the parameters passed to the function are incorrect.";
            break;
        case DDERR_INVALIDPIXELFORMAT:
            errorstr = "The pixel format was invalid as specified.";
            break;
        case DDERR_INVALIDPOSITION:
            errorstr =
                "Returned when the position of the overlay on the destination is no longer legal for that destination.";
            break;
        case DDERR_INVALIDRECT:
            errorstr = "Rectangle provided was invalid.";
            break;
        case DDERR_LOCKEDSURFACES:
            errorstr = "Operation could not be carried out because one or more surfaces are locked.";
            break;
        case DDERR_NO3D:
            errorstr = "There is no 3D present.";
            break;
        case DDERR_NOALPHAHW:
            errorstr =
                "Operation could not be carried out because there is no alpha accleration hardware present or available.";
            break;
        case DDERR_NOBLTHW:
            errorstr = "No blitter hardware present.";
            break;
        case DDERR_NOCLIPLIST:
            errorstr = "No cliplist available.";
            break;
        case DDERR_NOCLIPPERATTACHED:
            errorstr = "No clipper object attached to surface object.";
            break;
        case DDERR_NOCOLORCONVHW:
            errorstr =
                "Operation could not be carried out because there is no color conversion hardware present or available.";
            break;
        case DDERR_NOCOLORKEY:
            errorstr = "Surface doesn't currently have a color key";
            break;
        case DDERR_NOCOLORKEYHW:
            errorstr =
                "Operation could not be carried out because there is no hardware support of the destination color key.";
            break;
        case DDERR_NOCOOPERATIVELEVELSET:
            errorstr = "Create function called without DirectDraw object method SetCooperativeLevel being called.";
            break;
        case DDERR_NODC:
            errorstr = "No DC was ever created for this surface.";
            break;
        case DDERR_NODDROPSHW:
            errorstr = "No DirectDraw ROP hardware.";
            break;
        case DDERR_NODIRECTDRAWHW:
            errorstr =
                "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.";
            break;
        case DDERR_NOEMULATION:
            errorstr = "Software emulation not available.";
            break;
        case DDERR_NOEXCLUSIVEMODE:
            errorstr =
                "Operation requires the application to have exclusive mode but the application does not have exclusive "
                "mode.";
            break;
        case DDERR_NOFLIPHW:
            errorstr = "Flipping visible surfaces is not supported.";
            break;
        case DDERR_NOGDI:
            errorstr = "There is no GDI present.";
            break;
        case DDERR_NOHWND:
            errorstr =
                "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.";
            break;
        case DDERR_NOMIRRORHW:
            errorstr = "Operation could not be carried out because there is no hardware present or available.";
            break;
        case DDERR_NOOVERLAYDEST:
            errorstr =
                "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to "
                "establish a destination.";
            break;
        case DDERR_NOOVERLAYHW:
            errorstr = "Operation could not be carried out because there is no overlay hardware present or available.";
            break;
        case DDERR_NOPALETTEATTACHED:
            errorstr = "No palette object attached to this surface.";
            break;
        case DDERR_NOPALETTEHW:
            errorstr = "No hardware support for 16 or 256 color palettes.";
            break;
        case DDERR_NORASTEROPHW:
            errorstr =
                "Operation could not be carried out because there is no appropriate raster op hardware present or "
                "available.";
            break;
        case DDERR_NOROTATIONHW:
            errorstr = "Operation could not be carried out because there is no rotation hardware present or available.";
            break;
        case DDERR_NOSTRETCHHW:
            errorstr = "Operation could not be carried out because there is no hardware support for stretching.";
            break;
        case DDERR_NOT4BITCOLOR:
            errorstr =
                "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.";
            break;
        case DDERR_NOT4BITCOLORINDEX:
            errorstr =
                "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color "
                "index palette.";
            break;
        case DDERR_NOT8BITCOLOR:
            errorstr = "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.";
            break;
        case DDERR_NOTAOVERLAYSURFACE:
            errorstr = "Returned when an overlay member is called for a non-overlay surface.";
            break;
        case DDERR_NOTEXTUREHW:
            errorstr =
                "Operation could not be carried out because there is no texture mapping hardware present or available.";
            break;
        case DDERR_NOTFLIPPABLE:
            errorstr = "An attempt has been made to flip a surface that is not flippable.";
            break;
        case DDERR_NOTFOUND:
            errorstr = "Requested item was not found.";
            break;
        case DDERR_NOTLOCKED:
            errorstr =
                "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, "
                "has been attempted.";
            break;
        case DDERR_NOTPALETTIZED:
            errorstr = "The surface being used is not a palette-based surface.";
            break;
        case DDERR_NOVSYNCHW:
            errorstr =
                "Operation could not be carried out because there is no hardware support for vertical blank synchronized "
                "operations.";
            break;
        case DDERR_NOZBUFFERHW:
            errorstr = "Operation could not be carried out because there is no hardware support for zbuffer blitting.";
            break;
        case DDERR_NOZOVERLAYHW:
            errorstr =
                "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z "
                "layering of overlays.";
            break;
        case DDERR_OUTOFCAPS:
            errorstr = "The hardware needed for the requested operation has already been allocated.";
            break;
        case DDERR_OUTOFMEMORY:
            errorstr = "There is not enough memory to perform the operation.";
            break;
        case DDERR_OUTOFVIDEOMEMORY:
            errorstr = "DirectDraw does not have enough memory to perform the operation.";
            break;
        case DDERR_OVERLAYCANTCLIP:
            errorstr = "The hardware does not support clipped overlays.";
            break;
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
            errorstr = "Can only have ony color key active at one time for overlays.";
            break;
        case DDERR_OVERLAYNOTVISIBLE:
            errorstr = "Returned when GetOverlayPosition is called on a hidden overlay.";
            break;
        case DDERR_PALETTEBUSY:
            errorstr = "Access to this palette is being refused because the palette is already locked by another thread.";
            break;
        case DDERR_PRIMARYSURFACEALREADYEXISTS:
            errorstr = "This process already has created a primary surface.";
            break;
        case DDERR_REGIONTOOSMALL:
            errorstr = "Region passed to Clipper::GetClipList is too small.";
            break;
        case DDERR_SURFACEALREADYATTACHED:
            errorstr = "This surface is already attached to the surface it is being attached to.";
            break;
        case DDERR_SURFACEALREADYDEPENDENT:
            errorstr = "This surface is already a dependency of the surface it is being made a dependency of.";
            break;
        case DDERR_SURFACEBUSY:
            errorstr = "Access to this surface is being refused because the surface is already locked by another thread.";
            break;
        case DDERR_SURFACEISOBSCURED:
            errorstr = "Access to surface refused because the surface is obscured.";
            break;
        case DDERR_SURFACELOST:
            errorstr =
                "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object "
                "representing this surface should have Restore called on it.";
            break;
        case DDERR_SURFACENOTATTACHED:
            errorstr = "The requested surface is not attached.";
            break;
        case DDERR_TOOBIGHEIGHT:
            errorstr = "Height requested by DirectDraw is too large.";
            break;
        case DDERR_TOOBIGSIZE:
            errorstr = "Size requested by DirectDraw is too large, but the individual height and width are OK.";
            break;
        case DDERR_TOOBIGWIDTH:
            errorstr = "Width requested by DirectDraw is too large.";
            break;
        case DDERR_UNSUPPORTED:
            errorstr = "Function call not supported.";
            break;
        case DDERR_UNSUPPORTEDFORMAT:
            errorstr = "FOURCC format requested is unsupported by DirectDraw.";
            break;
        case DDERR_UNSUPPORTEDMASK:
            errorstr = "Bitmask in the pixel format requested is unsupported by DirectDraw.";
            break;
        case DDERR_VERTICALBLANKINPROGRESS:
            errorstr = "Vertical blank is in progress.";
            break;
        case DDERR_WASSTILLDRAWING:
            errorstr =
                "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is "
                "incomplete.";
            break;
        case DDERR_WRONGMODE:
            errorstr = "This surface can not be restored because it was created in a different mode.";
            break;
        case DDERR_XALIGN:
            errorstr = "Rectangle provided was not horizontally aligned on required boundary.";
            break;
        case DD_OK:
            if (show_success) {
                errorstr = "Direct Draw request went ok.";
            }
            break;
        default:
            errorstr = "Unrecognized error value.";
            break;
    };

    if (errorstr != nullptr) {
        MessageBoxA(MainWindow, errorstr, "Note", MB_ICONEXCLAMATION | MB_OK);
    }
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
