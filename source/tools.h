/**
 * @file tools.h
 * Contains different tools.
 */

#ifndef ToolsH
#define ToolsH
//---------------------------------------------------------------------------

#include "grrlib_class.h"
#include <string>
using namespace std;

void RUMBLE_Wiimote(s32, int);
void RUMBLE_Verify();
void RUMBLE_Init();

void WIILIGHT_TurnOn();
void WIILIGHT_TurnOff();

string str_replace(const string &, const string &, const string &);
wstring str_replaceW(const wstring &, const wstring &, const wstring &);

void GRRLIB_DrawImg_FadeInOut(struct GRRLIB_texImg *tex, f32 scaleX, f32 scaleY, u16 speed);
void GRRLIB_DrawImg_FadeIn(struct GRRLIB_texImg *tex, f32 scaleX, f32 scaleY, u16 speed);
void GRRLIB_DrawImg_FadeOut(struct GRRLIB_texImg *tex, f32 scaleX, f32 scaleY, u16 speed);
void Draw_FadeInOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
void Draw_FadeIn(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
void Draw_FadeOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
//---------------------------------------------------------------------------
#endif
