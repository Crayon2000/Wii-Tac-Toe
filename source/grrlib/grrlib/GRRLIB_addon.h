/**
 * @file GRRLIB__addon.h
 * GRRLIB function needed for Wii-Tac-Toe.
 */

/**** FREETYPE START ****/
extern void GRRLIB_InitFreetype();
extern void GRRLIB_initTexture(void);
extern void GRRLIB_Printf2(int x, int y, const char *string, unsigned int fontSize, int color); 
GRRLIB_texImg *GRRLIB_GetTexture(void);
unsigned int GRRLIB_TextWidth(const char *, unsigned int);
/**** FREETYPE END ****/

bool GRRLIB_ScrShot(const char*);
GRRLIB_texImg *GRRLIB_Screen2Texture();

void GRRLIB_DrawImg_FadeInOut(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed);
void GRRLIB_DrawImg_FadeIn(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed);
void GRRLIB_DrawImg_FadeOut(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed);
