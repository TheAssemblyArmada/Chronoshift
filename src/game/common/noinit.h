/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Dummy class to act as parameter to select constructors that do no initialisation.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef NOINIT_H
#define NOINIT_H

#include "always.h"

class NoInitClass
{
public:
    void operator()() const {};
};

class InitClass
{
public:
    void operator()() const {};
};

#endif // NOINIT_H
