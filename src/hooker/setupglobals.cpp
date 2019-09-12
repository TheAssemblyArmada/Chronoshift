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
