/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Chronal vortex class for handling the special vortex effect.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef VORTEX_H
#define VORTEX_H

#include "always.h"
#include "display.h"
#include "gbuffer.h"
#include "theater.h"

class Straw;
class Pipe;
class ObjectClass;
class PaletteClass;

enum VortexStateEnum
{
    VORTEX_STATE_0 = 0,
    VORTEX_STATE_1 = 1,
    VORTEX_STATE_2 = 2
};

class ChronalVortexClass
{
    enum
    {
        GHOST_TABLE_COUNT = 16,
        GHOST_TABLE_SIZE = 256,
        LUT_ENTRY_SIZE = 3,
        VORTEX_BUFF_SIZE = 96,
    };

public:
    ChronalVortexClass();
    ~ChronalVortexClass();

    void Appear(coord_t coord);
    void Disappear();
    void Hide();
    void Show();
    void Stop();
    void AI();
    void Movement();
    void Set_Target(ObjectClass *object);
    void Attack();
    void Zap_Target();
    void Coordinate_Remap(GraphicViewPortClass *vp, int x, int y, int w, int h, uint8_t *lut);
    void Set_Redraw();
    void Setup_Remap_Tables(TheaterType theater);
    void Build_Fading_Table(PaletteClass &palette, void *fading_tab, int color, int frac);
    void Render();
    void Detach(target_t target);
    BOOL Load(Straw &straw);
    BOOL Save(Pipe &pipe);
    BOOL Is_Active() const { return m_IsActive; }

    void Set_Range(int32_t range) { m_VortexRange = range; }
    void Set_Damage(int32_t damage) { m_VortexDamage = damage; }
    void Set_Speed(int32_t speed) { m_VortexSpeed = speed; }

private:
    coord_t m_CurrentPos;
    int32_t m_FrameIncrement;
    int32_t m_Frame;
    int32_t m_AIState;
    bool m_SkipStateTick; // Toggles each AI call to skip some logic every other tick.
    uint8_t m_GhostTable[GHOST_TABLE_COUNT][GHOST_TABLE_SIZE]; // x16 remap/palette tables [256][16]
    uint8_t m_RemapTable[GHOST_TABLE_SIZE];
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1; // 1
    BOOL m_ToHide : 1; // 2
    BOOL m_IsHidden : 1; // 4
    BOOL m_ToDeactivate : 1; // 8
#else
    bool m_IsActive;
    bool m_ToHide;
    bool m_IsHidden;
    bool m_ToDeactivate;
#endif
    TheaterType m_Theater;
    int32_t m_TargetSetFrame;
    int32_t m_AttackState;
    target_t m_Target;
    int32_t m_TargetDistance;
    int32_t m_ToDeactivateFrame;
    int32_t m_SpeedX;
    int32_t m_SpeedY;
    int32_t m_RandomSpeedX;
    int32_t m_RandomSpeedY;
    int32_t m_VortexRange;
    int32_t m_VortexSpeed;
    int32_t m_VortexDamage;
    GraphicBufferClass *m_VortexGBuffer;
};

#ifdef GAME_DLL
extern ChronalVortexClass &g_ChronalVortex;
#else
extern ChronalVortexClass g_ChronalVortex;
#endif

#endif // VORTEX_H
