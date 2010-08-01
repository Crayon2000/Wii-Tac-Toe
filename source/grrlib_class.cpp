#include "grrlib_class.h"

/**
 * Constructor for the Texture class.
 */
Texture::Texture()
{
    Initialize();
}

/**
 * Constructor for the Texture class.
 * @param Buffer The buffer to load.
 * @param Size The size of the buffer to load. This parameter is not required.
 * @see Load(const u8 *, const u32)
 */
Texture::Texture(const u8 *Buffer, const u32 Size)
{
    Initialize();
    Load(Buffer, Size);
}

/**
 * Constructor for the Texture class.
 * @param filename The JPEG, PNG or Bitmap file to load.
 * @see Load(const char *)
 */
Texture::Texture(const char *filename)
{
    Initialize();
    Load(filename);
}

/**
 * Constructor for the Texture class.
 * @param filename The JPEG, PNG or Bitmap file to load.
 * @see Load(const std::string &)
 */
Texture::Texture(const std::string &filename)
{
    Initialize();
    Load(filename);
}

/**
 * Constructor for the Texture class. Create an empty texture.
 * @param w Width of the new texture to create.
 * @param h Height of the new texture to create.
 * @see Create()
 */
Texture::Texture(const u32 w, const u32 h)
{
    Initialize();
    Create(w, h);
}

/**
 * Initialize a constructor with default values.
 */
void Texture::Initialize(void)
{
    _TextStruct = NULL;
    _Color = 0xFFFFFFFF;
    _ScaleX = 1.0;
    _ScaleY = 1.0;
    _Angle = 0.0;
}

/**
 * Destructor for the Texture class.
 */
Texture::~Texture()
{
    if(_TextStruct)
    {
        GRRLIB_FreeTexture(_TextStruct);
    }
}

/**
 * Load a texture from a buffer.
 * @param Buffer The buffer to load.
 * @param Size The size of the buffer to load. This parameter is not required.
 */
void Texture::Load(const u8 *Buffer, const u32 Size)
{
    if(_TextStruct)
    {   // Delete texture if already filled
        GRRLIB_FreeTexture(_TextStruct);
    }
    if(Buffer[0]==0xFF && Buffer[1]==0xD8 && Buffer[2]==0xFF)
    {   // JPEG image
        if(Size)
        {
            _TextStruct = GRRLIB_LoadTextureJPGEx(Buffer, Size);
        }
        else
        {
            _TextStruct = GRRLIB_LoadTextureJPG(Buffer);
        }
    }
    else if(Buffer[0]=='B' && Buffer[1]=='M')
    {   // Bitmap image
        _TextStruct = GRRLIB_LoadTextureBMP(Buffer);
    }
    else
    {   // PNG image
        _TextStruct = GRRLIB_LoadTexturePNG(Buffer);
    }
}

/**
 * Load a texture from a file.
 * @param filename The JPEG, PNG or Bitmap file to load.
 */
void Texture::Load(const char *filename)
{
    u8 *data;

    int FileLength = GRRLIB_LoadFile(filename, &data);
    if(FileLength <= 0)
    {   // Loading the file failed
        return;
    }

    Load(data, FileLength);

    // Free up the buffer
    free(data);
}

/**
 * Load a texture from a file.
 * @param filename The JPEG, PNG or Bitmap file to load.
 */
void Texture::Load(const std::string &filename)
{
    Load(filename.c_str());
}

/**
 * Create an empty texture.
 * @param w Width of the new texture to create.
 * @param h Height of the new texture to create.
 * @param Color A color in RGBA format to set the empty texture. The default color is transparent;
 */
void Texture::Create(const u32 w, const u32 h, const u32 Color)
{
    if(_TextStruct)
    {   // Delete texture if already filled
        GRRLIB_FreeTexture(_TextStruct);
    }

    // Code taken from GRRLIB_CreateEmptyTexture
    _TextStruct = (struct GRRLIB_texImg *)calloc(1, sizeof(GRRLIB_texImg));
    if(_TextStruct != NULL)
    {
        _TextStruct->data = memalign(32, h * w * 4);
        _TextStruct->w = w;
        _TextStruct->h = h;

        // Initialize the texture with a color
        u32 x, y, offs;
        u8*  bp = (u8*)_TextStruct->data;
        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {   // Code taken from GRRLIB_SetPixelTotexImg
                offs = (((y&(~3))<<2)*w) + ((x&(~3))<<4) + ((((y&3)<<2) + (x&3)) <<1);
                *((u16*)(bp+offs   )) = (u16)((Color <<8) | (Color >>24));
                *((u16*)(bp+offs+32)) = (u16) (Color >>8);
            }
        }

        GRRLIB_SetHandle(_TextStruct, 0, 0);
        GRRLIB_FlushTex(_TextStruct);
    }
}

/**
 * Return the width of the texture in pixels.
 * @return The width in pixels.
 */
u32 Texture::GetWidth()
{
    if(_TextStruct)
    {
        return _TextStruct->w;
    }
    return 0;
}

/**
 * Return the height of the texture in pixels.
 * @return The height in pixels.
 */
u32 Texture::GetHeight()
{
    if(_TextStruct)
    {
        return _TextStruct->h;
    }
    return 0;
}

/**
 * Return x texture offset.
 * @return The x texture offset.
 */
u32 Texture::GetOffsetX()
{
    if(_TextStruct)
    {
        return _TextStruct->offsetx;
    }
    return 0;
}

/**
 * Return y texture offset.
 * @return The y texture offset.
 */
u32 Texture::GetOffsetY()
{
    if(_TextStruct)
    {
        return _TextStruct->offsety;
    }
    return 0;
}

/**
 * Set the offset.
 * @param X The x texture offset.
 * @param Y The y texture offset.
 */
void Texture::SetOffset(u32 X, u32 Y)
{
    if(_TextStruct)
    {
        _TextStruct->offsetx = X;
        _TextStruct->offsety = Y;
    }
}

/**
 * Set the x offset.
 * @param X The x texture offset.
 */
void Texture::SetOffsetX(u32 X)
{
    if(_TextStruct)
    {
        _TextStruct->offsetx = X;
    }
}

/**
 * Set the y offset.
 * @param Y The y texture offset.
 */
void Texture::SetOffsetY(u32 Y)
{
    if(_TextStruct)
    {
        _TextStruct->offsety = Y;
    }
}

/**
 * Return x texture handle.
 * @return The x texture handle.
 */
u32 Texture::GetHandleX()
{
    if(_TextStruct)
    {
        return _TextStruct->handlex;
    }
    return 0;
}

/**
 * Return y texture handle.
 * @return The y texture handle.
 */
u32 Texture::GetHandleY()
{
    if(_TextStruct)
    {
        return _TextStruct->handley;
    }
    return 0;
}

/**
 * Set a texture's X and Y handles.
 * For example, it could be used for the rotation of a texture.
 * @param X The x-coordinate of the handle.
 * @param Y The y-coordinate of the handle.
 */
void Texture::SetHandle(u32 X, u32 Y)
{
    if(_TextStruct)
    {
        GRRLIB_SetHandle(_TextStruct, X, Y);
    }
}

/**
 * Return the color value of a pixel from the texture.
 * @param x Specifies the x-coordinate of the pixel in the texture.
 * @param y Specifies the y-coordinate of the pixel in the texture.
 * @return The color of a pixel in RGBA format.
 */
u32 Texture::GetPixel(const s32 x, const s32 y)
{
    return GRRLIB_GetPixelFromtexImg(x, y, _TextStruct);
}

/**
 * Set the color value of a pixel to a texture.
 * @see Refresh
 * @param x Specifies the x-coordinate of the pixel in the texture.
 * @param y Specifies the y-coordinate of the pixel in the texture.
 * @param color The color of the pixel in RGBA format.
 */
void Texture::SetPixel(const s32 x, const s32 y, const u32 color)
{
    if(_TextStruct)
    {
        GRRLIB_SetPixelTotexImg(x, y, _TextStruct, color);
    }
}

/**
 * Write the contents of a texture in the data cache down to main memory.
 * For performance the CPU holds a data cache where modifications are stored before they get written down to main memory.
 */
void Texture::Refresh()
{
    if(_TextStruct)
    {
        DCFlushRange(_TextStruct->data, _TextStruct->w * _TextStruct->h * 4);
    }
}

/**
 * Draw the texture.
 * @param xpos Specifies the x-coordinate of the upper-left corner.
 * @param ypos Specifies the y-coordinate of the upper-left corner.
 * @param degrees Angle of rotation.
 * @param scaleX Specifies the x-coordinate scale. -1 could be used for flipping the texture horizontally.
 * @param scaleY Specifies the y-coordinate scale. -1 could be used for flipping the texture vertically.
 * @param color Color in RGBA format.
 */
void Texture::Draw(const f32 xpos, const f32 ypos, const f32 degrees,
                   const f32 scaleX, const f32 scaleY, const u32 color)
{
    if(_TextStruct)
    {
        GRRLIB_DrawImg(xpos, ypos, _TextStruct, degrees, scaleX, scaleY, color);
    }
}

/**
 * Draw the texture with the default color.
 * @param xpos Specifies the x-coordinate of the upper-left corner.
 * @param ypos Specifies the y-coordinate of the upper-left corner.
 * @param degrees Angle of rotation.
 * @param scaleX Specifies the x-coordinate scale. -1 could be used for flipping the texture horizontally.
 * @param scaleY Specifies the y-coordinate scale. -1 could be used for flipping the texture vertically.
 * @see SetColor
 */
void Texture::Draw(const f32 xpos, const f32 ypos, const f32 degrees,
                   const f32 scaleX, const f32 scaleY)
{
    Draw(xpos, ypos, degrees, scaleX, scaleY, _Color);
}

/**
 * Draw the texture with the default color and the scale set to 1.
 * @param xpos Specifies the x-coordinate of the upper-left corner.
 * @param ypos Specifies the y-coordinate of the upper-left corner.
 * @param degrees Angle of rotation.
 * @see SetColor
 */
void Texture::Draw(const f32 xpos, const f32 ypos, const f32 degrees)
{
    Draw(xpos, ypos, degrees, _ScaleX, _ScaleY, _Color);
}

/**
 * Draw the texture with the default color, the scale set to 1 and no rotation.
 * @param xpos Specifies the x-coordinate of the upper-left corner.
 * @param ypos Specifies the y-coordinate of the upper-left corner.
 * @see SetColor
 */
void Texture::Draw(const f32 xpos, const f32 ypos)
{
    Draw(xpos, ypos, _Angle, _ScaleX, _ScaleY, _Color);
}

/**
 * Make a snapshot of the screen in a texture WITHOUT ALPHA LAYER.
 * @param posx top left corner of the grabbed part. Default is 0.
 * @param posy top left corner of the grabbed part. Default is 0.
 * @param clear When this flag is set to true, the screen is cleared after copy. Set to false by default.
 */
void Texture::CopyScreen(s32 posx, s32 posy, bool clear)
{
    if(_TextStruct)
    {
        GRRLIB_Screen2Texture(posx, posy, _TextStruct, clear);
    }
}

/**
 * Set the color of the texture.
 * @param Color New color in RGBA format.
 */
void Texture::SetColor(u32 Color)
{
   _Color = Color;
}

/**
 * Get the color of the texture.
 * @return The color of the texture in RGBA format.
 */
u32 Texture::GetColor(void)
{
    return _Color;
}

/**
 * Set the alpha channel of the texture without changing the color.
 * @param Alpha The alpha color.
 */
void Texture::SetAlpha(u8 Alpha)
{
    _Color = (_Color & 0xFFFFFF00) | (Alpha & 0xFF);
}

/**
 * Get the alpha of the texture.
 * @return Alpha color (0-255).
 */
u8 Texture::GetAlpha(void)
{
    return A(_Color);
}



/**
 * Clear screen with a specific color.
 * @param color The color to use to fill the screen.
 */
void Screen::FillScreen(const u32 color)
{
    GRRLIB_FillScreen(color);
}

/**
 * Draw a dot.
 * @param x Specifies the x-coordinate of the dot.
 * @param y Specifies the y-coordinate of the dot.
 * @param color The color of the dot in RGBA format.
 */
void Screen::SetPixel(const f32 x, const f32 y, const u32 color)
{
    GRRLIB_Plot(x, y, color);
}

/**
 * Draw a line.
 * @param x1 Starting point for line for the x coordinate.
 * @param y1 Starting point for line for the y coordinate.
 * @param x2 Ending point for line for the x coordinate.
 * @param y2 Ending point for line for the x coordinate.
 * @param color Line color in RGBA format.
 */
void Screen::Line(const f32 x1, const f32 y1, const f32 x2, const f32 y2, const u32 color)
{
    GRRLIB_Line(x1, y1, x2, y2, color);
}

/**
 * Draw a rectangle.
 * @param x Specifies the x-coordinate of the upper-left corner of the rectangle.
 * @param y Specifies the y-coordinate of the upper-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle in RGBA format.
 * @param filled Set to true to fill the rectangle.
 */
void Screen::Rectangle(const f32 x, const f32 y, const f32 width, const f32 height, const u32 color, const u8 filled)
{
    GRRLIB_Rectangle(x, y, width,  height, color, filled);
}

/**
 * Draw a circle.
 * @param x Specifies the x-coordinate of the circle.
 * @param y Specifies the y-coordinate of the circle.
 * @param radius The radius of the circle.
 * @param color The color of the circle in RGBA format.
 * @param filled Set to true to fill the circle.
 */
void Screen::Circle(const f32 x, const f32 y, const f32 radius, const u32 color, const u8 filled)
{
    GRRLIB_Circle(x, y, radius, color, filled);
}

/**
 * Make a PNG screenshot.
 * It should be called after drawing stuff on the screen, but before GRRLIB_Render.
 * libfat is required to use the function.
 * @param filename Name of the file to write.
 * @return bool true=everything worked, false=problems occurred.
 */
bool Screen::ScreenShot(const char* filename)
{
    return GRRLIB_ScrShot(filename);
}

/**
 * Make a PNG screenshot.
 * It should be called after drawing stuff on the screen, but before GRRLIB_Render.
 * libfat is required to use the function.
 * @param filename Name of the file to write.
 * @return bool true=everything worked, false=problems occurred.
 */
bool Screen::ScreenShot(const std::string &filename)
{
    return GRRLIB_ScrShot(filename.c_str());
}

/**
 * Return the width of the screen in pixels.
 * @return The width in pixels.
 */
u32 Screen::GetWidth(void)
{
    return rmode->fbWidth;
}

/**
 * Return the height of the screen in pixels.
 * @return The height in pixels.
 */
u32 Screen::GetHeight(void)
{
    return rmode->efbHeight;
}