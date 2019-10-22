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
#include "basec.h"
#include "gameini.h"
#include "house.h"
#include "iomap.h"
#include "pipe.h"
#include "straw.h"
#include <stdio.h>

#ifndef GAME_DLL
BaseClass g_Base;
#endif

/**
 * @brief
 *
 * @address 0x004265F8
 */
const BuildingClass *BaseClass::Get_Building(int entry) const
{
    BuildingClass *ptrs[7];
    const BaseNodeClass &node = m_Nodes[entry];
    CellClass &cell = Map[node.m_Cell];

    ptrs[0] = cell.Cell_Building();
    int count = 1;
    for (int i = 0; i < OVERLAPPER_COUNT; ++i) {
        if (cell.Get_Overlapper(i) != nullptr) {
            ptrs[count++] = (BuildingClass *)cell.Get_Overlapper(i);
        }
    }

    for (int j = 0; j < count; ++j) {
        if (ptrs[j] != nullptr && ptrs[j]->Get_Cell() == node.m_Cell) {
            if (ptrs[j]->What_Am_I() == RTTI_BUILDING && ptrs[j]->What_Type() == node.m_Type) {
                return ptrs[j];
            }
        }
    }
    return nullptr;
}

/**
 * @brief
 *
 * @address 0x0042674C
 */
BaseNodeClass *BaseClass::Get_Node(BuildingClass *building)
{
    for (int i = 0; i < m_Nodes.Count(); ++i) {
        if (m_Nodes[i].m_Type == building->What_Type() && building->Get_Cell() == m_Nodes[i].m_Cell) {
            return &m_Nodes[i];
        }
    }
    return nullptr;
}

/**
 * @brief
 *
 * @address 0x004267C4
 */
BaseNodeClass *BaseClass::Get_Node(cell_t cellnum)
{
    for (int i = 0; i < m_Nodes.Count(); ++i) {
        if (m_Nodes[i].m_Cell == cellnum) {
            return &m_Nodes[i];
        }
    }
    return nullptr;
}

/**
 * @brief
 *
 * @address 0x004267F8
 */
BaseNodeClass *BaseClass::Next_Buildable(BuildingType type)
{
    for (int i = 0; i < m_Nodes.Count(); ++i) {
        if (type == BUILDING_NONE) {
            if (!Is_Built(i)) {
                return &m_Nodes[i];
            }
        } else {
            if (type == m_Nodes[i].m_Type && !Is_Built(i)) {
                return &m_Nodes[i];
            }
        }
    }
    return nullptr;
}

/**
 * @brief
 *
 * @address 0x00426470
 */
BOOL BaseClass::Load(Straw &straw)
{
    int32_t size = 0;
    int32_t count = 0;
    BaseNodeClass node;

    if (straw.Get(&size, sizeof(size)) != sizeof(size)) {
        return false;
    }
    if (size != sizeof(BaseClass)) {
        return false;
    }
    if (straw.Get(&m_Player, sizeof(m_Player)) != sizeof(m_Player)) {
        return false;
    }
    if (straw.Get(&count, sizeof(count)) != sizeof(count)) {
        return false;
    }
    for (int i = 0; i < count; ++i) {
        if (straw.Get(&node, sizeof(BaseNodeClass)) != sizeof(BaseNodeClass)) {
            return false;
        }
        m_Nodes.Add(node);
    }
    return true;
}

/**
 * @brief
 *
 * @address 0x00426540
 */
BOOL BaseClass::Save(Pipe &pipe)
{
    int32_t size = sizeof(BaseClass);
    pipe.Put(&size, sizeof(size));
    pipe.Put(&m_Player, sizeof(m_Player));
    int32_t count = m_Nodes.Count();
    pipe.Put(&count, sizeof(count));
    for (int i = 0; i < count; ++i) {
        pipe.Put(&m_Nodes[i], sizeof(BaseNodeClass));
    }
    return true;
}

/**
 * @brief
 *
 * @address 0x00426858
 */
void BaseClass::Read_INI(GameINIClass &ini)
{
    BaseNodeClass node;

    m_Player = ini.Get_HousesType("Base", "Player", g_PlayerPtr->What_Type());
    int count = ini.Get_Int("Base", "Count", 0);

    if (count > 0) {
        for (int i = 0; i < count; ++i) {
            char entry[12];
            char value[128];

            snprintf(entry, sizeof(entry), "%03d", i);
            ini.Get_String("Base", entry, nullptr, value, sizeof(value));
            node.m_Type = BuildingTypeClass::From_Name(strtok(value, ","));
            node.m_Cell = atoi(strtok(nullptr, ","));
            m_Nodes.Add(node);
        }
    }
}

/**
 * @brief
 *
 * @address 0x00426944
 */
void BaseClass::Write_INI(GameINIClass &ini)
{
    ini.Clear("Base");

    if (m_Player != HOUSES_NONE) {
        ini.Put_HousesType("Base", "Player", m_Player);
        ini.Put_Int("Base", "Count", m_Nodes.Count());

        for (int i = 0; i < m_Nodes.Count(); ++i) {
            char entry[12];
            char value[128];

            snprintf(entry, sizeof(entry), "%03d", i);
            snprintf(value, sizeof(value), "%s,%d", BuildingTypeClass::Name_From(m_Nodes[i].m_Type), m_Nodes[i].m_Cell);
            ini.Put_String("Base", entry, value);
        }
    }
}
