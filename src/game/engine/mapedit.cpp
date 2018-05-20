#include "mapedit.h"
#include "dialog.h"
#include "controlc.h"
#include "scenario.h"
#include "gmouse.h"
#include "globals.h"

MapEditorClass::MapEditorClassStruct MapEditorClass::MapEditor;
MapEditorClass::EditorViewportInputClass MapEditorClass::EditorViewportInput;

BOOL MapEditorClass::EditorViewportInputClass::Action(unsigned flags, KeyNumType &key)
{

    int mouse_y = 0;
    int mouse_x = 0;

    if (flags & MOUSE_LEFT_PRESS || flags & MOUSE_LEFT_RLSE) {
        mouse_x = g_mouse->Get_Mouse_X();
        mouse_y = g_mouse->Get_Mouse_Y();
    } else {
        mouse_y = g_keyboard->Get_MouseQY();
        mouse_x = g_keyboard->Get_MouseQX();
    }
    // TODO
    return ControlClass::Action(flags, key);
}

MapEditorClass::MapEditorClass()
{
    MapEditor.CurrentObject = nullptr;

    for (int i = 0; i < 9; ++i) { //need to find out whats 9
        MapEditor.field_8CAF[i] = 0;
        MapEditor.field_8CD3[i] = 0;
    }
    Scen.Set_Waypoint(98, 0);
    MapEditor.HasUnsavedChanges = false;
    MapEditor.Bit_2 = false;
    MapEditor.ShowPassable = false;
}

void MapEditorClass::One_Time()
{
    GameMouseClass::One_Time();
    EditorViewportInput.Set_ID(515);
    EditorViewportInput.Set_Position(0, 16);
    EditorViewportInput.Set_Size(480, 384);

    WindowList[WINDOW_5].X = 0;
    WindowList[WINDOW_5].Y = 16;
    WindowList[WINDOW_5].W = 480;
    WindowList[WINDOW_5].H = 384;
}

void MapEditorClass::Init_IO()
{
    GameMouseClass::Init_IO();
    if (g_inMapEditor) {
        Add_A_Button(EditorViewportInput);
    }
}

void MapEditorClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    // TODO
    GameMouseClass::AI(key, mouse_x, mouse_y);
}

void MapEditorClass::Draw_It(BOOL force_redraw)
{
    CreditDisplay.Set_Available(TotalValue);
    GameMouseClass::Draw_It(force_redraw);
}

void MapEditorClass::Detach(ObjectClass *object)
{
    if (MapEditor.CurrentObject == object) {
        MapEditor.CurrentObject = 0;
    }
}

void MapEditorClass::Read_INI(CCINIClass &ini)
{
    GameMouseClass::Read_INI(ini);
}

BOOL MapEditorClass::Scroll_Map(DirType dir, int &distance, BOOL redraw)
{
    return GameMouseClass::Scroll_Map(dir, distance, redraw);
}

void MapEditorClass::Write_INI(CCINIClass &ini)
{
    DisplayClass::Write_INI(ini);
}