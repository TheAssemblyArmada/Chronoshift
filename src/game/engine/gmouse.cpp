/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling mouse input.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gmouse.h"
#include "animtype.h"
#include "buildingtype.h"
#include "gamedebug.h"
#include "gamefile.h"
#include "mouse.h"
#include "mouseshape.h"
#include "overlaytype.h"
#include "pipe.h"
#include "scenario.h"
#include "shape.h"
#include "smudgetype.h"
#include "straw.h"
#include "templatetype.h"
#include "terraintype.h"
#include "coord.h"
#include <new>

#ifndef GAME_DLL
void *GameMouseClass::s_MouseShapes = nullptr;
TCountDownTimerClass<SystemTimerClass> GameMouseClass::s_AnimationTimer;
#endif

// Frame, Count, Rate, Small, HotSpotX, HotSpotY
GameMouseClass::MouseStruct GameMouseClass::s_MouseControl[MOUSE_COUNT] = {
    { 0, 1, 0, 80, 0, 0 }, // 0  //Arrow
    { 1, 1, 0, -1, 14, 0 }, // 1  //ScrollN
    { 2, 1, 0, -1, 29, 0 }, // 2  //ScrollNE
    { 3, 1, 0, -1, 29, 11 }, // 3  //ScrollE
    { 4, 1, 0, -1, 29, 23 }, // 4  //ScrollSE
    { 5, 1, 0, -1, 14, 23 }, // 5  //ScrollS
    { 6, 1, 0, -1, 0, 23 }, // 6  //ScrollSW
    { 7, 1, 0, -1, 0, 11 }, // 7  //ScrollW
    { 8, 1, 0, -1, 0, 0 }, // 8  //ScrollNW
    { 124, 1, 0, -1, 14, 0 }, // 9  //NoScrollN
    { 125, 1, 0, -1, 29, 0 }, // 10 //NoScrollNE
    { 126, 1, 0, -1, 29, 11 }, // 11 //NoScrollE
    { 127, 1, 0, -1, 29, 23 }, // 12 //NoScrollSE
    { 128, 1, 0, -1, 14, 23 }, // 13 //NoScrollS
    { 129, 1, 0, -1, 0, 23 }, // 14 //NoScrollSW
    { 130, 1, 0, -1, 0, 11 }, // 15 //NoScrollW
    { 131, 1, 0, -1, 0, 0 }, // 16 //NoScrollNW
    { 14, 1, 0, 33, 14, 11 }, // 17 //NoMove
    { 10, 4, 4, 29, 14, 11 }, // 18 //NotAllowed
    { 113, 3, 4, 142, 14, 11 }, // 19 //Enter
    { 59, 9, 4, -1, 14, 11 }, // 20 //Deploy
    { 15, 6, 4, -1, 14, 11 }, // 21 //Select
    { 21, 8, 4, 134, 14, 11 }, // 22 //Harvest
    { 68, 12, 2, -1, 14, 11 }, // 23 //Sell
    { 148, 12, 2, -1, 14, 11 }, // 24 //SellUnit
    { 35, 24, 2, -1, 14, 11 }, // 25 //Repair
    { 120, 1, 0, -1, 14, 11 }, // 26 //NoRepair
    { 119, 1, 0, -1, 14, 11 }, // 27 //NoSell
    { 81, 1, 0, 145, 14, 11 }, // 28 //
    { 90, 7, 4, -1, 14, 11 }, // 29 //NukeBomb
    { 82, 8, 2, 213, 14, 11 }, // 30 //Airstrike
    { 116, 3, 4, 121, 14, 11 }, // 31 //Sabotage
    { 147, 1, 0, 146, 14, 11 }, // 32 //GuardArea
    { 160, 4, 4, 194, 14, 11 }, // 33 //Heal
    { 164, 3, 4, 167, 14, 11 }, // 34 //Damage
    { 170, 24, 2, -1, 14, 11 }, // 35 //GRepair
    { 195, 8, 4, 203, 14, 11 }, // 36 //Attack
    { 211, 1, 0, -1, 14, 11 }, // 37 //NoDeploy
    { 212, 1, 0, -1, 14, 11 }, // 38 //NoEnter
    { 213, 1, 0, -1, 14, 11 }, // 39 //NoGRepair
    { 97, 8, 3, -1, 14, 11 }, // 40 //Chronosphere
    { 105, 8, 2, -1, 14, 11 }, // 41 //Chrono2
    { 211, 1, 0, 80, 0, 0 } // 42 //EditorBrush RA++ addition
};

GameMouseClass::GameMouseClass() :
    m_MouseInRadar(false),
    m_CurrentMouse(MOUSE_POINTER),
    m_DefaultMouse(MOUSE_POINTER),
    m_MouseFrame(0)
{
}

void GameMouseClass::One_Time()
{
    ScrollClass::One_Time();
    s_MouseShapes = GameFileClass::Retrieve("mouse.shp");
}

void GameMouseClass::Init_Clear()
{
    ScrollClass::Init_Clear();
    m_DefaultMouse = MOUSE_POINTER;
    m_MouseInRadar = false;
}

void GameMouseClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    MouseStruct &cursor = s_MouseControl[m_CurrentMouse];

    if (cursor.Rate) {
        if (s_AnimationTimer == 0) {
            ++m_MouseFrame;
            m_MouseFrame %= cursor.Count;
            s_AnimationTimer = cursor.Rate;

            // If we have a small cursor or we aren't in radar evaluate setting a new cursor.
            if (!m_MouseInRadar || cursor.Small != -1) {
                int frame = 0;

                if (m_MouseInRadar) {
                    frame = cursor.Small;
                } else {
                    frame = cursor.Frame;
                }

                g_mouse->Set_Cursor(cursor.HotSpotX, cursor.HotSpotY, Extract_Shape(s_MouseShapes, m_MouseFrame + frame));
            }
        }
    }

    ScrollClass::AI(key, mouse_x, mouse_y);
}

void GameMouseClass::Set_Default_Mouse(MouseType mouse, BOOL in_radar)
{
    if (mouse != MOUSE_NONE && mouse < MOUSE_COUNT) {
        m_DefaultMouse = mouse;
        Override_Mouse_Shape(mouse, in_radar);
    }
}

/**
 * @brief Forces the mouse sprite to the requested type.
 *
 * 0x0050316C
 */
BOOL GameMouseClass::Override_Mouse_Shape(MouseType mouse, BOOL in_radar)
{
    DEBUG_ASSERT_PRINT(mouse != MOUSE_NONE, "mouse = %d.", mouse);
    DEBUG_ASSERT_PRINT(mouse < MOUSE_COUNT, "mouse = %d.", mouse);

    static BOOL _startup = false;

    MouseStruct &cursor = s_MouseControl[mouse];

    if (cursor.Small == -1) {
        in_radar = false;
    }
    
    // No need to change it, we are already using the correct cursor.
    if (_startup && (s_MouseShapes == nullptr || (mouse == m_CurrentMouse && m_MouseInRadar == in_radar))) {
        return false;
    }

    _startup = true;
    s_AnimationTimer = cursor.Rate;
    m_MouseFrame = 0;
    int frame = in_radar ? (cursor.Small == -1 ? cursor.Frame : cursor.Small) : cursor.Frame;
    g_mouse->Set_Cursor(cursor.HotSpotX, cursor.HotSpotY, Extract_Shape(s_MouseShapes, frame));
    m_CurrentMouse = mouse;
    m_MouseInRadar = in_radar;

    return true;
}

void GameMouseClass::Mouse_Small(BOOL use_small_frame)
{
    if (m_MouseInRadar != use_small_frame) {
        if (use_small_frame) {
            if (s_MouseControl[m_CurrentMouse].Small == -1) {
                g_mouse->Set_Cursor(s_MouseControl[MOUSE_POINTER].HotSpotX,
                    s_MouseControl[MOUSE_POINTER].HotSpotY,
                    Extract_Shape(s_MouseShapes, 0));
            } else {
                g_mouse->Set_Cursor(s_MouseControl[m_DefaultMouse].HotSpotX,
                    s_MouseControl[m_DefaultMouse].HotSpotY,
                    Extract_Shape(s_MouseShapes, s_MouseControl[m_DefaultMouse].Small + m_MouseFrame));
            }
        } else {
            g_mouse->Set_Cursor(s_MouseControl[m_DefaultMouse].HotSpotX,
                s_MouseControl[m_DefaultMouse].HotSpotY,
                Extract_Shape(s_MouseShapes, s_MouseControl[m_DefaultMouse].Frame + m_MouseFrame));
        }
    }
}

/**
 * @brief Loads data from a Straw interface for save game purposes.
 *
 * 0x004F8F70
 */
BOOL GameMouseClass::Load(Straw &straw)
{
/*#ifdef COMPILER_WATCOM
    DEBUG_ASSERT_PRINT(sizeof(*this) == 5739, "IOMap object is %d bytes instead of expected 5739 bytes", sizeof(*this));
#endif*/

    TheaterType theater = THEATER_NONE;
    NoInitClass noinit;

    if (straw.Get(&theater, sizeof(theater)) == sizeof(theater)) {
        g_lastTheater = THEATER_NONE;
        Reset_Theater_Shapes();
        Init_Theater(theater);
        TerrainTypeClass::Init(theater);
        TemplateTypeClass::Init(theater);
        OverlayTypeClass::Init(theater);
        BuildingTypeClass::Init(theater);
        AnimTypeClass::Init(theater);
        SmudgeTypeClass::Init(theater);
        Free_Cells();

        // TODO this needs breaking down into each class loading its own members in a size and endian controlled way
        // so we have a nice well defined save game format.
        straw.Get(this, sizeof(*this));
        new(this) GameMouseClass(noinit);
        Alloc_Cells();
        Init_Cells();

        uint32_t saved_cells = 0;

        if (straw.Get(&saved_cells, sizeof(saved_cells)) == sizeof(saved_cells)) {
            DEBUG_LOG("Loading data for %u cells.\n", saved_cells);
            for (unsigned i = 0; i < saved_cells; ++i) {
                cell_t cell_num;

                if (straw.Get(&cell_num, sizeof(cell_num)) != sizeof(cell_num)) {
                    return false;
                }

                if (!Array[cell_num].Load(straw)) {
                    return false;
                }
            }

            g_lastTheater = Scen.Get_Theater();

            return true;
        }
    }

    return false;
}

/**
 * @brief Saves data to a Pipe interface for save game purposes.
 *
 * 0x004F90F8
 */
BOOL GameMouseClass::Save(Pipe &pipe) const
{
/*#ifdef COMPILER_WATCOM
    DEBUG_ASSERT_PRINT(sizeof(*this) == 5739, "IOMap object is %d bytes instead of expected 5739 bytes", sizeof(*this));
#endif*/

    TheaterType theater = Scen.Get_Theater();
    pipe.Put(&theater, sizeof(theater));
    pipe.Put(this, sizeof(*this));
    uint32_t saved_cells = 0;

    // Count how many cells have state that needs saving.
    for (cell_t i = 0; i < MAP_MAX_AREA; ++i) {
        if (Array[i].Should_Save()) {
            ++saved_cells;
        }
    }

    DEBUG_LOG("Saving %u cells with none default state\n", saved_cells);
    pipe.Put(&saved_cells, sizeof(saved_cells));

    // Save the cells that need saving.
    for (cell_t i = 0; i < MAP_MAX_AREA; ++i) {
        if (Array[i].Should_Save()) {
            pipe.Put(&i, sizeof(i));
            Array[i].Save(pipe);
            --saved_cells;
        }
    }

    DEBUG_ASSERT_PRINT(saved_cells == 0, "IOMap save failed to save %d cells that were counted to save.\n", saved_cells);

    return saved_cells == 0;
}
