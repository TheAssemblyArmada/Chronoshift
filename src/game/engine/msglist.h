/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for handling in game messages.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
#include "keyboard.h"
#include "remap.h"
#include "txtlabel.h"

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

    void Init(int x, int y, int max_lines, int max_chars, int max_msg, int x_offset, int y_offset, BOOL a8, int a9, int a10,
        int width = MaxMessageWidth);
    void Reset();
    TextLabelClass *Add_Message(char const *player, int id, char const *message, PlayerColorType color,
        TextPrintType style = TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL /*4046h*/, int delay = -1);
    char *Get_Message(int id);
    TextLabelClass *Get_Label(int id);
    BOOL Concat_Message(char *msg, int id, char *to_concat, int delay);
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
    int Trim_Message(char *a1, char *a2, int a3, int a4, int a5);

private:
    void Compute_Y();

private:
    TextLabelClass *CurrentMessage;
    int XPos;
    int YPos;
    int FreeSlots;
    int MaxChars;
    int MessageHeight;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool MessageListBool1 : 1; // & 1
            bool Editing : 1; // & 2
            bool MessageListBool3 : 1; // & 4
        };
        int m_listFlags;
    };
#else
    bool MessageListBool1; // 1
    bool Editing; // 2
    bool MessageListBool3; // 4
#endif
    int EditXPos;
    int EditYPos;
    TextLabelClass *EditingLabel;
    char EditBuffer[MAX_MESSAGE_LENGTH];
    char ReserveBuffer[MAX_MESSAGE_LENGTH];
    int MessageSize;
    int EditPosition;
    char EditCursor;
    int field_15D;
    int field_161;
    int Width;
    char MessageBuffers[MAX_MESSAGES][MAX_MESSAGE_LENGTH];
    bool BufferAvailable[MAX_MESSAGES];

private:
    static int MaxMessageWidth;
};

#endif // MSGLIST_H
