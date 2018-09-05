#include "eventhandler.h"

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

unsigned &CCFocusMessage = Make_Global<unsigned>(0x00609BA0);
focusfunc_t &AudioFocusLoss = Make_Global<focusfunc_t>(0x006AC058);
focusfunc_t &MiscFocusLoss = Make_Global<focusfunc_t>(0x006B190C);
focusfunc_t &GBufferFocusLoss = Make_Global<focusfunc_t>(0x006AC278);
focusfunc_t &MiscFocusRestore = Make_Global<focusfunc_t>(0x006B1910);
#else
unsigned CCFocusMessage = 0x432;
focusfunc_t AudioFocusLoss;
focusfunc_t MiscFocusLoss;
focusfunc_t GBufferFocusLoss;
focusfunc_t MiscFocusRestore;
#endif

#if defined PLATFORM_WINDOWS
LRESULT __stdcall WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifndef CHRONOSHIFT_STANDALONE
    LRESULT(__stdcall *func)
    (HWND, UINT, WPARAM, LPARAM) = reinterpret_cast<LRESULT(__stdcall *)(HWND, UINT, WPARAM, LPARAM)>(0x005B373C);

	return func(hwnd, uMsg, wParam, lParam);
#else
    return 0;
#endif
}
#endif
