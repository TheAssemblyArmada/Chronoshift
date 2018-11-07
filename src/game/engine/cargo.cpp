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
#include "cargo.h"
#include "gamedebug.h"
//#include "foot.h"

void CargoClass::Attach(FootClass *object)
{
    // TODO Needs FootClass
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(CargoClass *, FootClass *) = reinterpret_cast<void (*)(CargoClass *, FootClass *)>(0x004623D0);
    func(this, object);
#else
    /*if (object != nullptr) {
        object->Limbo();

        // finding the last object in the list?
        ObjectClass *next_obj = nullptr;
        for (next_obj = object->Next; next_obj->Next; next_obj = next_obj->Next);

        if (next_obj != nullptr) { // temp
            next_obj->Next = Object;
        } else {
            object->Next = Object;
        }

        Object = object;

        // empty loop for checking how many linked objects?
        // It must use something like next object, otherwise if foot->Next was
        // valid it would keep looping forever
        if (Object != nullptr) {
            // for ( Count = 0; object->Next; ++Count );
            for (Count = 0, next_obj = object->Next; next_obj->Next; ++Count, next_obj = next_obj->Next)
                ;
        }
    }*/
#endif
}

FootClass *CargoClass::Attached_Object() const
{
    if (Object != nullptr) {
        return Object;
    }

    return nullptr;
}

FootClass *CargoClass::Detach_Object()
{
#ifndef CHRONOSHIFT_STANDALONE
    FootClass *(*func)(CargoClass *) = reinterpret_cast<FootClass *(*)(CargoClass *)>(0x004623D0);
    return func(this);
#else
    /*FootClass *object = Attached_Object();
    if (object != nullptr) {
        Object = object->Get_Next();
        object->Next = nullptr;
        --Count;

        return object;
    }*/

    return nullptr;
#endif
}

void CargoClass::Code_Pointers()
{
    // TODO requires FootClass
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(CargoClass *) = reinterpret_cast<void (*)(CargoClass *)>(0x004F9884);
    func(this);
#endif
}

void CargoClass::Decode_Pointers()
{
    // TODO requires FootClass
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(CargoClass *) = reinterpret_cast<void (*)(CargoClass *)>(0x004F98C0);
    func(this);
#endif
}
