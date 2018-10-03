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

    void Init(int x, int y, int max_lines, int max_chars, int max_msg, int edit_x, int edit_y, BOOL a8, int a9, int a10,
        int width = MaxMessageWidth);
    void Reset();
    TextLabelClass *Add_Message(const char *player, int id, const char *message, PlayerColorType color,
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

#endif // MSGLIST_H
