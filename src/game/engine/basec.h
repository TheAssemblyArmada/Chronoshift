/**
 * @file
 *
 * @author tomsons26
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#pragma once

#ifndef BASEC_H
#define BASEC_H

#include "always.h"
#include "building.h"
#include "vector.h"

class Straw;
class GameINIClass;
class Pipe;

class BaseNodeClass
{
public:
    BaseNodeClass() {}
    ~BaseNodeClass() {}

    BOOL operator==(const BaseNodeClass &that) { return m_Type == that.m_Type && m_Cell == that.m_Cell; }
    BOOL operator!=(const BaseNodeClass &that) { return !(this == &that); }
    BOOL operator>(const BaseNodeClass &that) { return true; }

public:
    BuildingType m_Type;
    cell_t m_Cell;
};

class BaseClass
{
public:
    BaseClass() : m_Nodes() {}
    ~BaseClass() {}

    BaseNodeClass &operator[](int entry) { return m_Nodes[entry]; }
    const BaseNodeClass &operator[](int entry) const { return m_Nodes[entry]; }

    BOOL Is_Built(int entry) const { return Get_Building(entry) != nullptr; }
    const BuildingClass *Get_Building(int entry) const;

    BOOL Is_Node(BuildingClass *building) { return Get_Node(building) != nullptr; }
    BaseNodeClass *Get_Node(BuildingClass *building);
    BaseNodeClass *Get_Node(cell_t cellnum);

    BaseNodeClass *Next_Buildable(BuildingType type);

    void Clear() { m_Player = HOUSES_NONE; m_Nodes.Clear(); }
    int Node_Count() const { return m_Nodes.Count(); }

    void Read_INI(GameINIClass &ini);
    void Write_INI(GameINIClass &ini);

    BOOL Load(Straw &straw);
    BOOL Save(Pipe &pipe);

    void Code_Pointers() {}
    void Decode_Pointers() {}

private:
    DynamicVectorClass<BaseNodeClass> m_Nodes;
    HousesType m_Player;
};

#ifdef GAME_DLL
extern BaseClass &g_Base;
#else
extern BaseClass g_Base;
#endif

#endif // BASEC_H
