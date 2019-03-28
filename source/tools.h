/**
 * @file tools.h
 * Contains different tools.
 */

#ifndef ToolsH
#define ToolsH
//---------------------------------------------------------------------------

#include "grrlib_class.h"

void RUMBLE_Wiimote(s32, int);
void RUMBLE_Verify();
void RUMBLE_Init();

void WIILIGHT_TurnOn();
void WIILIGHT_TurnOff();

void Draw_FadeInOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
void Draw_FadeIn(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);
void Draw_FadeOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed);

bool PtInCircle(const int xo, const int yo, const int radius,
                const int wpadx, const int wpady);
//---------------------------------------------------------------------------
#endif
