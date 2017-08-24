#include "startup.h"
#include "gamedebug.h"

void Emergency_Exit(int exit_code)
{
    DEBUG_LOG("Calling emergency exit.\n");
    // TODO Requires GraphicBufferClass, PaletteClass, KeyboardClass
    void(*hooked_exit)(int) = reinterpret_cast<void(*)(int)>(0x00552714);
    hooked_exit(exit_code);
}