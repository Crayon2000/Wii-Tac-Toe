/**
 * @file tools.h
 * Contains different tools.
 */

#ifndef ToolsH
#define ToolsH
//---------------------------------------------------------------------------

#include "grrlib_class.h"
#include <string>

void RUMBLE_Wiimote(s32, int);
void RUMBLE_Verify();
void RUMBLE_Init();

void WIILIGHT_TurnOn();
void WIILIGHT_TurnOff();

std::string str_replace(const std::string &, const std::string &, const std::string &);
std::wstring str_replaceW(const std::wstring &, const std::wstring &, const std::wstring &);

void Draw_FadeInOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
void Draw_FadeIn(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
void Draw_FadeOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
//---------------------------------------------------------------------------
#endif
