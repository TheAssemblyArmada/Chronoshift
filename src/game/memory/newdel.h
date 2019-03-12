/**
 * @file
 *
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Custom memory allocators for new and delete.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef NEWDEL_H
#define NEWDEL_H

#include "always.h"

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr) noexcept;

#endif