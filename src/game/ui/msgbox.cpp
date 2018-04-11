/**
* @file
*
* @author CCHyper
*
* @brief <todo>
*
* @copyright RedAlert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#include "msgbox.h"
#include "gbuffer.h"
#include "keyboard.h"

int MessageBoxClass::Process(const int body_text, const int button_1_text, const int button_2_text, const int button_3_text, BOOL a5)
{
#ifndef RAPP_STANDALONE
	int(*func)(MessageBoxClass *, const int, const int, const int, const int, const int) =
		reinterpret_cast<int(*)(MessageBoxClass *, const int, const int, const int, const int, const int)>(0x0050500C);
	return func(this, body_text, button_1_text, button_2_text, button_3_text, a5);
#else
	return Process(Fetch_String(body_text), Fetch_String(button_1_text), Fetch_String(button_2_text), Fetch_String(button_3_text), a5);
#endif
}

int MessageBoxClass::Process(const char *body_text, const int button_1_text, const int button_2_text, const int button_3_text, BOOL a5)
{
#ifndef RAPP_STANDALONE
	int(*func)(MessageBoxClass *, const char *, const int, const int, const int, const int) =
		reinterpret_cast<int(*)(MessageBoxClass *, const char *, const int, const int, const int, const int)>(0x0050507C);
	return func(this, body_text, button_1_text, button_2_text, button_3_text, a5);
#else
	return Process(body_text, Fetch_String(button_1_text), Fetch_String(button_2_text), Fetch_String(button_3_text), a5);
#endif
}

int MessageBoxClass::Process(const char *body_text, const char *button_1_text, const char *button_2_text, const char *button_3_text, BOOL a5)
{
#ifndef RAPP_STANDALONE
	int(*func)(MessageBoxClass *, const char *, const char *, const char *, const char *, const int) =
		reinterpret_cast<int(*)(MessageBoxClass *, const char *, const char *, const char *, const char *, const int)>(0x005043D0);
	return func(this, body_text, button_1_text, button_2_text, button_3_text, a5);
#else
	return 0;
#endif
}

