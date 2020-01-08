/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for handling layer placement.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "layer.h"
#include "gamedebug.h"
#include "object.h"
#include "target.h"

LayerClass::LayerClass()
{
}

LayerClass::~LayerClass()
{
}

/**
 *
 *
 */
void LayerClass::Init()
{
    DEBUG_LOG("LayerClass::Init()\n");

    Clear();
}

/**
 * @brief
 *
 * @address 0x004F9670
 */
void LayerClass::Code_Pointers()
{
    for (int i = 0; i < Count(); ++i) {
        ObjectClass *obj = (*this)[i];
        DEBUG_ASSERT(obj != nullptr);

        (*this)[i] = reinterpret_cast<ObjectClass *>(As_Target(obj));
    }
}

/**
 * @brief
 *
 * @address 0x004F96CC
 */
void LayerClass::Decode_Pointers()
{
    for (int i = 0; i < Count(); ++i) {
        (*this)[i] = As_Object((uintptr_t)((*this)[i]));
        DEBUG_ASSERT((*this)[i] != nullptr);
    }
}

/**
 *
 *
 */
BOOL LayerClass::Submit(ObjectClass *object, BOOL sort)
{
    DEBUG_ASSERT(object != nullptr);

    if (sort) {
        return Sorted_Add(object);
    }

    return Add(object);
}

/**
 *
 *
 */
void LayerClass::Sort()
{
    // Go through array to Count() - 1
    for (int index = 0; index < (ActiveCount - 1); ++index) {
        // Get current index and next for comparison
        ObjectClass *current = (*this)[index];
        ObjectClass *next = (*this)[index + 1];

        // Compare, if current is less than next, swap them.
        if (current != nullptr && next != nullptr) {
            if (next->Sort_Y() < current->Sort_Y()) {
                (*this)[index + 1] = current;
                (*this)[index] = next;
            }
        }
    }
}

/**
 *
 *
 */
BOOL LayerClass::Sorted_Add(ObjectClass *const object)
{
    DEBUG_ASSERT(object != nullptr);

    if (ActiveCount >= VectorMax) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            if (!Resize(VectorMax + GrowthStep)) {
                // Failure to increase the size of the vector is an error
                return false;
            }

        } else {
            // Increasing the size of this vector is not allowed! Bail this
            // outine with the error flag.
            return false;
        }
    }

    int index = 0;

    for (index = 0; index < ActiveCount; ++index) {
        ObjectClass *objptr = (*this)[index];
        // DEBUG_ASSERT(objptr != nullptr);
        if (objptr != nullptr) {
            if (object->Sort_Y() < objptr->Sort_Y()) {
                break;
            }
        }
    }

    for (int j = (ActiveCount - 1); j >= index; --j) {
        (*this)[j + 1] = (*this)[j];
    }

    (*this)[index] = object;

    // Increase the active count as we have added a new object.
    ++ActiveCount;

    return true;
}

/**
 *
 *
 */
void LayerClass::Mark_All(MarkType mark)
{
    for (int index = 0; index < Count(); ++index) {
        ObjectClass *objptr = (*this)[index];
        DEBUG_ASSERT(objptr != nullptr);

        if (objptr != nullptr && objptr->Is_Active()) {
            objptr->Mark(mark);
        }
    }
}

/**
 *
 *
 */
void LayerClass::Render_All(BOOL force_redraw)
{
    for (int index = 0; index < Count(); ++index) {
        ObjectClass *objptr = (*this)[index];
        // DEBUG_ASSERT(objptr != nullptr);
        if (objptr != nullptr && objptr->Is_Active()) {
            (*this)[index]->Render(force_redraw);
        }
    }
}

/**
 *
 *
 */
BOOL LayerClass::Load(Straw &straw)
{
    uint32_t count = 0;
    ObjectClass *objptr = nullptr;

    if (straw.Get(&count, sizeof(count)) != sizeof(count)) {
        return false;
    }

    Clear();

    for (int i = 0; i < count; ++i) {
        if (straw.Get(&objptr, sizeof(objptr)) != sizeof(objptr)) {
            return false;
        }
        Add(objptr);
    }

    return true;
}

/**
 *
 *
 */
BOOL LayerClass::Save(Pipe &pipe) const
{
    uint32_t count = Count();

    if (pipe.Put(&count, sizeof(count)) != sizeof(count)) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        ObjectClass *objptr = (*this)[i];

        if (pipe.Put(&objptr, sizeof(objptr)) != sizeof(objptr)) {
            return false;
        }
    }

    return true;
}
