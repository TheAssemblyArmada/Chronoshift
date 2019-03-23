/**
 * @file
 *
 * @author CCHyper
 * @author jonwil
 * @author LRFLEW
 * @author OmniBlade
 * @author SaberHawk
 *
 * @brief Code for injecting code into the original and calling code from the original.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 *
 * Originally based on work by the Tiberian Technologies team to hook Renegade, rewritten based on work by LRFLEW for
 * OpenMC2. Provides methods for accessing data and functions in an existing binary and replacing functions with new
 * implementations from an injected DLL.
 */
#pragma once

#ifndef HOOK_SUPPORT_H
#define HOOK_SUPPORT_H

#include "always.h"

#ifdef __WATCOMC__
#include <windows.h>
#else
#include <processthreadsapi.h>
#include <memoryapi.h>
#endif

template<typename T, const int size>
class RefArrayHelper
{
protected:
    char _dummy[size * sizeof(T)];

public:
    operator T *() { return (T *)this; };

    operator const T *() const { return (T *)this; };

    T *operator&() { return (T *)this; };

    const T *operator&() const { return (T *)this; };
};

// Helper Functions based on those from OpenMC2

// Use Make_Global to access global variables in the original exe once you know
// the correct type and address. This should not change from run to run if the
// exe loads at a standard base address.

// Typical use will be to use define to create a friendly name, e.g:
// #define SomeGlobalVar (Make_Global<bool>(0x00FF00FF))
// allows you to use SomeGlobalVar as though it was a bool you declared, though
// it will reflect the value the original exe sees at address 0x00FF00FF
template<typename T>
inline T &Make_Global(const uintptr_t address)
{
    return *reinterpret_cast<T *>(address);
}

template<typename T>
inline T *Make_Pointer(const uintptr_t address)
{
    return reinterpret_cast<T *>(address);
}

//Macro version of Call_Function function as Watcom doesn't support variadic template

//If the called function returns you don't need to add return after CALL_FUNCTION as it will be handled automatically by the macro.
//You can't have a "return CALL_FUNCTION" with this macro as that just then returns the function address, for using return value use DEFINE_CALL

// unsigned int adr = address - needed because of a watcom bug making address a const which makes it create call dword ptr [eax]
#define CALL_FUNCTION(address, pointer_type, ...)\
    unsigned int adr = address; \
    return reinterpret_cast<pointer_type>(adr)(##__VA_ARGS__)

//DEFINE_CALL wraps our inital reinterpret_cast method of calling a function in a simpler package

//to use it define DEFINE_CALL in function and call it, like so:
//  DEFINE_CALL(MyFunction, 0xDEADBEEF, void*, (int, int));
//  return MyFunction(integral, integral2);

#define DEFINE_CALL(func_name, address, return_type, ...) \
    return_type (*func_name)(##__VA_ARGS__) = reinterpret_cast<return_type (*)(##__VA_ARGS__)>(address);

// A nice struct to pack the assembly in for jumping into replacement code.
// So long as the calling conventions and arguments for the replaced and
// replacement functions are the same, everything should just work.
#pragma pack(push, 1)
struct x86_jump
{
    uint8_t cmd;
    uintptr_t addr;
};
#pragma pack(pop)

/**
 * @brief Replaces a function in the original binary with a new one at run time.
 *
 * Replaces a function with a different function at run time by patching in an assembly "jump" instruction to jump to the new
 * function. Provided the new function has the same parameters and calling convention as the one that it replaced, it should
 * work without issue so long as it can fulfill the same role.
 *
 * @param in The address of the function to replace.
 * @param out The address of the function to replace with.
 */
template<typename T>
void Hook_Function(uintptr_t in, T out)
{
    static_assert(sizeof(x86_jump) == 5, "Jump struct not expected size.");

    x86_jump cmd;
    cmd.cmd = 0xE9;
    cmd.addr = reinterpret_cast<uintptr_t>(out) - in - 5;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)in, &cmd, 5, nullptr);
}

/**
* @brief Replaces a function call in the original binary with a new one at run time.
*
* Replaces a function with a different function at run time by patching in an assembly "jump" instruction to jump to the new
* function. Provided the new function has the same parameters and calling convention as the one that it replaced, it should
* work without issue so long as it can fulfill the same role.
*
* @param in The address of the function call to replace.
* @param out The address of the function to replace with.
*/
template<typename T>
void Hook_Call(uintptr_t in, T out)
{
    static_assert(sizeof(x86_jump) == 5, "Jump struct not expected size.");

    x86_jump cmd;
    cmd.cmd = 0xE8;
    cmd.addr = reinterpret_cast<uintptr_t>(out) - in - 5;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)in, &cmd, 5, nullptr);
}

/**
 * @brief Replaces a member function in the original binary with a new one at run time.
 *
 * Replaces a function with a different member function at run time by patching in an assembly "jump" instruction to jump to
 * the new function. Provided the new function has the same parameters and calling convention as the one that it replaced, it
 * should work without issue so long as it can fulfill the same role.
 *
 * @param in The address of the function to replace.
 * @param out The address of the function to replace with.
 */
template<typename T>
void Hook_Method(uintptr_t in, T out)
{
    static_assert(sizeof(x86_jump) == 5, "Jump struct not expected size.");

    x86_jump cmd;
#ifdef COMPILER_WATCOM
    x86_jump *tmp = reinterpret_cast<x86_jump*>((void *&)out);
    cmd.addr = tmp.addr - in - 5;
#else
    cmd.addr = reinterpret_cast<uintptr_t>((void *&)out) - in - 5;
#endif
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)in, &cmd, 5, nullptr);
}

__declspec(dllexport) void StartHooking();
__declspec(dllexport) void StopHooking();

#define REF_DECL(type, name) type &name
#define REF_ARR_DECL(type, name, size) RefArrayHelper<type, size> &name
#define REF_DEF(type, name, client) type &name = client;
#define REF_ARR_DEF(type, name, size, client) RefArrayHelper<type, size> &name = client;

#endif // _HOOK_SUPPORT_H
