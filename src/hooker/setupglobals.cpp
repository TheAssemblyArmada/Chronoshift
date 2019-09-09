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
