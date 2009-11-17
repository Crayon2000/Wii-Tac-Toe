#include "grrlib.h"

#include <fat.h>
#include <wchar.h>

/**
 * Fade in, than fade out.
 * @param data   Texture.
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
 * @param data   Texture.
 * @param scaleX Texture X scale.
 * @param scaleY Texture Y scale.
 * @param speed  Fade speed (1 is the normal speed, 2 is two time the normal speed, etc).
 */
void GRRLIB_DrawImg_FadeIn(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed)
{
    int alpha;
    f32 xpos = (rmode->fbWidth - tex->w) / 2;
    f32 ypos = (rmode->efbHeight - tex->h) / 2;

    for(alpha = 0; alpha < 255; alpha += speed)
    {
        if(alpha > 255) alpha = 255;
        GRRLIB_DrawImg(xpos, ypos, tex, 0, scaleX, scaleY, 0xFFFFFF00 | alpha);
        GRRLIB_Render();
    }
}

/**
 * Fade out.
 * @param data   Texture.
 * @param scaleX Texture X scale.
 * @param scaleY Texture Y scale.
 * @param speed  Fade speed (1 is the normal speed, 2 is two time the normal speed, etc).
 */
void GRRLIB_DrawImg_FadeOut(struct GRRLIB_texImg *tex, float scaleX, f32 scaleY, u16 speed)
{
    int alpha;
    f32 xpos = (rmode->fbWidth - tex->w) / 2;
    f32 ypos = (rmode->efbHeight - tex->h) / 2;

    for(alpha = 255; alpha > 0; alpha -= speed)
    {
        if(alpha < 0) alpha = 0;
        GRRLIB_DrawImg(xpos, ypos, tex, 0, scaleX, scaleY, 0xFFFFFF00 | alpha);
        GRRLIB_Render();
    }
}

/******************************************************************************/
/**** FREETYPE START ****/
/* This is a very rough implementation if freetype using GRRLIB */
#include "../fonts/Swis721_Ex_BT.h" // A truetype font
#include "../freetype/config/ftheader.h"
#include FT_FREETYPE_H

static FT_Library ftLibrary;
static FT_Face ftFace;

GRRLIB_texImg *fontTempLayer;
GRRLIB_texImg *fontTexture;

/* Static function prototypes */
static void BitmapTo4x4RGBA(const unsigned char *src, void *dst, const unsigned int width, const unsigned int height);
static bool BlitGlyph(FT_Bitmap *bitmap, int offset, int top, int color) ;

extern void GRRLIB_InitFreetype(void)
{
    if (FT_Init_FreeType(&ftLibrary) ||
        FT_New_Memory_Face(ftLibrary, Swis721_Ex_BT, Swis721_Ex_BT_size, 0, &ftFace))
    {
        exit(0);
    }

    fontTempLayer = (struct GRRLIB_texImg *)calloc(1, sizeof(GRRLIB_texImg));
    fontTempLayer->w = rmode->fbWidth;
    fontTempLayer->h = rmode->efbHeight;
    fontTempLayer->data = memalign(32, fontTempLayer->w * fontTempLayer->h * 4);
    fontTexture = (struct GRRLIB_texImg *)calloc(1, sizeof(GRRLIB_texImg));
    fontTexture->w = rmode->fbWidth;
    fontTexture->h = rmode->efbHeight;
    fontTexture->data = memalign(32, fontTexture->w * fontTexture->h * 4);
    if(fontTempLayer->data == NULL || fontTexture->data == NULL)
    {
        exit(0); /* Oops! Something went wrong! */
    }
}

extern void GRRLIB_initTexture(void)
{
    GRRLIB_ClearTex(fontTempLayer);
}

/**
 * Print function for TTF font.
 */
extern void GRRLIB_Printf2(int x, int y, const char *string, unsigned int fontSize, int color)
{
    unsigned int error = 0;
    int penX = 0;
    int penY = fontSize;
    FT_GlyphSlot slot = ftFace->glyph;
    FT_UInt glyphIndex = 0;
    FT_UInt previousGlyph = 0;
    FT_Bool hasKerning = FT_HAS_KERNING(ftFace);

    error = FT_Set_Pixel_Sizes(ftFace, 0, fontSize);
    if (error)
    {
        /* Failed to set the font size to the requested size.
         * You probably should set a default size or something.
         * I'll leave that up to the reader. */
         FT_Set_Pixel_Sizes(ftFace, 0, 12);
    }

    /* Convert the string to UTF32 */
    size_t length = strlen(string);
    wchar_t *utf32 = (wchar_t*)malloc(length * sizeof(wchar_t));
    length = mbstowcs(utf32, string, length);

    /* Loop over each character, drawing it on to the 4, until the
     * end of the string is reached, or until the pixel width is too wide */
    unsigned int loop;
    for (loop = 0; loop < length; ++loop)
    {
        glyphIndex = FT_Get_Char_Index(ftFace, utf32[ loop ]);

        /* To the best of my knowledge, none of the other freetype
         * implementations use kerning, so my method ends up looking
         * slightly better :) */
        if (hasKerning && previousGlyph && glyphIndex)
        {
            FT_Vector delta;
            FT_Get_Kerning(ftFace, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }

        error = FT_Load_Glyph(ftFace, glyphIndex, FT_LOAD_RENDER);
        if (error)
        {
            /* Whoops, something went wrong trying to load the glyph
             * for this character... you should handle this better */
            continue;
        }

        if (BlitGlyph(&slot->bitmap, penX + slot->bitmap_left+x, penY - slot->bitmap_top+y, color) == true)
        {
            /* The glyph was successfully blitted to the buffer, move the pen forwards */
            penX += slot->advance.x >> 6;
            previousGlyph = glyphIndex;
        }
        else
        {
            /* BlitGlyph returned false, the line must be full */
            free(utf32);
            return;
        }
    }

    free(utf32);
}

/**
 * Print function for TTF font.
 */
extern void GRRLIB_Printf2W(int x, int y, const wchar_t *utf32, unsigned int fontSize, int color)
{
    unsigned int error = 0;
    int penX = 0;
    int penY = fontSize;
    FT_GlyphSlot slot = ftFace->glyph;
    FT_UInt glyphIndex = 0;
    FT_UInt previousGlyph = 0;
    FT_Bool hasKerning = FT_HAS_KERNING(ftFace);

    error = FT_Set_Pixel_Sizes(ftFace, 0, fontSize);
    if (error)
    {
        /* Failed to set the font size to the requested size.
         * You probably should set a default size or something.
         * I'll leave that up to the reader. */
         FT_Set_Pixel_Sizes(ftFace, 0, 12);
    }

    size_t length = wcslen(utf32);

    /* Loop over each character, drawing it on to the 4, until the
     * end of the string is reached, or until the pixel width is too wide */
    unsigned int loop;
    for (loop = 0; loop < length; ++loop)
    {
        glyphIndex = FT_Get_Char_Index(ftFace, utf32[ loop ]);

        /* To the best of my knowledge, none of the other freetype
         * implementations use kerning, so my method ends up looking
         * slightly better :) */
        if (hasKerning && previousGlyph && glyphIndex)
        {
            FT_Vector delta;
            FT_Get_Kerning(ftFace, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }

        error = FT_Load_Glyph(ftFace, glyphIndex, FT_LOAD_RENDER);
        if (error)
        {
            /* Whoops, something went wrong trying to load the glyph
             * for this character... you should handle this better */
            continue;
        }

        if (BlitGlyph(&slot->bitmap, penX + slot->bitmap_left+x, penY - slot->bitmap_top+y, color) == true)
        {
            /* The glyph was successfully blitted to the buffer, move the pen forwards */
            penX += slot->advance.x >> 6;
            previousGlyph = glyphIndex;
        }
        else
        {
            /* BlitGlyph returned false, the line must be full */
            return;
        }
    }
}

/**
 * Returns true if the character was draw on to the buffer, false if otherwise.
 */
static bool BlitGlyph(FT_Bitmap *bitmap, int offset, int top, int color)
{
    int bitmapWidth = bitmap->width;
    int bitmapHeight = bitmap->rows;

    if (offset + bitmapWidth > 640)
    {
        /* Drawing this character would over run the buffer, so don't draw it */
        return false;
    }

    /* Draw the glyph onto the buffer, blitting from the bottom up */
    /* CREDIT: Derived from a function by DragonMinded */
    unsigned char *p = fontTempLayer->data;
    unsigned int y = 0;
    int sywidth, dywidth;
    unsigned int column;
    unsigned int dstloc;
    for (y = 0; y < bitmapHeight; ++y)
    {
        sywidth = y * bitmapWidth;
        dywidth = (y + top) * 640;

        for (column = 0; column < bitmapWidth; ++column)
        {
            dstloc = ((column + offset) + dywidth) << 2;

            /* Copy the alpha value for this pixel into the texture buffer */
            p[ dstloc + 2 ] = (color & 0xFF);
            p[ dstloc + 1 ] = ((color >> 8) & 0xFF);
            p[ dstloc + 0 ] = ((color >> 16) & 0xFF);
            p[ dstloc + 3 ] = (bitmap->buffer[ column + sywidth ]);
        }
    }

    return true;
}

/**
 * Render the text string to a 4x4RGBA texture, return a pointer to this texture.
 */
GRRLIB_texImg *GRRLIB_GetTexture(void)
{
    BitmapTo4x4RGBA(fontTempLayer->data, fontTexture->data, 640, 480);
    GRRLIB_FlushTex(fontTexture);

    return fontTexture;
}

/**
 * Convert the RGBA temp buffer to a format usuable by GX.
 */
static void BitmapTo4x4RGBA(const unsigned char *src, void *dst, const unsigned int width, const unsigned int height)
{
    unsigned int block, i, c, ar, gb;
    unsigned char *p = (unsigned char*)dst;

    for (block = 0; block < height; block += 4) {
        for (i = 0; i < width; i += 4) {
            /* Alpha and Red */
            for (c = 0; c < 4; ++c) {
                for (ar = 0; ar < 4; ++ar) {
                    /* Alpha pixels */
                    *p++ = src[(((i + ar) + ((block + c) * width)) * 4) + 3];
                    /* Red pixels */
                    *p++ = src[((i + ar) + ((block + c) * width)) * 4];
                }
            }

            /* Green and Blue */
            for (c = 0; c < 4; ++c) {
                for (gb = 0; gb < 4; ++gb) {
                    /* Green pixels */
                    *p++ = src[(((i + gb) + ((block + c) * width)) * 4) + 1];
                    /* Blue pixels */
                    *p++ = src[(((i + gb) + ((block + c) * width)) * 4) + 2];
                }
            }
        } /* i */
    } /* block */
}

/**
 * Get the width of a text in pixel.
 * @param string The text to check.
 * @param fontSize The size of the font.
 * @return The width of a text in pixel.
 */
unsigned int GRRLIB_TextWidth(const char *string, unsigned int fontSize) {
    int penX = 0;
    FT_UInt glyphIndex;
    FT_UInt previousGlyph = 0;
    FT_Bool hasKerning = FT_HAS_KERNING(ftFace);
    unsigned int loop;
    size_t length;
    wchar_t *utf32;

    if(string == NULL)
    {
        return 0;
    }

    if(FT_Set_Pixel_Sizes(ftFace, 0, fontSize))
    {
        /* Failed to set the font size to the requested size.
         * You probably should set a default size or something. */
         FT_Set_Pixel_Sizes(ftFace, 0, 12);
    }

    /* Convert the string to UTF32 */
    length = strlen(string);
    utf32 = (wchar_t *)malloc(strlen(string) * sizeof(wchar_t));
    length = mbstowcs(utf32, string, length);

    /* Loop over each character, drawing it on to the 4, until the
     * end of the string is reached, or until the pixel width is too wide */
    for(loop = 0; loop < length; ++loop)
    {
        glyphIndex = FT_Get_Char_Index(ftFace, utf32[ loop ]);

        /* To the best of my knowledge, none of the other freetype
         * implementations use kerning, so my method ends up looking
         * slightly better :) */
        if(hasKerning && previousGlyph && glyphIndex)
        {
            FT_Vector delta;
            FT_Get_Kerning(ftFace, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }

        if(FT_Load_Glyph(ftFace, glyphIndex, FT_LOAD_RENDER))
        {
            continue;
        }

        penX += ftFace->glyph->advance.x >> 6;
        previousGlyph = glyphIndex;
    }

    free(utf32);
    return penX;
}

/**
 * Get the width of a text in pixel.
 * @param string The text to check.
 * @param fontSize The size of the font.
 * @return The width of a text in pixel.
 */
unsigned int GRRLIB_TextWidthW(const wchar_t *utf32, unsigned int fontSize) {
    int penX = 0;
    FT_UInt glyphIndex;
    FT_UInt previousGlyph = 0;
    FT_Bool hasKerning = FT_HAS_KERNING(ftFace);
    unsigned int loop;
    size_t length;

    if(utf32 == NULL)
    {
        return 0;
    }

    if(FT_Set_Pixel_Sizes(ftFace, 0, fontSize))
    {
        /* Failed to set the font size to the requested size.
         * You probably should set a default size or something. */
         FT_Set_Pixel_Sizes(ftFace, 0, 12);
    }

    length = wcslen(utf32);

    /* Loop over each character, drawing it on to the 4, until the
     * end of the string is reached, or until the pixel width is too wide */
    for(loop = 0; loop < length; ++loop)
    {
        glyphIndex = FT_Get_Char_Index(ftFace, utf32[ loop ]);

        /* To the best of my knowledge, none of the other freetype
         * implementations use kerning, so my method ends up looking
         * slightly better :) */
        if(hasKerning && previousGlyph && glyphIndex)
        {
            FT_Vector delta;
            FT_Get_Kerning(ftFace, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }

        if(FT_Load_Glyph(ftFace, glyphIndex, FT_LOAD_RENDER))
        {
            continue;
        }

        penX += ftFace->glyph->advance.x >> 6;
        previousGlyph = glyphIndex;
    }

    return penX;
}

/**** FREETYPE END ****/
/******************************************************************************/
