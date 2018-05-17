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

#ifndef RAPP_STANDALONE
void *&GameMouseClass::MouseShapes = Make_Global<void *>(0x00685160);
#else
void *GameMouseClass::MouseShapes = nullptr;
#endif


GameMouseClass::GameMouseClass()
{
#ifndef RAPP_STANDALONE
    void (*func)(const GameMouseClass *) = reinterpret_cast<void (*)(const GameMouseClass *)>(0x004D2270);
    func(this);
#endif
}

void GameMouseClass::One_Time()
{
    TabClass::One_Time();
    MouseShapes = MixFileClass<CCFileClass>::Retrieve("MOUSE.SHP");
}

void GameMouseClass::Init_Clear()
{
#ifndef RAPP_STANDALONE
    DEFINE_CALL(func, 0x005033FC, void, (GameMouseClass *));
    func(this);
#endif
}

void GameMouseClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
#ifndef RAPP_STANDALONE
    DEFINE_CALL(func, 0x0050329C, void, (const GameMouseClass *, KeyNumType &, int, int));
    func(this, key, mouse_x, mouse_y);
#endif
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
    DEFINE_CALL(func, 0x0050316C, BOOL, (GameMouseClass*, MouseType, BOOL));
    return func(this, mouse, a2);
#endif
}

void GameMouseClass::Revert_Mouse_Shape()
{
    Override_Mouse_Shape(PreviousMouseShape);
}

void GameMouseClass::Mouse_Small(BOOL a1)
{
#ifndef RAPP_STANDALONE
    DEFINE_CALL(func, 0x00503098, void, (GameMouseClass *, BOOL));
    func(this, a1);
#endif
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