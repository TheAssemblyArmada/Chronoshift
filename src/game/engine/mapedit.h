/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Part of IOMap stack handling the map editor interface.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MAPEDIT_H
#define MAPEDIT_H

#include "always.h"
#include "gmouse.h"
#include "scenario.h"

class NoInitClass;
class TechnoTypeClass;

class MapEditClass : public GameMouseClass
{
public:
    MapEditClass();
    MapEditClass(const NoInitClass &noinit) : GameMouseClass(noinit) {}

    ~MapEditClass();

    virtual void One_Time() override;
    virtual void Init_IO() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw = false) override;
    virtual void Detach(ObjectClass *object) override;
    virtual void Read_INI(GameINIClass &ini) override;
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;
    virtual void Write_INI(GameINIClass &ini) override;

    BOOL Add_To_List(ObjectTypeClass *objecttype);
    static void Toggle_Editor_Mode(BOOL editor_mode);

private:
    BOOL New_Scenario();
    BOOL Load_Scenario();
    int Save_Scenario();
    int Scenario_Dialog();
    int Pick_Scenario(char *a1, int &a2, ScenarioPlayerEnum &a3, ScenarioDirEnum &a4, ScenarioVarEnum &a5);
    int Size_Map(int a1, int a2, int a3, int a4);
    void Popup_Controls();
    void AI_Menu();
    void Handle_Teams(char *a1);
    void Handle_Triggers();
    void Main_Menu();
    BOOL Mouse_Moved();
    BOOL Move_Grabbed_Object();
    void Grab_Object();
    void Place_Home();
    BOOL Place_Object();
    void Place_Next();
    void Place_Next_Category();
    void Place_Prev();
    void Place_Prev_Category();
    void Place_Trigger();
    void Select_Next();
    int Select_Object();
    int Select_Team(char *a1);
    int Select_Trigger();
    int Placement_Dialog();
    void Start_Base_Building();
    void Build_Base_To(int a1);
    void Cancel_Base_Building();
    void Start_Placement();
    void Cancel_Placement();
    BOOL Change_House(HousesType house);
    void Clear_List();
    HousesType Cycle_House(HousesType house, ObjectTypeClass *objecttype);
    void Draw_Member(TechnoTypeClass *objecttype, int a2, int a3, HousesType house);
    BOOL Get_Waypoint_Name(char *a1);
    void Set_House_Buttons(HousesType house, GadgetClass *a2, int a3);
    BOOL Verify_House(HousesType house, ObjectTypeClass *objecttype);
    void Start_Trigger_Placement();
    void Stop_Trigger_Placement();
    BOOL Team_Members(HousesType house);
    void Toggle_House();
    void Update_Waypoint(int a2);

protected:
    // NOTE: All members for this class must be static otherwise it
    //       it will break the existing MapIO we hook into in RA!
};

#endif // MAPEDIT_H
