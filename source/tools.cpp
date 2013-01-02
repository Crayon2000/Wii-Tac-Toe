#include <wiiuse/wpad.h>        // Wiimote
#include <ogcsys.h>             // nanosleep
#include <ogc/lwp_watchdog.h>   // gettime

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
    int i;
    for(i = 0; i < WPAD_MAX_WIIMOTES; ++i)
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
    if(Rumble_Info[WPAD_CHAN_0].rumbeling && ticks_to_millisecs(gettime()) > Rumble_Info[WPAD_CHAN_0].time2rumble)
    {
        WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off
        Rumble_Info[WPAD_CHAN_0].rumbeling = false;
    }
    if(Rumble_Info[WPAD_CHAN_1].rumbeling && ticks_to_millisecs(gettime()) > Rumble_Info[WPAD_CHAN_1].time2rumble)
    {
        WPAD_Rumble(WPAD_CHAN_1, 0); // Rumble off
        Rumble_Info[WPAD_CHAN_1].rumbeling = false;
    }
    if(Rumble_Info[WPAD_CHAN_2].rumbeling && ticks_to_millisecs(gettime()) > Rumble_Info[WPAD_CHAN_2].time2rumble)
    {
        WPAD_Rumble(WPAD_CHAN_2, 0); // Rumble off
        Rumble_Info[WPAD_CHAN_2].rumbeling = false;
    }
    if(Rumble_Info[WPAD_CHAN_3].rumbeling && ticks_to_millisecs(gettime()) > Rumble_Info[WPAD_CHAN_3].time2rumble)
    {
        WPAD_Rumble(WPAD_CHAN_3, 0); // Rumble off
        Rumble_Info[WPAD_CHAN_3].rumbeling = false;
    }
}

/**
 * Waits for an amount of time in msec.
 * @param[in] millisec Number of milliseconds to wait.
 */
void msleep(unsigned long millisec)
{
    struct timespec req = {0};
    time_t sec = (int)(millisec / 1000);
    millisec -= (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = millisec * 1000000L;
    while(nanosleep(&req) == -1)
    {
        continue;
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
    f32 xpos = (Screen::GetWidth() - tex->GetWidth()) / 2;
    f32 ypos = (Screen::GetHeight() - tex->GetHeight()) / 2;

    for(s16 alpha = 0; alpha < 255; alpha += speed)
    {
        if(alpha > 255)
        {
            alpha = 255;
        }
        tex->Draw(xpos, ypos, 0, scaleX, scaleY, 0xFFFFFF00 | alpha);
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
    f32 xpos = (Screen::GetWidth() - tex->GetWidth()) / 2;
    f32 ypos = (Screen::GetHeight() - tex->GetHeight()) / 2;

    for(s16 alpha = 255; alpha > 0; alpha -= speed)
    {
        if(alpha < 0)
        {
            alpha = 0;
        }
        tex->Draw(xpos, ypos, 0, scaleX, scaleY, 0xFFFFFF00 | alpha);
        GRRLIB_Render();
    }
}
