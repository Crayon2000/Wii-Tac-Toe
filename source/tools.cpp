#include <wiiuse/wpad.h>        // Wiimote
#include <ogcsys.h>             // nanosleep
#include <ogc/lwp_watchdog.h>   // gettime

#include "grrlib.h"
#include "tools.h"

/**
 * Structure to hold the rumble data.
 */
typedef struct _rumble_data
{
    bool rumbeling;  /**< True if Wii Remote is rumbeling, false otherwise. */
    u64 time2rumble; /**< Time to rumble in milisecond. */
} RUMBLE_DATA;

static RUMBLE_DATA Rumble_Info[WPAD_MAX_WIIMOTES];

/**
 * Set rumbeling time for a specific controller.
 * @param[in] chan Controller ID
 * @param[in] rumble_time Time to rumble in milisecond
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
    for(i = 0; i < WPAD_MAX_WIIMOTES; i++)
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
 * @param[in] milisec Number of milliseconds to wait.
 */
void msleep(unsigned long milisec)
{
    struct timespec req = {0};
    time_t sec = (int)(milisec / 1000);
    milisec -= (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = milisec * 1000000L;
    while(nanosleep(&req) == -1)
        continue;
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
 * Replace a string by another string in a string.
 * @param[in] txt String to search.
 * @param[in] Before String to be replaced.
 * @param[in] After Replacement string.
 * @return Pointer to the new string with replaced string, must be freed
 */
string str_replace(const string &txt, const string &Before, const string &After)
{
    string str = txt;
    size_t pos = 0, AfterSize = After.length(), BeforeSize = Before.length();
    while((pos = str.find(Before, pos)) != string::npos)
    {
        str.replace(pos, BeforeSize, After, 0, AfterSize);
        pos += AfterSize;
    }
    return str;
}

/**
 * Replace a wide string by another wide string in a wide string.
 * @param[in] txt String to search.
 * @param[in] Before String to be replaced.
 * @param[in] After Replacement string.
 * @return Pointer to the new wide string with replaced string, must be freed
 */
wstring str_replaceW(const wstring &txt, const wstring &Before, const wstring &After)
{
    wstring str = txt;
    size_t pos = 0, AfterSize = After.length(), BeforeSize = Before.length();
    while((pos = str.find(Before, pos)) != string::npos)
    {
        str.replace(pos, BeforeSize, After, 0, AfterSize);
        pos += AfterSize;
    }
    return str;
}

/**
 * Fade in, than fade out.
 * @param tex    Texture.
 * @param scaleX Texture X scale.
 * @param scaleY Texture Y scale.
 * @param speed  Fade speed (1 is the normal speed, 2 is two time the normal speed, etc).
 */
void GRRLIB_DrawImg_FadeInOut(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed)
{
    GRRLIB_DrawImg_FadeIn(tex, scaleX, scaleY, speed);
    GRRLIB_DrawImg_FadeOut(tex, scaleX, scaleY, speed);
}

/**
 * Fade in.
 * @param tex    Texture.
 * @param scaleX Texture X scale.
 * @param scaleY Texture Y scale.
 * @param speed  Fade speed (1 is the normal speed, 2 is two time the normal speed, etc).
 */
void GRRLIB_DrawImg_FadeIn(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed)
{
    s16 alpha;
    f32 xpos = (rmode->fbWidth - tex->w) / 2;
    f32 ypos = (rmode->efbHeight - tex->h) / 2;

    for(alpha = 0; alpha < 255; alpha += speed) {
        if(alpha > 255) alpha = 255;
        GRRLIB_DrawImg(xpos, ypos, tex, 0, scaleX, scaleY, 0xFFFFFF00 | alpha);
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
void GRRLIB_DrawImg_FadeOut(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed)
{
    s16 alpha;
    f32 xpos = (rmode->fbWidth - tex->w) / 2;
    f32 ypos = (rmode->efbHeight - tex->h) / 2;

    for(alpha = 255; alpha > 0; alpha -= speed) {
        if(alpha < 0) alpha = 0;
        GRRLIB_DrawImg(xpos, ypos, tex, 0, scaleX, scaleY, 0xFFFFFF00 | alpha);
        GRRLIB_Render();
    }
}