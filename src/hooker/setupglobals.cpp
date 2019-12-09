#include "hooker.h"

// alloc.cpp
#include "alloc.h"
memerrorhandler_t &g_MemoryError = Make_Global<memerrorhandler_t>(0x006B1A2C); // Memory error handler function pointer.
memexithandler_t &g_MemoryErrorExit = Make_Global<memexithandler_t>(0x006B1A30);

// ostimer.cpp
#include "ostimer.h"
PlatformTimerClass *&g_PlatformTimer = Make_Global<PlatformTimerClass *>(0x00665EB0);
BOOL &PlatformTimerClass::s_timerSystemOn = Make_Global<BOOL>(0x006ABF68);
BOOL &PlatformTimerClass::s_inCallback = Make_Global<BOOL>(0x006ABF8C);
void *&PlatformTimerClass::s_threadHandle = Make_Global<void *>(0x006ABF88);

// cdfile.cpp
#include "cdfile.h"
char *CDFileClass::s_RawPath = Make_Pointer<char>(0x006AC06C);
CDFileClass::SearchDriveType *&CDFileClass::s_First =
    *reinterpret_cast<CDFileClass::SearchDriveType **>(0x006AC060); // first entry in the search drive, each entry is linked.
int &CDFileClass::s_CurrentCDDrive = *reinterpret_cast<int *>(0x006AC064);
int &CDFileClass::s_LastCDDrive = *reinterpret_cast<int *>(0x006AC068);

// gamefile.cpp
#include "gamefile.h"
ARRAY_DEF(0x00635BE4, GameFileClass, g_Handles, 10);

// cd.cpp
int &g_RequiredCD = *reinterpret_cast<int *>(0x006017D0);
int &g_CurrentCD = Make_Global<int>(0x006017D4);

// mixfile.cpp
template<>
List<MixFileClass<GameFileClass> *> &
    MixFileClass<GameFileClass>::s_MixList = *reinterpret_cast<List<MixFileClass<GameFileClass> *> *>(0x006ECE30);

// rndstraw.cpp
RandomStraw<RandomClass> &g_CryptRandom = *reinterpret_cast<RandomStraw<RandomClass> *>(0x00667764);

// initvideo.cpp
#ifdef BUILD_WITH_DDRAW
#define DIRECTDRAW_VERSION 0x300
#include <ddraw.h>
LPDIRECTDRAW &g_DirectDrawObject = Make_Global<LPDIRECTDRAW>(0x006B1490);
LPDIRECTDRAWSURFACE &g_PaletteSurface = Make_Global<LPDIRECTDRAWSURFACE>(0x006B18A4);
ARRAY_DEF(0x006B149C, tagPALETTEENTRY, g_PaletteEntries, 256);
LPDIRECTDRAWPALETTE &g_PalettePtr = Make_Global<LPDIRECTDRAWPALETTE>(0x006B189C);
#endif

// eventhandler.cpp
typedef void (*focusfunc_t)();
unsigned &g_CCFocusMessage = Make_Global<unsigned>(0x00609BA0);
focusfunc_t &g_AudioFocusLoss = Make_Global<focusfunc_t>(0x006AC058);
focusfunc_t &g_GBufferFocusLoss = Make_Global<focusfunc_t>(0x006AC278);

// keyboard.cpp
class KeyboardClass;
KeyboardClass *&g_Keyboard = Make_Global<KeyboardClass *>(0x00666904);

// mouse.cpp
class MouseClass;
MouseClass *&g_Mouse = Make_Global<MouseClass *>(0x006AC284);
MouseClass *&g_WWMouse = Make_Global<MouseClass *>(0x00665E08);

// gbuffer.cpp
#include "gbuffer.h"
int &GraphicViewPortClass::s_ScreenWidth = Make_Global<int>(0x006016B0);
int &GraphicViewPortClass::s_ScreenHeight = Make_Global<int>(0x006016B4);
GraphicViewPortClass *&g_LogicPage = Make_Global<GraphicViewPortClass *>(0x006AC274);
GraphicViewPortClass &g_SeenBuff = Make_Global<GraphicViewPortClass>(0x006807A4);
GraphicViewPortClass &g_HidPage = Make_Global<GraphicViewPortClass>(0x006807CC);
GraphicBufferClass &g_VisiblePage = Make_Global<GraphicBufferClass>(0x0068065C);
GraphicBufferClass &g_HiddenPage = Make_Global<GraphicBufferClass>(0x00680700);
GraphicBufferClass &g_SysMemPage = Make_Global<GraphicBufferClass>(0x00665E0C);
GraphicBufferClass &g_ModeXBuff = Make_Global<GraphicBufferClass>(0x00665EB4);

// textprint.cpp
char *&g_FontPtr = Make_Global<char *>(0x006B1974);
char *&g_fontWidthBlockPtr = Make_Global<char *>(0x006B1978);
int &g_FontYSpacing = Make_Global<int>(0x006B1970);
int &g_FontXSpacing = Make_Global<int>(0x006B196C);
uint8_t &g_FontHeight = Make_Global<uint8_t>(0x0060CE65);
uint8_t &g_FontWidth = Make_Global<uint8_t>(0x0060CE64);
uint8_t *g_ColorXlat = Make_Pointer<uint8_t>(0x00609BC8);
uint8_t *byte_608988 = Make_Pointer<uint8_t>(0x00609BD8);

// interpolate.cpp
uint8_t *g_PaletteInterpolationTable = reinterpret_cast<uint8_t *>(0x00691734);
BOOL &g_PalettesRead = Make_Global<BOOL>(0x00655DA0);
int &g_PaletteCounter = Make_Global<int>(0x00655DA4);
int &g_InterpolationMode = Make_Global<int>(0x006A1738);
uint8_t **g_InterpolatedPalettes = reinterpret_cast<uint8_t **>(0x00655C10);

// pal.cpp
uint8_t *g_CurrentPalette = reinterpret_cast<uint8_t *>(0x0060CE90);

// palette.cpp
class PaletteClass;
PaletteClass &g_GamePalette = Make_Global<PaletteClass>(0x00669C5C);
PaletteClass &g_OriginalPalette = Make_Global<PaletteClass>(0x0066A55C);
PaletteClass &g_CCPalette = Make_Global<PaletteClass>(0x0066995C);
PaletteClass &g_BlackPalette = Make_Global<PaletteClass>(0x00668F5C);
PaletteClass &g_WhitePalette = Make_Global<PaletteClass>(0x0066925C);

// shape.cpp
BOOL &g_UseBigShapeBuffer = Make_Global<BOOL>(0x006A1784);
BOOL &g_IsTheaterShape = Make_Global<BOOL>(0x006A1788);
BOOL &g_OriginalUseBigShapeBuffer = Make_Global<BOOL>(0x006A178C);
char *&g_BigShapeBufferStart = Make_Global<char *>(0x006A177C);
char *&g_BigShapeBufferPtr = Make_Global<char *>(0x006A1790);
// int &TotalBigShapes;
BOOL &g_ReallocShapeBufferFlag = Make_Global<BOOL>(0x006A1798);
char *&g_TheaterShapeBufferStart = Make_Global<char *>(0x006A1780);
char *&g_TheaterShapeBufferPtr = Make_Global<char *>(0x006A179C);
int &g_TotalTheaterShapes = Make_Global<int>(0x006A17A0);
uint32_t **g_KeyFrameSlots = reinterpret_cast<uint32_t **>(0x006A17A4);
int &g_TotalSlotsUsed = Make_Global<int>(0x006A2F14);
// int &BuildFrameLength;
BOOL &g_UseOldShapeDraw = Make_Global<BOOL>(0x00655C08);
uint8_t *&g_ShapeBuffer = Make_Global<uint8_t *>(0x0060BE58);
int &g_ShapeBufferSize = Make_Global<int>(0x0060BE5C);
unsigned int &g_BigShapeBufferLength = Make_Global<unsigned int>(0x00609D68);
unsigned int &g_TheaterShapeBufferLength = Make_Global<unsigned int>(0x00609D6C);
int &g_TheaterSlotsUsed = Make_Global<int>(0x00609D70);
int &g_ShapeLength = Make_Global<int>(0x006A2F18);

// surfacemonitor.cpp
class SurfaceMonitorClass;
void (*&g_MiscFocusLoss)() = Make_Global<void (*)()>(0x006B190C);
void (*&g_MiscFocusRestore)() = Make_Global<void (*)()>(0x006B1910);
SurfaceMonitorClass &g_AllSurfaces = Make_Global<SurfaceMonitorClass>(0x006B18A8);

// dialog.cpp
#include "dialog.h"
WindowType *g_WindowList = reinterpret_cast<WindowType *>(0x006016CC);
unsigned &g_Window = Make_Global<unsigned>(0x006B1A08);
int &g_WindowColumns = Make_Global<int>(0x0060CE6C);
int &g_WindowLines = Make_Global<int>(0x0060CE70);
int &g_WindowWidth = Make_Global<int>(0x0060CE74);
int &g_WPos = Make_Global<int>(0x006B19DC);
int &g_WinX = Make_Global<int>(0x006B19F8);
int &g_WinY = Make_Global<int>(0x006B19FC);
int &g_WinW = Make_Global<int>(0x0060CE80);
int &g_WinH = Make_Global<int>(0x0060CE7C);
int &g_WinC = Make_Global<int>(0x0060CE78);
int &g_WinB = Make_Global<int>(0x006B19F4);
int &g_WinCx = Make_Global<int>(0x006B1A00);
int &g_WinCy = Make_Global<int>(0x006B1A04);
int &g_ScrollCounter = Make_Global<int>(0x006B197C);

// gadget.cpp
#include "gadget.h"
RemapControlType *&GadgetClass::s_ColorScheme = Make_Global<RemapControlType *>(0x00601694);
GadgetClass *&GadgetClass::s_StuckOn = Make_Global<GadgetClass *>(0x0065D7B0);
GadgetClass *&GadgetClass::s_LastList = Make_Global<GadgetClass *>(0x0065D7B4);
GadgetClass *&GadgetClass::s_Focused = Make_Global<GadgetClass *>(0x0065D7B8);

// globals.cpp
#include "globals.h"
HWND &g_MainWindow = Make_Global<HWND>(0x006B1498);
HMODULE &g_ProgramInstance = Make_Global<HMODULE>(0x0068A4BC);
int &g_INIFormat = Make_Global<int>(0x00665DE8);
int &g_GameFrame = Make_Global<int>(0x006680C4);
BOOL &g_GameInFocus = Make_Global<BOOL>(0x00665F64);
BOOL &g_InMovie = Make_Global<BOOL>(0x00665F58);
BOOL &g_InMapEditor = Make_Global<BOOL>(0x0065D7E8);
char *&g_Metal12FontPtr = Make_Global<char *>(0x0066992C);
char *&g_MapFontPtr = Make_Global<char *>(0x00669930);
char *&g_Font6Ptr = Make_Global<char *>(0x0066993C);
char *&g_GradFont6Ptr = Make_Global<char *>(0x00669954);
char *&g_EditorFont = Make_Global<char *>(0x00669940);
char *&g_Font8Ptr = Make_Global<char *>(0x00669944);
char *&g_Font3Ptr = Make_Global<char *>(0x00669938);
char *&g_ScoreFontPtr = Make_Global<char *>(0x00669950);
char *&g_FontLEDPtr = Make_Global<char *>(0x00669948);
char *&g_VCRFontPtr = Make_Global<char *>(0x0066994C);
char *&g_TypeFontPtr = Make_Global<char *>(0x00669934);
BOOL &g_AllowVoice = Make_Global<BOOL>(0x006016C0);
BOOL &g_GameActive = Make_Global<BOOL>(0x00669924);
int &g_ScenarioInit = Make_Global<int>(0x00669914);
BOOL &g_DebugUnshroud = Make_Global<BOOL>(0x0065D7F0);
BOOL &g_DebugQuiet = Make_Global<BOOL>(0x0065D7CC);
BOOL &g_ScoresPresent = Make_Global<BOOL>(0x006680C0);
BOOL &g_StreamLowImpact = Make_Global<BOOL>(0x006ABFCC);
BOOL &g_CancelCurrentMsgBox = Make_Global<BOOL>(0x00685170);
BOOL &g_VideoBackBufferAllowed = Make_Global<BOOL>(0x00604DE8);
BOOL &AllowHardwareFilledBlits = Make_Global<BOOL>(0x0060BA70);
BOOL &g_OverlappedVideoBlits = Make_Global<BOOL>(0x0060BE54);
BOOL &g_SoundOn = Make_Global<BOOL>(0x006807F4);
BOOL &g_SlowPalette = Make_Global<BOOL>(0x006678E4);
BOOL &g_BreakoutAllowed = Make_Global<BOOL>(0x006016B8);
SpecialDialogType &g_SpecialDialog = Make_Global<SpecialDialogType>(0x00680834);
BOOL &g_IsTanyaDead = Make_Global<BOOL>(0x00665DD4);
BOOL &g_SaveTanya = Make_Global<BOOL>(0x00665DD8);
BOOL &g_AntsEnabled = Make_Global<BOOL>(0x00665DDC);
BOOL &g_AftermathMultiplayer = Make_Global<BOOL>(0x00680538);
BOOL &g_NewUnitsEnabled = Make_Global<BOOL>(0x00665DE0);
BOOL &g_SecretUnitsEnabled = Make_Global<BOOL>(0x00665DE4);
BOOL &g_NewMaxAheadFrame1 = Make_Global<BOOL>(0x00680530);
BOOL &g_NewMaxAheadFrame2 = Make_Global<BOOL>(0x00680534);
int &g_UnitBuildPenalty = Make_Global<int>(0x006017D8);
BOOL &g_TimeQuake = Make_Global<BOOL>(0x00665DEC);
BOOL &g_PendingTimeQuake = Make_Global<BOOL>(0x00665DF0);
int &g_TimeQuakeCenter = Make_Global<int>(0x00665DF4);
BOOL &g_GameStatisticsPacketSent = Make_Global<BOOL>(0x006ABBB8);
BOOL &g_PlayerWins = Make_Global<BOOL>(0x006680C8);
BOOL &g_PlayerLoses = Make_Global<BOOL>(0x006680CC);
BOOL &g_PlayerRestarts = Make_Global<BOOL>(0x006680D0);
BOOL g_PlayerAborts = false;
int &g_BuildLevel = Make_Global<int>(0x006016C8);
BOOL &g_FormMove = Make_Global<BOOL>(0x0065E0CC);
SpeedType &g_FormSpeed = Make_Global<SpeedType>(0x0065E0D0);
MPHType &g_FormMaxSpeed = Make_Global<MPHType>(0x0065E0D1);
BOOL &g_MouseInstalled = Make_Global<BOOL>(0x00680838);
int &g_Seed = Make_Global<int>(0x00680654);
int &g_CustomSeed = Make_Global<int>(0x00680658);
int &g_RandNumb = Make_Global<int>(0x0060D61C);
int &g_SpareTicks = Make_Global<int>(0x006670E8);
int &g_ReadyToQuit = Make_Global<int>(0x00680880);
HousesType &g_Whom = Make_Global<HousesType>(0x00669910);
void *&g_WakeShapes = Make_Global<void *>(0x0068D2DC);
void *&g_TurretShapes = Make_Global<void *>(0x0068D2E0);
void *&g_SamShapes = Make_Global<void *>(0x0068D2E4);
void *&g_MGunShapes = Make_Global<void *>(0x0068D2E8);
const char **g_TutorialText = Make_Pointer<const char *>(0x00666304);
BOOL &g_Debug_MotionCapture = Make_Global<BOOL>(0x0065D7C4);
BOOL &g_Debug_Rotate = Make_Global<BOOL>(0x0065D7C8);
BOOL &g_Debug_Quiet = Make_Global<BOOL>(0x0065D7CC);
BOOL &g_Debug_Cheat = Make_Global<BOOL>(0x0065D7D0);
BOOL &g_Debug_Remap = Make_Global<BOOL>(0x0065D7D4);
BOOL &g_Debug_Icon = Make_Global<BOOL>(0x0065D7D8);
BOOL &g_Debug_Flag = Make_Global<BOOL>(0x0065D7DC);
BOOL &g_Debug_Lose = Make_Global<BOOL>(0x0065D7E0);
BOOL &g_Debug_Win = Make_Global<BOOL>(0x0065D7E4);
BOOL &g_Debug_Map = Make_Global<BOOL>(0x0065D7E8);
BOOL &g_Debug_Passable = Make_Global<BOOL>(0x0065D7EC);
BOOL &g_Debug_Unshroud = Make_Global<BOOL>(0x0065D7F0);
BOOL &g_Debug_Threat = Make_Global<BOOL>(0x0065D7F4);
BOOL &g_Debug_Find_Path = Make_Global<BOOL>(0x0065D7F8);
BOOL &g_Debug_Check_Map = Make_Global<BOOL>(0x0065D7FC);
BOOL &g_Debug_Playtest = Make_Global<BOOL>(0x0065D800);
BOOL &g_Debug_Heap_Dump = Make_Global<BOOL>(0x0065D804);
BOOL &g_Debug_Smart_Print = Make_Global<BOOL>(0x0065D808);
BOOL &g_Debug_Trap_Check_Heap = Make_Global<BOOL>(0x0065D80C);
BOOL &g_Debug_Modem_Dump = Make_Global<BOOL>(0x0065D810);
BOOL &g_Debug_Print_Events = Make_Global<BOOL>(0x0065D814);
BOOL &g_MonoEnabled = Make_Global<BOOL>(0x006AC288); // Actually a part of MonoClass.

// aircraft.cpp
#include "aircraft.h"
TFixedIHeapClass<AircraftClass> &g_Aircraft = Make_Global<TFixedIHeapClass<AircraftClass> >(0x0065D818);

// aircrafttype.cpp
TFixedIHeapClass<AircraftTypeClass> &g_AircraftTypes = Make_Global<TFixedIHeapClass<AircraftTypeClass> >(0x0065DDBC);
void *&AircraftTypeClass::g_LeftRotorData = Make_Global<void *>(0x00623010);
void *&AircraftTypeClass::g_RightRotorData = Make_Global<void *>(0x00623014);

// anim.cpp
#include "anim.h"
TFixedIHeapClass<AnimClass> &g_Anims = Make_Global<TFixedIHeapClass<AnimClass> >(0x0065D864);

// animtype.cpp
TFixedIHeapClass<AnimTypeClass> &g_AnimTypes = Make_Global<TFixedIHeapClass<AnimTypeClass> >(0x0065DEA0);

// building.cpp
#include "building.h"
TFixedIHeapClass<BuildingClass> &g_Buildings = Make_Global<TFixedIHeapClass<BuildingClass> >(0x0065D8B0);

// buildingtype.cpp
TFixedIHeapClass<BuildingTypeClass> &g_BuildingTypes = Make_Global<TFixedIHeapClass<BuildingTypeClass> >(0x0065DD70);
void *&BuildingTypeClass::g_WarFactoryOverlay = Make_Global<void *>(0x00635BA8);
void *&BuildingTypeClass::g_LightningShapes = Make_Global<void *>(0x00635BAC); // TODO: Should be moved to TechnoTypeClass.

// bullet.cpp
#include "bullet.h"
TFixedIHeapClass<BulletClass> &g_Bullets = Make_Global<TFixedIHeapClass<BulletClass> >(0x0065D8FC);

// bullettype.cpp
#include "bullettype.h"
TFixedIHeapClass<BulletTypeClass> &g_BulletTypes = Make_Global<TFixedIHeapClass<BulletTypeClass> >(0x0065DE54);

// crate.cpp
#include "crate.h"
char *CrateClass::s_CrateAnims = Make_Pointer<char>(0x005FF24C); // TODO should be AnimType *s_CrateAnims[CRATE_COUNT];
int *CrateClass::s_CrateShares = Make_Pointer<int>(0x005FF204);
int *CrateClass::s_CrateData = Make_Pointer<int>(0x00655DA8);

// display.cpp
#include "display.h"
LayerClass *DisplayClass::s_Layers = Make_Pointer<LayerClass>(0x00656080);
DisplayClass::TacticalClass &DisplayClass::s_TacticalButton = Make_Global<DisplayClass::TacticalClass>(0x00658804);
ARRAY_DEF(0x006560E0, uint8_t, DisplayClass::s_FadingBrighten, 256);
ARRAY_DEF(0x006561E0, uint8_t, DisplayClass::s_FadingShade, 256);
ARRAY_DEF(0x006562E0, uint8_t, DisplayClass::s_FadingWayDark, 256);
ARRAY_DEF(0x006563E0, uint8_t, DisplayClass::s_FadingLight, 256);
ARRAY_DEF(0x006564E0, uint8_t, DisplayClass::s_FadingGreen, 256);
ARRAY_DEF(0x006565E0, uint8_t, DisplayClass::s_FadingYellow, 256);
ARRAY_DEF(0x006566E0, uint8_t, DisplayClass::s_FadingRed, 256);
ARRAY2D_DEF(0x006567E0, uint8_t, DisplayClass::s_TranslucentTable, 14, 256);
ARRAY2D_DEF(0x006575E0, uint8_t, DisplayClass::s_WhiteTranslucentTable, 2, 256);
ARRAY2D_DEF(0x006577E0, uint8_t, DisplayClass::s_MouseTranslucentTable, 5, 256);
ARRAY2D_DEF(0x006582E8, uint8_t, DisplayClass::s_ShadowTrans, 5, 256);
ARRAY2D_DEF(0x00657CE4, uint8_t, DisplayClass::s_UnitShadow, 2, 256);
ARRAY2D_DEF(0x00657EE4, uint8_t, DisplayClass::s_UnitShadowAir, 2, 256);
ARRAY2D_DEF(0x006580E4, uint8_t, DisplayClass::s_SpecialGhost, 2, 256);
void *&DisplayClass::s_TransIconset = Make_Global<void *>(0x00657CE0);
void *&DisplayClass::s_ShadowShapes = Make_Global<void *>(0x006582E4);
BufferClass *&DisplayClass::s_TheaterBuffer = Make_Global<BufferClass *>(0x006680E4);
BooleanVectorClass &DisplayClass::s_CellRedraw = Make_Global<BooleanVectorClass>(0x006587E8);

// factory.cpp
#include "factory.h"
TFixedIHeapClass<FactoryClass> &g_Factories = Make_Global<TFixedIHeapClass<FactoryClass> >(0x0065D948);

// foot.cpp
cell_t &g_StartLocation = Make_Global<cell_t>(0x0065D7AE);
cell_t &g_DestLocation = Make_Global<cell_t>(0x0065D7AC);

// gameoptions.cpp
class GameOptionsClass;
GameOptionsClass &g_Options = Make_Global<GameOptionsClass>(0x00668188);

// gameptr.cpp
#include "template.h"
#include "terrain.h"
#include "terraintype.h"
#include "unittype.h"
#include "overlay.h"
#include "vesseltype.h"
#include "smudge.h"
template<>
FixedIHeapClass *const GamePtr<AircraftTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DDBC);
template<>
FixedIHeapClass *const GamePtr<AnimClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D864);
template<>
FixedIHeapClass *const GamePtr<AnimTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DEA0);
template<>
FixedIHeapClass *const GamePtr<BuildingClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D8B0);
template<>
FixedIHeapClass *const GamePtr<BuildingTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DD70);
template<>
FixedIHeapClass *const GamePtr<BulletTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DE54);
template<>
FixedIHeapClass *const GamePtr<FactoryClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D948);
template<>
FixedIHeapClass *const GamePtr<HouseClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D994);
template<>
FixedIHeapClass *const GamePtr<HouseTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DD24);
template<>
FixedIHeapClass *const GamePtr<InfantryClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D9E0);
template<>
FixedIHeapClass *const GamePtr<InfantryTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DE08);
template<>
FixedIHeapClass *const GamePtr<OverlayClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DA2C);
template<>
FixedIHeapClass *const GamePtr<OverlayTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065E01C);
template<>
FixedIHeapClass *const GamePtr<SmudgeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DA78);
template<>
FixedIHeapClass *const GamePtr<SmudgeTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065E068);
template<>
FixedIHeapClass *const GamePtr<TeamClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DAC4);
template<>
FixedIHeapClass *const GamePtr<TeamTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DB10);
template<>
FixedIHeapClass *const GamePtr<TemplateClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DB5C);
template<>
FixedIHeapClass *const GamePtr<TemplateTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DF84);
template<>
FixedIHeapClass *const GamePtr<TerrainClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DBA8);
template<>
FixedIHeapClass *const GamePtr<TerrainTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DFD0);
template<>
FixedIHeapClass *const GamePtr<TriggerClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DBF4);
template<>
FixedIHeapClass *const GamePtr<TriggerTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DCD8);
template<>
FixedIHeapClass *const GamePtr<UnitClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DC40);
template<>
FixedIHeapClass *const GamePtr<UnitTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DEEC);
template<>
FixedIHeapClass *const GamePtr<VesselClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DC8C);
template<>
FixedIHeapClass *const GamePtr<VesselTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DF38);
// template<>
// FixedIHeapClass *const GamePtr<WarheadTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x00691600);
// template<>
// FixedIHeapClass *const GamePtr<WeaponTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0069164C);

// getcd.cpp
class GetCDClass;
GetCDClass &g_CDList = Make_Global<GetCDClass>(0x00680884);

// gmouse.cpp
#include "gmouse.h"
void *&GameMouseClass::s_MouseShapes = Make_Global<void *>(0x00685160);
TCountDownTimerClass<SystemTimerClass> &GameMouseClass::s_AnimationTimer =
    Make_Global<TCountDownTimerClass<SystemTimerClass> >(0x00685164);

// ground.cpp
class GroundClass;
GroundClass *g_Ground = Make_Pointer<GroundClass>(0x00655DF0);

// gscreen.cpp
GadgetClass *&GameScreenClass::g_Buttons = Make_Global<GadgetClass *>(0x00680900);
GraphicViewPortClass *&GameScreenClass::g_ShadowPage = Make_Global<GraphicViewPortClass *>(0x00680904);

// help.cpp
ARRAY_DEF(0x006018B8, int16_t, HelpClass::s_OverlapList, HelpClass::HELP_OVERLAP_BUFFER);
char *&HelpClass::s_HelpText = Make_Global<char *>(0x006821B8);

// house.cpp
#include "house.h"
TFixedIHeapClass<HouseClass> &g_Houses = Make_Global<TFixedIHeapClass<HouseClass> >(0x0065D994);
HouseClass *&g_PlayerPtr = Make_Global<HouseClass *>(0x00669958);
TFixedIHeapClass<HouseClass::BuildChoiceClass> &HouseClass::g_BuildChoice =
    Make_Global<TFixedIHeapClass<HouseClass::BuildChoiceClass> >(0x006821BC);

// housetype.cpp
TFixedIHeapClass<HouseTypeClass> &g_HouseTypes = Make_Global<TFixedIHeapClass<HouseTypeClass> >(0x0065DD24);

// infantry.cpp
#include "infantry.h"
TFixedIHeapClass<InfantryClass> &g_Infantry = Make_Global<TFixedIHeapClass<InfantryClass> >(0x0065D9E0);

// infantrytype.cpp
TFixedIHeapClass<InfantryTypeClass> &g_InfantryTypes = Make_Global<TFixedIHeapClass<InfantryTypeClass> >(0x0065DE08);

// init.cpp
GameMixFile *&g_MainMix = Make_Global<GameMixFile *>(0x00668180);
GameMixFile *&g_ConquerMix = Make_Global<GameMixFile *>(0x00668184);
GameMixFile *&g_GeneralMix = Make_Global<GameMixFile *>(0x00668178);
GameMixFile *&g_MoviesMix = Make_Global<GameMixFile *>(0x00668174);
GameMixFile *&g_ScoreMix = Make_Global<GameMixFile *>(0x0066817C);

// iomap.cpp
class MapEditClass;
MapEditClass &g_Map = Make_Global<MapEditClass>(0x00668250);

// logic.cpp
#include "logic.h"
LogicClass &g_Logic = Make_Global<LogicClass>(0x00668230);
DynamicVectorClass<TriggerClass *> &g_LogicTriggers = Make_Global<DynamicVectorClass<TriggerClass *> >(0x0067F270);
DynamicVectorClass<TriggerClass *> &g_MapTriggers = Make_Global<DynamicVectorClass<TriggerClass *> >(0x0067F254);
DynamicVectorClass<TriggerClass *> *g_HouseTriggers = Make_Pointer<DynamicVectorClass<TriggerClass *> >(0x0067F074);

// missioncontrol.cpp
#include "missioncontrol.h"
MissionControlClass *const MissionControlClass::s_MissionControl = Make_Pointer<MissionControlClass>(0x00666234);

// object.cpp
DynamicVectorClass<ObjectClass *> &CurrentObjects = Make_Global<DynamicVectorClass<ObjectClass *> >(0x006677F8);

// objecttypes.cpp
void *&ObjectTypeClass::SelectShapes = Make_Global<void *>(0x006857A8);
void *&ObjectTypeClass::PipShapes = Make_Global<void *>(0x006857AC);

// overlay.cpp
TFixedIHeapClass<OverlayClass> &g_Overlays = Make_Global<TFixedIHeapClass<OverlayClass> >(0x0065DA2C);

// overlaytype.cpp
TFixedIHeapClass<OverlayTypeClass> &g_OverlayTypes = Make_Global<TFixedIHeapClass<OverlayTypeClass> >(0x0065E01C);

// power.cpp
PowerClass::PowerButtonClass &PowerClass::s_PowerButton = Make_Global<PowerClass::PowerButtonClass>(0x006877C0);
void *&PowerClass::s_PowerShape = Make_Global<void *>(0x006877B8);
void *&PowerClass::s_PowerBarShape = Make_Global<void *>(0x006877BC);

// queue.cpp
#include "gameevent.h"
#include "queue.h"
TEventQueueClass<GameEventClass, OUTGOING_SIZE> &g_OutgoingEvents =
    Make_Global<TEventQueueClass<GameEventClass, OUTGOING_SIZE> >(0x0066AB5C);
TEventQueueClass<GameEventClass, SCHEDULED_SIZE> &g_ScheduledEvents =
    Make_Global<TEventQueueClass<GameEventClass, SCHEDULED_SIZE> >(0x0066B068);

// radar.cpp
RadarClass::RTacticalClass &RadarClass::s_RadarButton = Make_Global<RadarClass::RTacticalClass>(0x006878E4);
void *&RadarClass::s_RadarAnim = Make_Global<void *>(0x00687908);
void *&RadarClass::s_RadarPulse = Make_Global<void *>(0x0068790C);
void *&RadarClass::s_RadarFrame = Make_Global<void *>(0x00687910);
BOOL &RadarClass::s_FullRedraw = Make_Global<BOOL>(0x00687914);
GraphicBufferClass &RadarClass::s_TileStage = Make_Global<GraphicBufferClass>(0x006879BC);

// rules.cpp
#include "rules.h"
fixed_t &RulesClass::m_EngineerDamage = Make_Global<fixed_t>(0x00665E02);
fixed_t &RulesClass::m_EngineerCaptureLevel = Make_Global<fixed_t>(0x00665E04);
fixed_t &RulesClass::m_ChronoTankDuration = Make_Global<fixed_t>(0x00665E00);
int &RulesClass::m_MTankDistance = Make_Global<int>(0x006016A4);
int &RulesClass::m_CarrierLaunchDelay = Make_Global<int>(0x006016A8);
fixed_t &RulesClass::m_QuakeUnitDamage = Make_Global<fixed_t>(0x00665DF8);
fixed_t RulesClass::m_QuakeVesselDamage = (fixed_t)0;
fixed_t &RulesClass::m_QuakeBuildingDamage = Make_Global<fixed_t>(0x00665DFA);
int &RulesClass::m_QuakeInfantryDamage = Make_Global<int>(0x006016AC);
int &RulesClass::m_QuakeDelay = Make_Global<int>(0x00665DFC);
BOOL &RulesClass::m_OrigNewUnitsEnabled = Make_Global<BOOL>(0x00665DE0);
BOOL &RulesClass::m_OrigSecretUnitsEnabled = Make_Global<BOOL>(0x00665DE4);
fixed_t RulesClass::m_CloakDelay;
RulesClass &g_Rule = Make_Global<RulesClass>(0x00666704);

// scenario.cpp
class ScenarioClass;
ScenarioClass &g_Scen = Make_Global<ScenarioClass>(0x006678E8);

// scroll.cpp
TCountDownTimerClass<SystemTimerClass> &ScrollClass::s_ScrollingCounter =
    Make_Global<TCountDownTimerClass<SystemTimerClass> >(0x00687C08);

// session.cpp
class SessionClass;
SessionClass &g_Session = Make_Global<SessionClass>(0x0067F2B4);

// sidebar.cpp
void *&SidebarClass::StripClass::s_LogoShapes = Make_Global<void *>(0x0068A464);
void *&SidebarClass::StripClass::s_ClockShapes = Make_Global<void *>(0x0068A468);
ARRAY_DEF(0x0068A46C, void *, SidebarClass::StripClass::s_SpecialShapes, SPECIAL_COUNT);
ARRAY2D_DEF(0x0068A2C4, SidebarClass::StripClass::SelectClass, SidebarClass::StripClass::s_SelectButton, COLUMN_COUNT,
    SidebarClass::StripClass::ROW_COUNT);
ARRAY_DEF(0x0068A1E4, ShapeButtonClass, SidebarClass::StripClass::s_UpButton, COLUMN_COUNT);
ARRAY_DEF(0x0068A254, ShapeButtonClass, SidebarClass::StripClass::s_DownButton, COLUMN_COUNT);
ARRAY2D_DEF(0x00689F18, char, SidebarClass::StripClass::s_ClockTranslucentTable, 2, 256);
SidebarClass::SBGadgetClass &SidebarClass::s_Background = Make_Global<SidebarClass::SBGadgetClass>(0x0068A118);
ShapeButtonClass &SidebarClass::s_RepairButton = Make_Global<ShapeButtonClass>(0x0068A13C);
ShapeButtonClass &SidebarClass::s_SellButton = Make_Global<ShapeButtonClass>(0x0068A174);
ShapeButtonClass &SidebarClass::s_ZoomButton = Make_Global<ShapeButtonClass>(0x0068A1AC);
void *&SidebarClass::s_SidebarShape = Make_Global<void *>(0x00689F0C);
void *&SidebarClass::s_SidebarMiddleShape = Make_Global<void *>(0x00689F10);
void *&SidebarClass::s_SidebarBottomShape = Make_Global<void *>(0x00689F14);

// smudge.cpp
#include "smudge.h"
TFixedIHeapClass<SmudgeClass> &g_Smudges = Make_Global<TFixedIHeapClass<SmudgeClass> >(0x0065DA78);

// smudgetype.cpp
#include "smudgetype.h"
TFixedIHeapClass<SmudgeTypeClass> &g_SmudgeTypes = Make_Global<TFixedIHeapClass<SmudgeTypeClass> >(0x0065E068);

// special.cpp
SpecialClass &s_Special = Make_Global<SpecialClass>(0x00669908);

// tab.cpp
void *&TabClass::s_TabShape = Make_Global<void *>(0x0068A4C0);

// team.cpp
#include "team.h"
TFixedIHeapClass<TeamClass> &g_Teams = Make_Global<TFixedIHeapClass<TeamClass> >(0x0065DAC4);

// teamtype.cpp
TFixedIHeapClass<TeamTypeClass> &g_TeamTypes = Make_Global<TFixedIHeapClass<TeamTypeClass> >(0x0065DB10);

// technotype.cpp
void *&TechnoTypeClass::s_WakeShapes = Make_Global<void *>(0x0068D2DC);
void *&TechnoTypeClass::s_TurretShapes = Make_Global<void *>(0x0068D2E0);
void *&TechnoTypeClass::s_SamShapes = Make_Global<void *>(0x0068D2E4);
void *&TechnoTypeClass::s_MGunShapes = Make_Global<void *>(0x0068D2E8);

// templatetype.cpp
TFixedIHeapClass<TemplateTypeClass> &g_TemplateTypes = Make_Global<TFixedIHeapClass<TemplateTypeClass> >(0x0065DF84);

// template.cpp
TFixedIHeapClass<TemplateClass> &g_Templates = Make_Global<TFixedIHeapClass<TemplateClass> >(0x0065DB5C);

// terrain.cpp
TFixedIHeapClass<TerrainClass> &g_Terrains = Make_Global<TFixedIHeapClass<TerrainClass> >(0x0065DBA8);

// terraintype.cpp
TFixedIHeapClass<TerrainTypeClass> &g_TerrainTypes = Make_Global<TFixedIHeapClass<TerrainTypeClass> >(0x0065DFD0);

// theater.cpp
TheaterType &g_LastTheater = Make_Global<TheaterType>(0x006017CC);

// theme.cpp
ThemeClass &g_Theme = Make_Global<ThemeClass>(0x00668248);
ThemeClass::ThemeControl *ThemeClass::s_Themes = Make_Pointer<ThemeClass::ThemeControl>(0x006052FC);

// trigger.cpp
TFixedIHeapClass<TriggerClass> &g_Triggers = Make_Global<TFixedIHeapClass<TriggerClass> >(0x0065DBF4);

// triggertypes.cpp
TFixedIHeapClass<TriggerTypeClass> &g_TriggerTypes = *reinterpret_cast<TFixedIHeapClass<TriggerTypeClass> *>(0x0065DCD8);

// unit.cpp
#include "unit.h"
TFixedIHeapClass<UnitClass> &g_Units = Make_Global<TFixedIHeapClass<UnitClass> >(0x0065DC40);

// unittype.cpp
TFixedIHeapClass<UnitTypeClass> &g_UnitTypes = Make_Global<TFixedIHeapClass<UnitTypeClass> >(0x0065DEEC);

// version.cpp
class VersionClass;
VersionClass &g_Version = Make_Global<VersionClass>(0x00667810);

// vessel.cpp
#include "vessel.h"
TFixedIHeapClass<VesselClass> &g_Vessels = Make_Global<TFixedIHeapClass<VesselClass> >(0x0065DC8C);

// vesseltype.cpp
TFixedIHeapClass<VesselTypeClass> &g_VesselTypes = Make_Global<TFixedIHeapClass<VesselTypeClass> >(0x0065DF38);

// vortex.cpp
class ChronalVortexClass;
ChronalVortexClass &g_ChronalVortex = Make_Global<ChronalVortexClass>(0x006904B4);

// vox.cpp
#include "vox.h"
void **g_SpeechBuffer = Make_Pointer<void *>(0x006680D8);
VoxType *g_SpeechRecord = Make_Pointer<VoxType>(0x006680E0);

// warheadtype.cpp
TFixedIHeapClass<WarheadTypeClass> &g_WarheadTypes = Make_Global<TFixedIHeapClass<WarheadTypeClass> >(0x00691600);

// weapontype.cpp
TFixedIHeapClass<WeaponTypeClass> &g_WeaponTypes = Make_Global<TFixedIHeapClass<WeaponTypeClass> >(0x0069164C);

// language.cpp
char *&g_GameStrings = Make_Global<char *>(0x0066991C);
char *&g_DebugStrings = Make_Global<char *>(0x00669920);
char **g_NameOverride = Make_Pointer<char *>(0x0066616C);
int *g_NameIDOverride = Make_Pointer<int>(0x006661D0);

// random.cpp
RandomClass &g_NonCriticalRandom = Make_Global<RandomClass>(0x00667760);

// remap.cpp
RemapControlType *g_ColorRemaps = Make_Pointer<RemapControlType>(0x00666908);
RemapControlType &g_MetalScheme = Make_Global<RemapControlType>(0x00667528);
RemapControlType &g_GreyScheme = Make_Global<RemapControlType>(0x00667644);
RemapControlType &g_SidebarScheme = Make_Global<RemapControlType>(0x00684CC0);

// timer.cpp
TimerClass &g_TickCount = Make_Global<TimerClass>(0x006ABF6C);
CountDownTimerClass &g_CountDown = Make_Global<CountDownTimerClass>(0x006ABF78);

// ttimer.cpp
TTimerClass<SystemTimerClass> &g_TickCountTimer = Make_Global<TTimerClass<SystemTimerClass> >(0x00680870);
TCountDownTimerClass<SystemTimerClass> &g_FrameTimer = Make_Global<TCountDownTimerClass<SystemTimerClass> >(0x006807F8);

// pk.cpp
PKey &g_PublicKey = Make_Global<PKey>(0x00665F68);

// wsock.cpp
class WinsockInterfaceClass;
WinsockInterfaceClass *&g_PacketTransport = Make_Global<WinsockInterfaceClass *>(0x0069172C);

// basec.cpp
class BaseClass;
BaseClass &g_Base = Make_Global<BaseClass>(0x0067F28C);

// carryover.cpp
class CarryoverClass;
CarryoverClass *&g_Carryover = Make_Global<CarryoverClass*>(0x0067F2AC);

//ego.cpp
class EgoClass;
DynamicVectorClass<EgoClass *> &g_EgoList = Make_Global<DynamicVectorClass<EgoClass *> >(0x00658830);

// audio.cpp
#define DIRECTSOUND_VERSION 0x0600
// These includes must not be resorted.
// clang-format off
#include <mmsystem.h>
#include <dsound.h>
// clang-format on
struct LockedDataType;
BOOL &g_ReverseChannels = Make_Global<BOOL>(0x006ABFDC);
LockedDataType &g_LockedData = Make_Global<LockedDataType>(0x006EC4D8);
#ifdef BUILD_WITH_DSOUND
LPDIRECTSOUND &g_SoundObject = Make_Global<LPDIRECTSOUND>(0x006ABFE0);
LPDIRECTSOUNDBUFFER &g_DumpBuffer = Make_Global<LPDIRECTSOUNDBUFFER>(0x006ABFBC);
LPDIRECTSOUNDBUFFER &g_PrimaryBufferPtr = Make_Global<LPDIRECTSOUNDBUFFER>(0x006ABFE4);
WAVEFORMATEX &g_DsBuffFormat = Make_Global<WAVEFORMATEX>(0x006ABFEC);
WAVEFORMATEX &g_PrimaryBuffFormat = Make_Global<WAVEFORMATEX>(0x006AC014);
DSBUFFERDESC &g_BufferDesc = Make_Global<DSBUFFERDESC>(0x006AC000);
DSBUFFERDESC &g_PrimaryBufferDesc = Make_Global<DSBUFFERDESC>(0x006AC028);
CRITICAL_SECTION &g_GlobalAudioCriticalSection = Make_Global<CRITICAL_SECTION>(0x006AC03C);
#endif
void *&g_SoundThreadHandle = Make_Global<void *>(0x006ABFC0);
BOOL &g_SoundThreadActive = Make_Global<BOOL>(0x006ABFC4);
BOOL &g_StartingFileStream = Make_Global<BOOL>(0x006ABFC8);
MemoryFlagType &g_StreamBufferFlag = Make_Global<MemoryFlagType>(0x006ABFD0);
int &g_Misc = Make_Global<int>(0x006ABFD4);
#ifdef BUILD_WITH_DSOUND
UINT &g_SoundTimerHandle = Make_Global<UINT>(0x006ABFE8);
#endif
void *&g_FileStreamBuffer = Make_Global<void *>(0x006AC054);
BOOL volatile &g_AudioDone = Make_Global<BOOL>(0x006AC05C);
int16_t &g_SoundType = Make_Global<int16_t>(0x006ABFD8);
int16_t &g_SampleType = Make_Global<int16_t>(0x006ABFDA);
