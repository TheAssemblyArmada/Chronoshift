////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: HOOKER.H
//
//        Author:: OmniBlade
//
//  Contributors:: CCHyper, Saberhawk, jonwil, LRFLEW
//
//   Description:: Originally based on work by the Tiberian Technologies team
//                 to hook Renegade, rewritten based on work by LRFLEW for
//                 OpenMC2. Provides methods for accessing data and functions in
//                 an existing binary and replacing functions with new
//                 implementations from an injected DLL.
//
//       License:: Redalert++ is free software: you can redistribute it and/or
//                 modify it under the terms of the GNU General Public License
//                 as published by the Free Software Foundation, either version
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef HOOK_SUPPORT_H
#define HOOK_SUPPORT_H

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "always.h"

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

#if 0
// Call_Function and Call_Method can be used to call functions from the original
// binary where they are required and a replacement hasn't been written yet.
template<typename T, typename... Types>
__forceinline T Call_Function(const uintptr_t address, Types... args) {
    return reinterpret_cast<T(__cdecl *)(Types...)>(address)(args...);
}

template<typename T, typename... Types>
__forceinline T Call__StdCall_Function(const uintptr_t address, Types... args) {
    return reinterpret_cast<T(__stdcall *)(Types...)>(address)(args...);
}

template<typename T, typename X, typename... Types>
__forceinline T Call_Method(const uintptr_t address, X *const self, Types... args) {
    return reinterpret_cast<T(__thiscall *)(X *, Types...)>(address)(self, args...);
}

// These create pointers to various types of function where the return,
// parameters and calling convention are known.
template<typename T, typename... Types>
__forceinline T(__cdecl *Make_Function_Ptr(const uintptr_t address))(Types...) {
    return reinterpret_cast<T(__cdecl *)(Types...)>(address);
}

template<typename T, typename... Types>
__forceinline T(__stdcall *Make_StdCall_Ptr(const uintptr_t address))(Types...)
{
    return reinterpret_cast<T(__stdcall *)(Types...)>(address);
}

template<typename T, typename C, typename... Types>
__forceinline T(__thiscall *Make_Method_Ptr(const uintptr_t address))(C *, Types...)
{
    return reinterpret_cast<T(__thiscall *)(C *, Types...)>(address);
}

template<typename T, typename... Types>
__forceinline T(__cdecl *Make_VA_Function_Ptr(const uintptr_t address))(Types..., ...) {
    return reinterpret_cast<T(__cdecl *)(Types..., ...)>(address);
}
#endif

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
    cmd.addr = reinterpret_cast<uintptr_t>((void *&)out) - in - 5;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)in, &cmd, 5, nullptr);
}

__declspec(dllexport) void StartHooking();
__declspec(dllexport) void StopHooking();

#define REF_DECL(type, name) type &name
#define REF_ARR_DECL(type, name, size) RefArrayHelper<type, size> &name
#define REF_DEF(type, name, client) type &name = client;
#define REF_ARR_DEF(type, name, size, client) RefArrayHelper<type, size> &name = client;

#endif // _HOOK_SUPPORT_H
