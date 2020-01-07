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
#pragma once

#ifndef LAYER_H
#define LAYER_H

#include "always.h"
#include "gametypes.h"
#include "vector.h"

class Pipe;
class Straw;
class ObjectClass;

enum LayerType
{
    LAYER_SURFACE,
    LAYER_GROUND,
    LAYER_AIR,
    LAYER_TOP,

    LAYER_COUNT,

    LAYER_NONE = -1,
    LAYER_FIRST = LAYER_SURFACE,
};

DEFINE_ENUMERATION_OPERATORS(LayerType);

class LayerClass : public DynamicVectorClass<ObjectClass *>
{
public:
    LayerClass();
    virtual ~LayerClass();

    virtual void Init();
    virtual void One_Time() {}

    virtual void Code_Pointers();
    virtual void Decode_Pointers();

    void Remove(ObjectClass *object) { Delete(object); }
    BOOL Submit(ObjectClass *object, BOOL sort = false);

    void Sort();
    BOOL Sorted_Add(ObjectClass *const object);

    void Mark_All(MarkType mark);
    void Render_All(BOOL force_redraw = false);

    BOOL Load(Straw &straw);
    BOOL Save(Pipe &pipe) const;

#ifdef GAME_DLL
public:
    BOOL Hook_Save(Pipe &pipe)
    {
        return LayerClass::Save(pipe);
    }
#endif
};

#endif // LAYER_H
