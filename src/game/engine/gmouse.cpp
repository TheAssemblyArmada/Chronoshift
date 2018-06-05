/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling mouse input.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "gmouse.h"
#include "ccfileclass.h"
#include "mixfile.h"
#include "tab.h"
#include "mouseshape.h"

#ifndef RAPP_STANDALONE
void *&GameMouseClass::MouseShapes = Make_Global<void *>(0x00685160);
TCountDownTimerClass<SystemTimerClass> &GameMouseClass::AnimationTimer =
    *reinterpret_cast<TCountDownTimerClass<SystemTimerClass> *>(0x00685169);
#else
void *GameMouseClass::MouseShapes = nullptr;
TCountDownTimerClass<SystemTimerClass> GameMouseClass::AnimationTimer;
#endif

// Frame, Count, Rate, Small, HotSpotX, HotSpotY
GameMouseClass::MouseStruct GameMouseClass::MouseControl[MOUSE_COUNT] = {
    { 0, 1, 0, 80, 0, 0 }, // 0		//Arrow
    { 1, 1, 0, -1, 14, 0 }, // 1		//ScrollN
    { 2, 1, 0, -1, 29, 0 }, // 2		//ScrollNE
    { 3, 1, 0, -1, 29, 11 }, // 3		//ScrollE
    { 4, 1, 0, -1, 29, 23 }, // 4		//ScrollSE
    { 5, 1, 0, -1, 14, 23 }, // 5		//ScrollS
    { 6, 1, 0, -1, 0, 23 }, // 6		//ScrollSW
    { 7, 1, 0, -1, 0, 11 }, // 7		//ScrollW
    { 8, 1, 0, -1, 0, 0 }, // 8		//ScrollNW
    { 124, 1, 0, -1, 14, 0 }, // 9		//NoScrollN
    { 125, 1, 0, -1, 29, 0 }, // 10	//NoScrollNE
    { 126, 1, 0, -1, 29, 11 }, // 11	//NoScrollE
    { 127, 1, 0, -1, 29, 23 }, // 12	//NoScrollSE
    { 128, 1, 0, -1, 14, 23 }, // 13	//NoScrollS
    { 129, 1, 0, -1, 0, 23 }, // 14	//NoScrollSW
    { 130, 1, 0, -1, 0, 11 }, // 15	//NoScrollW
    { 131, 1, 0, -1, 0, 0 }, // 16	//NoScrollNW
    { 14, 1, 0, 33, 14, 11 }, // 17	//NoMove
    { 10, 4, 4, 29, 14, 11 }, // 18	//NotAllowed		//guessed name
    { 113, 3, 4, 142, 14, 11 }, // 19	//Enter
    { 59, 9, 4, -1, 14, 11 }, // 20	//Deploy
    { 15, 6, 4, -1, 14, 11 }, // 21	//Select
    { 21, 8, 4, 134, 14, 11 }, // 22	//Harvest
    { 68, 12, 2, -1, 14, 11 }, // 23	//Sell
    { 148, 12, 2, -1, 14, 11 }, // 24	//SellUnit
    { 35, 24, 2, -1, 14, 11 }, // 25	//Repair
    { 120, 1, 0, -1, 14, 11 }, // 26	//NoRepair
    { 119, 1, 0, -1, 14, 11 }, // 27	//NoSell
    { 81, 1, 0, 145, 14, 11 }, // 28	//
    { 90, 7, 4, -1, 14, 11 }, // 29	//NukeBomb
    { 82, 8, 2, 213, 14, 11 }, // 30	//Airstrike
    { 116, 3, 4, 121, 14, 11 }, // 31	//Sabotage
    { 147, 1, 0, 146, 14, 11 }, // 32	//GuardArea
    { 160, 4, 4, 194, 14, 11 }, // 33	//Heal
    { 164, 3, 4, 167, 14, 11 }, // 34	//Damage
    { 170, 24, 2, -1, 14, 11 }, // 35	//GRepair
    { 195, 8, 4, 203, 14, 11 }, // 36	//Attack
    { 211, 1, 0, -1, 14, 11 }, // 37	//NoDeploy
    { 212, 1, 0, -1, 14, 11 }, // 38	//NoEnter
    { 213, 1, 0, -1, 14, 11 }, // 39	//NoGRepair
    { 97, 8, 3, -1, 14, 11 }, // 40	//Chronosphere
    { 105, 8, 2, -1, 14, 11 }, // 41	//Chrono2
    { 211, 1, 0, 80, 0, 0 } // 42	//EditorBrush
};

GameMouseClass::GameMouseClass() :
    MouseInRadar(false),
    PreviousMouseShape(MOUSE_POINTER),
    MouseShape(MOUSE_POINTER),
    MouseFrame(0)
{
}

void GameMouseClass::One_Time()
{
    ScrollClass::One_Time();
    MouseShapes = MixFileClass<CCFileClass>::Retrieve("mouse.shp");
}

void GameMouseClass::Init_Clear()
{
    ScrollClass::Init_Clear();
    MouseShape = MOUSE_POINTER;
    MouseInRadar = false;
}

void GameMouseClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    int frame;

    MouseStruct *cursor = &MouseControl[MouseShape];
    if (cursor->Rate) {
        if (AnimationTimer.Time() <= 0) {
            ++MouseFrame;
            MouseFrame %= cursor->Count;
            AnimationTimer = cursor->Rate;
            if (!(MouseInRadar & 1) || cursor->Small != -1) { // eh?
                if (MouseInRadar & 1) { // isn't it pointless to check MouseInRadar now.....
                    frame = cursor->Small;
                } else {
                    frame = cursor->Frame;
                }
                void *shape = Extract_Shape(MouseShapes, MouseFrame + frame);
                g_mouse->Set_Cursor(cursor->HotSpotX, cursor->HotSpotY, shape);
            }
        }
    }
    ScrollClass::AI(key, mouse_y, mouse_y);
}

void GameMouseClass::Set_Default_Mouse(MouseType mouse, BOOL a2)
{
    if (mouse != MOUSE_NONE && mouse < MOUSE_COUNT) {
        MouseShape = mouse;
        Override_Mouse_Shape(mouse, a2);
    }
}

BOOL GameMouseClass::Override_Mouse_Shape(MouseType mouse, BOOL a2)
{
#ifndef RAPP_STANDALONE
    DEFINE_CALL(func, 0x0050316C, BOOL, GameMouseClass *, MouseType, BOOL);
    return func(this, mouse, a2);
#else
    int v3; // eax@3
    CDTimerClass<SystemTimerClass> *v4; // eax@12
    void *v5; // ebx@15
    int v6; // edx@15
    signed int v8; // [sp+0h] [bp-2Ch]@11
    int v9; // [sp+8h] [bp-24h]@13
    CountDownTimerClass v10; // [sp+Ch] [bp-20h]@12
    MouseStruct *v11; // [sp+18h] [bp-14h]@4
    int v12; // [sp+1Ch] [bp-10h]@2
    const char *v14; // [sp+24h] [bp-8h]@1

    v14 = a3;
    if (mouse >= 0x27) {
        Assert("(unsigned)mouse < MOUSE_COUNT", 172, "mouse.cpp");
        v12 = v3;
    } else {
        v12 = 0;
    }
    v11 = &MouseClass::MouseControl[mouse];
    if (v11->MiniFrame == -1) {
        v14 = 0;
    }
    if (Override_Mouse_Shape::.0 ::startup
        && (!MouseClass::MouseShapes || mouse == this->PreviousMouseShape && (*&this->MouseClass_Bitfield1 & 1) == v14)) {
        v8 = 0;
    } else {
        Override_Mouse_Shape::.0 ::startup = 1;
        AnimationTimer = v11->Rate;
        this->CurrentMouseFrame = 0;
        if (v14) {
            v9 = v11->MiniFrame;
        } else {
            v9 = v11->Frame;
        }
        v5 = Extract_Shape(MouseClass::MouseShapes,
            v9
                + ((this->CurrentMouseFrame
                       - (__CFSHL__(this->CurrentMouseFrame >> 32, 2) + 4 * (this->CurrentMouseFrame >> 32)))
                      >> 2));
        Set_Mouse_Cursor(v11->HotSpotX, v11->HotSpotY, v5);
        this->PreviousMouseShape = mouse;
        v6 = v14 & 1;
        this->MouseClass_Bitfield1 &= 0xFEu;
        *&this->MouseClass_Bitfield1 |= v6;
        v8 = 1;
    }
    return v8;
#endif
}

void GameMouseClass::Revert_Mouse_Shape()
{
    Override_Mouse_Shape(PreviousMouseShape);
}

void GameMouseClass::Mouse_Small(BOOL use_small_frame)
{
    if (MouseInRadar != use_small_frame) {
        if (use_small_frame) {
            if (MouseControl[PreviousMouseShape].Small == -1) {
                g_mouse->Set_Cursor(MouseControl[MOUSE_POINTER].HotSpotX,
                    MouseControl[MOUSE_POINTER].HotSpotY,
                    Extract_Shape(MouseShapes, 0));
            } else {
                g_mouse->Set_Cursor(MouseControl[MouseShape].HotSpotX,
                    MouseControl[MouseShape].HotSpotY,
                    Extract_Shape(MouseShapes, MouseControl[MouseShape].Small + MouseFrame));
            }
        } else {
            g_mouse->Set_Cursor(MouseControl[MouseShape].HotSpotX,
                MouseControl[MouseShape].HotSpotY,
                Extract_Shape(MouseShapes, MouseControl[MouseShape].Frame + MouseFrame));
        }
    }
}

MouseType GameMouseClass::Get_Mouse_Shape(void) const
{
    return MouseShape;
}

BOOL GameMouseClass::Load(Straw &straw)
{
    return false; // TODO
}

BOOL GameMouseClass::Save(Pipe &pipe) const
{
    return false; // TODO
}