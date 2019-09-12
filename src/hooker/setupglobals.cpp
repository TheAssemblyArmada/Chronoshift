#include "hooker.h"

// alloc.cpp
#include "alloc.h"
memerrorhandler_t &g_memoryError = Make_Global<memerrorhandler_t>(0x006B1A2C); // Memory error handler function pointer.
memexithandler_t &g_memoryErrorExit = Make_Global<memexithandler_t>(0x006B1A30);

// ostimer.cpp
#include "ostimer.h"
PlatformTimerClass *&PlatformTimer = Make_Global<PlatformTimerClass *>(0x00665EB0);
BOOL &PlatformTimerClass::s_timerSystemOn = Make_Global<BOOL>(0x006ABF68);
BOOL &PlatformTimerClass::s_inCallback = Make_Global<BOOL>(0x006ABF8C);
void *&PlatformTimerClass::s_threadHandle = Make_Global<void *>(0x006ABF88);

// cdfile.cpp
#include "cdfile.h"
char *CDFileClass::s_rawPath = Make_Pointer<char>(0x006AC06C);
CDFileClass::SearchDriveType *&CDFileClass::s_first =
    *reinterpret_cast<CDFileClass::SearchDriveType **>(0x006AC060); // first entry in the search drive, each entry is linked.
int &CDFileClass::s_currentCDDrive = *reinterpret_cast<int *>(0x006AC064);
int &CDFileClass::s_lastCDDrive = *reinterpret_cast<int *>(0x006AC068);

// gamefile.cpp
#include "gamefile.h"
ARRAY_DEF(0x00635BE4, GameFileClass, g_handles, 10);

// cd.cpp
int &g_requiredCD = *reinterpret_cast<int *>(0x006017D0);
int &g_currentCD = Make_Global<int>(0x006017D4);

// mixfile.cpp
template<>
List<MixFileClass<GameFileClass> *> &
    MixFileClass<GameFileClass>::s_mixList = *reinterpret_cast<List<MixFileClass<GameFileClass> *> *>(0x006ECE30);

// rndstraw.cpp
RandomStraw<RandomClass> &g_cryptRandom = *reinterpret_cast<RandomStraw<RandomClass> *>(0x00667764);

// initvideo.cpp
#ifdef BUILD_WITH_DDRAW
#define DIRECTDRAW_VERSION 0x300
#include <ddraw.h>
LPDIRECTDRAW &g_directDrawObject = Make_Global<LPDIRECTDRAW>(0x006B1490);
LPDIRECTDRAWSURFACE &g_paletteSurface = Make_Global<LPDIRECTDRAWSURFACE>(0x006B18A4);
ARRAY_DEF(0x006B149C, tagPALETTEENTRY, g_paletteEntries, 256);
LPDIRECTDRAWPALETTE &g_palettePtr = Make_Global<LPDIRECTDRAWPALETTE>(0x006B189C);
#endif

// eventhandler.cpp
typedef void (*focusfunc_t)();
unsigned &CCFocusMessage = Make_Global<unsigned>(0x00609BA0);
focusfunc_t &AudioFocusLoss = Make_Global<focusfunc_t>(0x006AC058);
focusfunc_t &MiscFocusLoss = Make_Global<focusfunc_t>(0x006B190C);
focusfunc_t &GBufferFocusLoss = Make_Global<focusfunc_t>(0x006AC278);
focusfunc_t &MiscFocusRestore = Make_Global<focusfunc_t>(0x006B1910);

// keyboard.cpp
class KeyboardClass;
KeyboardClass *&g_keyboard = Make_Global<KeyboardClass *>(0x00666904);

// mouse.cpp
class MouseClass;
MouseClass *&g_mouse = Make_Global<MouseClass *>(0x006AC284);
MouseClass *&g_wwmouse = Make_Global<MouseClass *>(0x00665E08);

// gbuffer.cpp
#include "gbuffer.h"
int &GraphicViewPortClass::ScreenWidth = Make_Global<int>(0x006016B0);
int &GraphicViewPortClass::ScreenHeight = Make_Global<int>(0x006016B4);
GraphicViewPortClass *&g_logicPage = Make_Global<GraphicViewPortClass *>(0x006AC274);
GraphicViewPortClass &g_seenBuff = Make_Global<GraphicViewPortClass>(0x006807A4);
GraphicViewPortClass &g_hidPage = Make_Global<GraphicViewPortClass>(0x006807CC);
GraphicBufferClass &g_visiblePage = Make_Global<GraphicBufferClass>(0x0068065C);
GraphicBufferClass &g_hiddenPage = Make_Global<GraphicBufferClass>(0x00680700);
GraphicBufferClass &g_sysMemPage = Make_Global<GraphicBufferClass>(0x00665E0C);
GraphicBufferClass &g_modeXBuff = Make_Global<GraphicBufferClass>(0x00665EB4);

// textprint.cpp
char *&g_fontPtr = Make_Global<char *>(0x006B1974);
char *&g_fontWidthBlockPtr = Make_Global<char *>(0x006B1978);
int &g_fontYSpacing = Make_Global<int>(0x006B1970);
int &g_fontXSpacing = Make_Global<int>(0x006B196C);
uint8_t &g_fontHeight = Make_Global<uint8_t>(0x0060CE65);
uint8_t &g_fontWidth = Make_Global<uint8_t>(0x0060CE64);
uint8_t *g_colorXlat = Make_Pointer<uint8_t>(0x00609BC8);
uint8_t *byte_608988 = Make_Pointer<uint8_t>(0x00609BD8);

// interpolate.cpp
uint8_t *g_paletteInterpolationTable = reinterpret_cast<uint8_t *>(0x00691734);
BOOL &g_palettesRead = Make_Global<BOOL>(0x00655DA0);
int &g_paletteCounter = Make_Global<int>(0x00655DA4);
int &g_interpolationMode = Make_Global<int>(0x006A1738);
uint8_t **g_interpolatedPalettes = reinterpret_cast<uint8_t **>(0x00655C10);

// pal.cpp
// doesn't link for some reason?
//uint8_t *const g_currentPalette = reinterpret_cast<uint8_t *>(0x0060CE90);

// palette.cpp
class PaletteClass;
PaletteClass &GamePalette = Make_Global<PaletteClass>(0x00669C5C);
PaletteClass &OriginalPalette = Make_Global<PaletteClass>(0x0066A55C);
PaletteClass &CCPalette = Make_Global<PaletteClass>(0x0066995C);
PaletteClass &BlackPalette = Make_Global<PaletteClass>(0x00668F5C);
PaletteClass &WhitePalette = Make_Global<PaletteClass>(0x0066925C);

// shape.cpp
BOOL &g_useBigShapeBuffer = Make_Global<BOOL>(0x006A1784);
BOOL &g_isTheaterShape = Make_Global<BOOL>(0x006A1788);
BOOL &g_originalUseBigShapeBuffer = Make_Global<BOOL>(0x006A178C);
char *&g_bigShapeBufferStart = Make_Global<char *>(0x006A177C);
char *&g_bigShapeBufferPtr = Make_Global<char *>(0x006A1790);
// int &TotalBigShapes;
BOOL &g_reallocShapeBufferFlag = Make_Global<BOOL>(0x006A1798);
char *&g_theaterShapeBufferStart = Make_Global<char *>(0x006A1780);
char *&g_theaterShapeBufferPtr = Make_Global<char *>(0x006A179C);
int &g_totalTheaterShapes = Make_Global<int>(0x006A17A0);
uint32_t **g_keyFrameSlots = reinterpret_cast<uint32_t **>(0x006A17A4);
int &g_totalSlotsUsed = Make_Global<int>(0x006A2F14);
// int &BuildFrameLength;
BOOL &g_useOldShapeDraw = Make_Global<BOOL>(0x00655C08);
uint8_t *&g_shapeBuffer = Make_Global<uint8_t *>(0x0060BE58);
int &g_shapeBufferSize = Make_Global<int>(0x0060BE5C);
unsigned int &g_bigShapeBufferLength = Make_Global<unsigned int>(0x00609D68);
unsigned int &g_theaterShapeBufferLength = Make_Global<unsigned int>(0x00609D6C);
int &g_theaterSlotsUsed = Make_Global<int>(0x00609D70);
int &g_shapeLength = Make_Global<int>(0x006A2F18);

//surfacemonitor.cpp
class SurfaceMonitorClass;
void (*&Misc_Focus_Loss_Function)() = Make_Global<void (*)()>(0x006B190C);
void (*&Misc_Focus_Restore_Function)() = Make_Global<void (*)()>(0x006B1910);
SurfaceMonitorClass &g_allSurfaces = Make_Global<SurfaceMonitorClass>(0x006B18A8);
