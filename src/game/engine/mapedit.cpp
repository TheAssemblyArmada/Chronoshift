/**
 * @file
 *
 * @author CCHyper
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
#include "mapedit.h"
#include "globals.h"
#include "iomap.h"
#include "cell.h"

/**
 * @brief 
 *
 * @address 0x (beta)
 */
MapEditClass::MapEditClass() :
    GameMouseClass()
{
}

MapEditClass::~MapEditClass()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::One_Time()
{
    GameMouseClass::One_Time();
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Init_IO()
{
    GameMouseClass::Init_IO();
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    if (g_InMapEditor) {
        // TODO!
    }
    GameMouseClass::AI(key, mouse_x, mouse_y);
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Draw_It(BOOL force_redraw)
{
    GameMouseClass::Draw_It(force_redraw);
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Detach(ObjectClass *object)
{
    GameMouseClass::Detach(object);
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Read_INI(GameINIClass &ini)
{
    GameMouseClass::Read_INI(ini);
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Scroll_Map(DirType dir, int &distance, BOOL redraw)
{
    if (g_InMapEditor && redraw) {
        Flag_To_Redraw(true);
    }
    return GameMouseClass::Scroll_Map(dir, distance, redraw);
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Write_INI(GameINIClass &ini)
{
    GameMouseClass::Write_INI(ini);
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Add_To_List(ObjectTypeClass *objecttype)
{
    return false;
}

/**
 * @brief Toggles between game and editor mode.
 *
 * @address 0x004BBC94 (beta)
 */
void MapEditClass::Toggle_Editor_Mode(BOOL editor_mode)
{
    if (editor_mode) {
        g_InMapEditor = true;
        g_Debug_Unshroud = true;
        Unselect_All();
        Map.Activate(0);
        Map.Init_IO();
        g_hidPage.Clear();
    } else {
        g_InMapEditor = false;
        g_Debug_Unshroud = false;
        Unselect_All();
        Map.Init_IO();
    }
    Map.Flag_To_Redraw(true);
    Map.Render();
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::New_Scenario()
{
    return false;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Load_Scenario()
{
    return false;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Save_Scenario()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Scenario_Dialog()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Pick_Scenario(char *a1, int &a2, ScenarioPlayerEnum &a3, ScenarioDirEnum &a4, ScenarioVarEnum &a5)
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Size_Map(int a1, int a2, int a3, int a4)
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Popup_Controls()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::AI_Menu()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Handle_Teams(char *a1)
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Handle_Triggers()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Main_Menu()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Mouse_Moved()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Move_Grabbed_Object()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Grab_Object()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Place_Home()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Place_Object()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Place_Next()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Place_Next_Category()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Place_Prev()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Place_Prev_Category()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Place_Trigger()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Select_Next()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Select_Object()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Select_Team(char *a1)
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Select_Trigger()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
int MapEditClass::Placement_Dialog()
{
    return 0;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Start_Base_Building()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Build_Base_To(int a1)
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Cancel_Base_Building()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Start_Placement()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Cancel_Placement()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Change_House(HousesType house)
{
    return false;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Clear_List()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
HousesType MapEditClass::Cycle_House(HousesType house, ObjectTypeClass *objecttype)
{
    return HOUSES_NONE;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Draw_Member(TechnoTypeClass *objecttype, int a2, int a3, HousesType house)
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Get_Waypoint_Name(char *a1)
{
    return false;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Set_House_Buttons(HousesType house, GadgetClass *a2, int a3)
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Verify_House(HousesType house, ObjectTypeClass *objecttype)
{
    return false;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Start_Trigger_Placement()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Stop_Trigger_Placement()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
BOOL MapEditClass::Team_Members(HousesType house)
{
    return false;
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Toggle_House()
{
}

/**
 * @brief 
 *
 * @address 0x (beta)
 */
void MapEditClass::Update_Waypoint(int a2)
{
}
