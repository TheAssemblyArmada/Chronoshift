/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Handles tracking units currently stored as cargo.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CARGO_H
#define CARGO_H

#include "always.h"

class NoInitClass;
class FootClass;

class CargoClass
{
public:
    CargoClass() : Count(0), Object(nullptr) {}
    CargoClass(const NoInitClass &noinit) {}
    ~CargoClass() { Object = nullptr; }

    void AI() {}
    void Attach(FootClass *object);
    FootClass *Attached_Object() const;
    FootClass *Detach_Object();
    void Code_Pointers();
    void Decode_Pointers();
    int Cargo_Count() const { return Count; }
    BOOL Has_Cargo() const { return Attached_Object() != nullptr; }

private:
    uint8_t Count;
    FootClass *Object;
};

#endif // CARGO_H
