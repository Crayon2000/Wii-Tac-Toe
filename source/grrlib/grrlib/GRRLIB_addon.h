/**
 * @file GRRLIB__addon.h
 * GRRLIB function needed for Wii-Tac-Toe.
 */

/**** FREETYPE START ****/
extern void GRRLIB_InitFreetype();
extern void GRRLIB_ExitFreetype();
extern void GRRLIB_initTexture(void);
extern void GRRLIB_Printf2(int x, int y, const char *string, unsigned int fontSize, int color);
extern void GRRLIB_Printf2W(int x, int y, const wchar_t *string, unsigned int fontSize, int color);
GRRLIB_texImg *GRRLIB_GetTexture(void);
unsigned int GRRLIB_TextWidth(const char *, unsigned int);
unsigned int GRRLIB_TextWidthW(const wchar_t *, unsigned int);
/**** FREETYPE END ****/

void GRRLIB_DrawImg_FadeInOut(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed);
void GRRLIB_DrawImg_FadeIn(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed);
void GRRLIB_DrawImg_FadeOut(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed);
