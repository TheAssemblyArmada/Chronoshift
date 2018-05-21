#include "mapedit.h"
#include "dialog.h"
#include "controlc.h"
#include "scenario.h"
#include "gmouse.h"
#include "globals.h"
#include "iomap.h"

MapEditorClass::MapEditorClassStruct MapEditorClass::MapEditor;
MapEditorClass::TacticalClass MapEditorClass::EditorTacButton;

BOOL MapEditorClass::TacticalClass::Action(unsigned flags, KeyNumType &key)
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
    // xpos = Clamp(mouse_x, 0, Lepton_To_Pixel(Map.DisplayHeight) - 1);
    // ypos = Clamp(mouse_y, 16, Lepton_To_Pixel(Map.DisplayWidth) + 15);

    if (/*Map.PendingObjectTypePtr ||*/ MapEditor.CurrentObject) {
        if (flags & MOUSE_LEFT_PRESS /*&& PendingObjectTypePtr && !MapEditorClass::Evaluate_What_Was_Clicked(&Map)*/) {
            MapEditor.HasUnsavedChanges = true;
        }
        if (flags & MOUSE_LEFT_HELD /*&& MapEditorClass_44A399(&Map)*/) {
            /*
            if (Map.PendingObjectTypePtr) {
                Map.Flag_To_Redraw(0);
                if (!MapEditorClass::Evaluate_What_Was_Clicked()) {
                    MapEditor.HasUnsavedChanges = true;
                }
            */
        } else {
            MapEditor.CurrentObject->Mark(MARK_REDRAW);
            /*
            if (!Make_Selection()) {
                MapEditor.HasUnsavedChanges = true;
            }
            */
        }
        if (flags & MOUSE_LEFT_RLSE && MapEditor.CurrentObject) {
            MapEditor.CurrentObject = 0;
            // v8 = VectorClass<ObjectClass *>::operator[](&MapEditor.CurrentObject, 0);
            //(*v8)->a.vftble->Unselect(*v8, (*v8)->a.vftble, v7);
        }
    } else {
        if (flags & MOUSE_LEFT_PRESS) {
            Map.Refresh_Band();
            /*
            Map.DisplayBit8 = true;
            Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaBottomRightCell = RadarClass::Click_Cell_Calc(xpos, ypos);
            Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaTopLeftCell = Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaBottomRightCell;
            Map.DisplayToRedraw = true;
            */
            Map.Flag_To_Redraw(0);
            
        }
        if (flags & MOUSE_LEFT_HELD) {
            /*
            if (Map.DisplayBit8) {
                v11 = RadarClass::Click_Cell_Calc(xpos, ypos);
                if (v11 != Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaBottomRightCell) {
                    Map.Refresh_Band();
                    Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaBottomRightCell = v11;
                    Map.DisplayToRedraw = true;
                    Map.Flag_To_Redraw(0);
                }
                
            }
            */
        }
        if (flags & MOUSE_LEFT_RLSE /*&& Map.DisplayBit8*/) {
            Map.Refresh_Band();
            /*
            Map.DisplayBit8 = false;
            Map.DisplayBit16 = false;
            Map.DisplayToRedraw = true;
            */
            Map.Flag_To_Redraw(0);
        }
        if (flags & MOUSE_LEFT_UP /*&& Map.DisplayBit8*/) {
            /*
            LOBYTE(flags) = flags | 4;
            */
        }
    }
    if (flags & MOUSE_RIGHT_PRESS) {
        /*
        if (MapEditorClass::Clicked_In_Selection(RadarClass::Click_Cell_Calc(xpos, ypos))) {
            MapEditorClass::Draw_Rightclick_Menu(xpos, ypos);
        } else {
            MapEditorClass::Unselect();
        }
        */
    }
    return ControlClass::Action(flags, key);
}

MapEditorClass::MapEditorClass()
{
    MapEditor.CurrentObject = nullptr;

    for (int i = 0; i < 9; ++i) { // need to find out whats 9
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
    EditorTacButton.Set_ID(515);
    EditorTacButton.Set_Position(0, 16);
    EditorTacButton.Set_Size(480, 384);

    WindowList[WINDOW_5].X = 0;
    WindowList[WINDOW_5].Y = 16;
    WindowList[WINDOW_5].W = 480;
    WindowList[WINDOW_5].H = 384;
}

void MapEditorClass::Init_IO()
{
    GameMouseClass::Init_IO();
    if (g_inMapEditor) {
        Add_A_Button(EditorTacButton);
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

signed int MapEditorClass::Make_Selection()
{
    /*
    int v11; // [sp+4h] [bp-24h]@1
    signed int v12; // [sp+8h] [bp-20h]@1

    v12 = -1;
    CurrentObject->Mark(0);
    v11 = Cell_To_Coord((a1->DisplayCursorStart_8CA9 + DisplayCursorStart));
    if (ObjectClass_inlined_41B1E8(a1->CurrentObject) == 13) {
        v11 &= 0xFF00FF00;
    }
    if (CurrentObject->Can_Enter_Cell(Coord_Cell(v11), Coord_defarg_44BA2C)) {
        v11 = 0;
    }
    if (v11) {
        CurrentObject->Coord = v11;
        v12 = 0;
    }
    CurrentObject->Mark(1);
    Set_Default_Mouse(0, Set_Default_Mouse_defarg);
    Override_Mouse_Shape(0, Override_Mouse_Shape_defarg_0);
    Flag_To_Redraw)(1);
    return v12;
    */
    return -1;
}

BOOL MapEditorClass::Clicked_In_Selection(int16_t clicked_cell)
{
    /*
    int v2; // ST0C_4@4
    signed int v3; // ST08_4@6
    int v5; // [sp+0h] [bp-30h]@15
    int v6; // [sp+4h] [bp-2Ch]@13
    signed int clicked_y; // [sp+10h] [bp-20h]@3
    int clicked_x; // [sp+14h] [bp-1Ch]@3
    signed int v9; // [sp+18h] [bp-18h]@3
    int v10; // [sp+1Ch] [bp-14h]@3
    signed int v11; // [sp+20h] [bp-10h]@3
    int v12; // [sp+24h] [bp-Ch]@3

    if (!Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaTopLeftCell && !Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaBottomRightCell) {
        return false;
    } else {
        v12 = INLINE_Get_Cell_X(*(&Map.mo.sc.hl.tab.sb.rd.dp.PressedSidebarButton + 2) >> 16);
        v11 = INLINE_Get_Cell_Y(*(&Map.mo.sc.hl.tab.sb.rd.dp.PressedSidebarButton + 2) >> 16);
        v10 = INLINE_Get_Cell_X(*&Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaTopLeftCell >> 16);
        v9 = INLINE_Get_Cell_Y(*&Map.mo.sc.hl.tab.sb.rd.dp.SelectedAreaTopLeftCell >> 16);
        clicked_x = INLINE_Get_Cell_X(clicked_cell);
        clicked_y = INLINE_Get_Cell_Y(clicked_cell);
        if (v12 > v10) {
            v2 = v12;
            v12 = v10;
            v10 = v2;
        }
        if (v11 > v9) {
            v3 = v11;
            v11 = v9;
            v9 = v3;
        }
        v6 = clicked_x >= v12 && clicked_x <= v10 && clicked_y >= v11 && clicked_y <= v9;
        v5 = v6;
    }
    return v5;
    */
    return false;
}