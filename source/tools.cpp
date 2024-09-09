#include <wiiuse/wpad.h>        // Wiimote
#include <ogc/lwp_watchdog.h>   // gettime
#include <cstdlib>              // abs

#include "tools.h"
#include "grrlib_class.h"

/**
 * Structure to hold the rumble data.
 */
typedef struct _rumble_data
{
    bool rumbeling;  /**< True if Wii Remote is rumbeling, false otherwise. */
    u64 time2rumble; /**< Time to rumble in millisecond. */
} RUMBLE_DATA;

static RUMBLE_DATA Rumble_Info[WPAD_MAX_WIIMOTES];

/**
 * Set rumbeling time for a specific controller.
 * @param[in] chan Controller ID.
 * @param[in] rumble_time Time to rumble in millisecond.
 */
void RUMBLE_Wiimote(s32 chan, int rumble_time)
{
    Rumble_Info[chan].time2rumble = ticks_to_millisecs(gettime()) + rumble_time;
    Rumble_Info[chan].rumbeling = true;
    WPAD_Rumble(chan, 1); // Rumble on
}

/**
 * Initialize rumbeling to false.
 */
void RUMBLE_Init()
{
    for(u8 i = 0; i < WPAD_MAX_WIIMOTES; ++i)
    {
        Rumble_Info[i].rumbeling = false;
        Rumble_Info[i].time2rumble = 0;
    }
}

/**
 * Stop rumbeling if time is elapsed.
 */
void RUMBLE_Verify()
{
    const u64 currentTime = ticks_to_millisecs(gettime());
    for (int i = 0; i < WPAD_MAX_WIIMOTES; ++i)
    {
        if (Rumble_Info[i].rumbeling && currentTime > Rumble_Info[i].time2rumble)
        {
            WPAD_Rumble(i, 0); // Rumble off
            Rumble_Info[i].rumbeling = false;
        }
    }
}

/**
 * Turn Wii light off.
 */
void WIILIGHT_TurnOff()
{
    *(u32*)0xCD0000C0 &= ~0x20;
}

/**
 * Turn Wii light on.
 */
void WIILIGHT_TurnOn()
{
    *(u32*)0xCD0000C0 |= 0x20;
}

/**
 * Fade in, than fade out.
 * @param tex    Texture.
 * @param scaleX Texture X scale.
 * @param scaleY Texture Y scale.
 * @param speed  Fade speed (1 is the normal speed, 2 is two time the normal speed, etc).
 */
void Draw_FadeInOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed)
{
    Draw_FadeIn(tex, scaleX, scaleY, speed);
    Draw_FadeOut(tex, scaleX, scaleY, speed);
}

/**
 * Fade in.
 * @param tex    Texture.
 * @param scaleX Texture X scale.
 * @param scaleY Texture Y scale.
 * @param speed  Fade speed (1 is the normal speed, 2 is two time the normal speed, etc).
 */
void Draw_FadeIn(Texture *tex, f32 scaleX, f32 scaleY, u16 speed)
{
    const f32 xpos = (Screen::GetWidth() - tex->GetWidth()) / 2.0f;
    const f32 ypos = (Screen::GetHeight() - tex->GetHeight()) / 2.0f;

    for(s16 alpha = 0; alpha < 255; alpha += speed)
    {
        tex->Draw(xpos, ypos, 0, scaleX, scaleY, 0xFFFFFF00 | (alpha > 255 ? 255 : alpha));
        GRRLIB_Render();
    }
}

/**
 * Fade out.
 * @param tex    Texture.
 * @param scaleX Texture X scale.
 * @param scaleY Texture Y scale.
 * @param speed  Fade speed (1 is the normal speed, 2 is two time the normal speed, etc).
 */
void Draw_FadeOut(Texture *tex, f32 scaleX, f32 scaleY, u16 speed)
{
    const f32 xpos = (Screen::GetWidth() - tex->GetWidth()) / 2.0f;
    const f32 ypos = (Screen::GetHeight() - tex->GetHeight()) / 2.0f;

    for(s16 alpha = 255; alpha > 0; alpha -= speed)
    {
        tex->Draw(xpos, ypos, 0, scaleX, scaleY, 0xFFFFFF00 | (alpha < 0 ? 0 : alpha));
        GRRLIB_Render();
    }
}

/**
 * Determine whether the specified point lies within the specified circle.
 * @param xo Specifies the x-coordinate of the circle.
 * @param yo Specifies the y-coordinate of the circle.
 * @param radius The radius of the circle.
 * @param wpadx Specifies the x-coordinate of the point.
 * @param wpady Specifies the y-coordinate of the point.
 * @return If the specified point lies within the circle, the return value is true otherwise it's false.
 */
bool PtInCircle(const int xo, const int yo, const int radius,
                const int wpadx, const int wpady) {
    const int dx = std::abs(wpadx - xo);
    const int dy = std::abs(wpady - yo);

    if (dx > radius || dy > radius)
    {
        return false;
    }

    return (dx + dy <= radius) || (dx * dx + dy * dy <= radius * radius);
}
