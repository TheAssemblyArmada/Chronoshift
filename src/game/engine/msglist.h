/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for handling in game messages.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MSGLIST_H
#define MSGLIST_H

#include "always.h"
#include "dialog.h"
#include "keyboard.h"
#include "remap.h"

class TextLabelClass;

class MessageListClass
{
    enum
    {
        MAX_MESSAGES = 14,
        MAX_MESSAGE_LENGTH = 150,
    };

public:
    MessageListClass();
    ~MessageListClass();

    void Init(int x, int y, int max_lines, int max_chars, int max_msg, int edit_x, int edit_y, BOOL a8, int a9, int a10,
        int width = MaxMessageWidth);
    void Reset();
    TextLabelClass *Add_Message(const char *player, int id, const char *message, PlayerColorType color,
        TextPrintType style = TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL /*4046h*/, int lifetime = -1);
    TextLabelClass *Add_Simple_Message(const char *player, const char *message, PlayerColorType color);
    char *Get_Message(int id);
    TextLabelClass *Get_Label(int id);
    BOOL Concat_Message(char *msg, int id, char *to_concat, int lifetime = -1);
    void Set_Edit_Focus();
    bool Has_Edit_Focus();
    TextLabelClass *Add_Edit(PlayerColorType player, TextPrintType style, char *string, char cursor, int width);
    void Remove_Edit();
    char *Get_Edit_Buf();
    void Set_Edit_Color(PlayerColorType player);
    BOOL Manage();
    int Input(KeyNumType &key);
    void Draw();
    int Num_Messages();
    void Set_Width(int width);

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

private:
    void Compute_Y();
    int Trim_Message(char *reserve, char *msg, int trim_start, int trim_end, BOOL trim_right);

private:
    TextLabelClass *LabelList;
    int XPos;
    int YPos;
    int FreeSlots;
    int MaxChars;
    int MessageHeight;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Concatenate : 1; // & 1
            bool Editing : 1; // & 2
            bool EditAsMessage : 1; // & 4
        };
        int m_listFlags;
    };
#else
    bool Concatenate; // 1
    bool Editing; // 2
    bool EditAsMessage; // 4
#endif
    int EditXPos;
    int EditYPos;
    TextLabelClass *EditingLabel;
    char EditBuffer[MAX_MESSAGE_LENGTH];
    char ReserveBuffer[MAX_MESSAGE_LENGTH];
    int EditPos;
    int EditStart;
    char EditCursor;
    int EditTrimStart;
    int EditTrimEnd;
    int Width;
    char MessageBuffers[MAX_MESSAGES][MAX_MESSAGE_LENGTH];
    bool SlotAvailable[MAX_MESSAGES];

private:
    static int MaxMessageWidth;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void MessageListClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x00505244, *MessageListClass::Init);
    Hook_Function(0x005053AC, *MessageListClass::Reset);
    Hook_Function(0x0050542C, *MessageListClass::Add_Message);
    Hook_Function(0x005057B0, *MessageListClass::Get_Message);
    Hook_Function(0x005057DC, *MessageListClass::Get_Label);
    Hook_Function(0x00505804, *MessageListClass::Concat_Message);
    Hook_Function(0x00505AE4, *MessageListClass::Set_Edit_Focus);
    Hook_Function(0x00505B00, *MessageListClass::Has_Edit_Focus);
    Hook_Function(0x00505B20, *MessageListClass::Add_Edit);
    Hook_Function(0x00505C9C, *MessageListClass::Remove_Edit);
    Hook_Function(0x00505CDC, *MessageListClass::Get_Edit_Buf);
    Hook_Function(0x00505CF4, *MessageListClass::Set_Edit_Color);
    Hook_Function(0x00505D18, *MessageListClass::Manage);
    Hook_Function(0x00505DEC, *MessageListClass::Input);
    Hook_Function(0x005060E4, *MessageListClass::Draw);
    Hook_Function(0x005061DC, *MessageListClass::Num_Messages);
    Hook_Function(0x00506214, *MessageListClass::Set_Width);
    Hook_Function(0x00506248, *MessageListClass::Trim_Message);
    Hook_Function(0x00506340, *MessageListClass::Compute_Y);
#endif
}
#endif

#endif // MSGLIST_H
