#include "grrlib_class.h"

/**
 * Constructor for the Texture class.
 */
Texture::Texture() :
    _Color(0xFFFFFFFF),
    _ScaleX(1.0f),
    _ScaleY(1.0f),
    _Angle(0.0f)
{
    data = nullptr;
}

/**
 * Constructor for the Texture class.
 * @param Buffer The buffer to load.
 * @param Size The size of the buffer to load. This parameter is not required.
 * @see Load(const u8 *, const u32)
 */
Texture::Texture(const u8 *Buffer, const u32 Size) : Texture()
{
    Load(Buffer, Size);
}

/**
 * Constructor for the Texture class.
 * @param filename The JPEG, PNG or Bitmap file to load.
 * @see Load(const char *)
 */
Texture::Texture(const char *filename) : Texture()
{
    Load(filename);
}

/**
 * Constructor for the Texture class.
 * @param filename The JPEG, PNG or Bitmap file to load.
 * @see Load(const std::string &)
 */
Texture::Texture(const std::string &filename) : Texture()
{
    Load(filename);
}

/**
 * Constructor for the Texture class. Create an empty texture.
 * @param w Width of the new texture to create.
 * @param h Height of the new texture to create.
 * @see Create()
 */
Texture::Texture(const u32 w, const u32 h) : Texture()
{
    Create(w, h);
}

/**
 * Destructor for the Texture class.
 */
Texture::~Texture()
{
    free(data);
}

/**
 * Assign a GRRLIB texture to this object.
 * The GRRLIB texture will be destroy.
 */
void Texture::Assign(GRRLIB_texImg *other)
{
    if(other == nullptr)
    {
        return;
    }

    w = other->w;
    h = other->h;
    format = other->format;
    handlex = other->handlex;
    handley = other->handley;
    offsetx = other->offsetx;
    offsety = other->offsety;

    tiledtex = other->tiledtex;
    tilew = other->tilew;
    tileh = other->tileh;
    nbtilew = other->nbtilew;
    nbtileh = other->nbtileh;
    tilestart = other->tilestart;
    ofnormaltexx = other->ofnormaltexx;
    ofnormaltexy = other->ofnormaltexy;

    free(data);
    data = other->data;

    free(other);
}

/**
 * Load a texture from a buffer.
 * @param Buffer The buffer to load.
 * @param Size The size of the buffer to load. This parameter is not required.
 */
void Texture::Load(const u8 *Buffer, const u32 Size)
{
    if(Buffer[0]==0x00 && Buffer[1]==0x20 && Buffer[2]==0xAF && Buffer[3]==0x30)
    {   // TPL file
        //Assign(GRRLIB_LoadTextureTPL(Buffer, 0));
    }
    else if(Buffer[0]==0xFF && Buffer[1]==0xD8 && Buffer[2]==0xFF)
    {   // JPEG image
        if(Size > 0)
        {
            Assign(GRRLIB_LoadTextureJPGEx(Buffer, Size));
        }
        else
        {
            Assign(GRRLIB_LoadTextureJPG(Buffer));
        }
    }
    else if(Buffer[0]=='B' && Buffer[1]=='M')
    {   // Bitmap image
        Assign(GRRLIB_LoadTextureBMP(Buffer));
    }
    else
    {   // PNG image
        Assign(GRRLIB_LoadTexturePNG(Buffer));
    }
}

/**
 * Create a texture from a png file buffer.
 * @param Buffer The buffer to load.
 */
std::unique_ptr<Texture> Texture::CreateFromPNG(const u8 *Buffer)
{
    auto texture = std::make_unique<Texture>();
    texture->Assign(GRRLIB_LoadTexturePNG(Buffer));
    return texture;
}

/**
 * Load a texture from a file.
 * @param filename The JPEG, PNG or Bitmap file to load.
 */
void Texture::Load(const char *filename)
{
    u8 *mydata;

    const int FileLength = GRRLIB_LoadFile(filename, &mydata);
    if(FileLength <= 0)
    {   // Loading the file failed
        return;
    }

    Load(mydata, FileLength);

    // Free up the buffer
    free(mydata);
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
 * @param Color A color in RGBA format to set the empty texture. The default color is transparent.
 */
void Texture::Create(const u32 w, const u32 h, const u32 Color)
{
    // Delete texture if already filled
    free(data);

    data = memalign(32, h * w * 4);
    this->w = w;
    this->h = h;
    format  = GX_TF_RGBA8;
    handlex = 0;
    handley = 0;
    offsetx = 0;
    offsety = 0;

    tiledtex = 0;
    tilew = 0;
    tileh = 0;
    nbtilew = 0;
    nbtileh = 0;
    tilestart = 0;
    ofnormaltexx = 0.0f;
    ofnormaltexy = 0.0f;

    // Initialize the texture with a color
    u8* bp = static_cast<u8*>(data);
    for(u32 y = 0; y < h; ++y)
    {
        for(u32 x = 0; x < w; ++x)
        {   // Code taken from GRRLIB_SetPixelTotexImg
            u32 offs = (((y&(~3))<<2)*w) + ((x&(~3))<<4) + ((((y&3)<<2) + (x&3)) <<1);
            *((u16*)(bp+offs   )) = (u16)((Color <<8) | (Color >>24));
            *((u16*)(bp+offs+32)) = (u16) (Color >>8);
        }
    }

    GRRLIB_SetHandle(this, 0, 0);
    GRRLIB_FlushTex(this);
}

/**
 * Return the width of the texture in pixels.
 * @return The width in pixels.
 */
u32 Texture::GetWidth()
{
    return w;
}

/**
 * Return the height of the texture in pixels.
 * @return The height in pixels.
 */
u32 Texture::GetHeight()
{
    return h;
}

/**
 * Return the format of the texture.
 * @return The texture format.
 */
u32 Texture::GetFormat()
{
    return format;
}

/**
 * Return x texture offset.
 * @return The x texture offset.
 */
u32 Texture::GetOffsetX()
{
    return offsetx;
}

/**
 * Return y texture offset.
 * @return The y texture offset.
 */
u32 Texture::GetOffsetY()
{
    return offsety;
}

/**
 * Set the offset.
 * @param X The x texture offset.
 * @param Y The y texture offset.
 */
void Texture::SetOffset(u32 X, u32 Y)
{
    offsetx = X;
    offsety = Y;
}

/**
 * Set the x offset.
 * @param X The x texture offset.
 */
void Texture::SetOffsetX(u32 X)
{
    offsetx = X;
}

/**
 * Set the y offset.
 * @param Y The y texture offset.
 */
void Texture::SetOffsetY(u32 Y)
{
    offsety = Y;
}

/**
 * Return x texture handle.
 * @return The x texture handle.
 */
u32 Texture::GetHandleX()
{
    return handlex;
}

/**
 * Return y texture handle.
 * @return The y texture handle.
 */
u32 Texture::GetHandleY()
{
    return handley;
}

/**
 * Set a texture's X and Y handles.
 * For example, it could be used for the rotation of a texture.
 * @param X The x-coordinate of the handle.
 * @param Y The y-coordinate of the handle.
 */
void Texture::SetHandle(u32 X, u32 Y)
{
    GRRLIB_SetHandle(this, X, Y);
}

/**
 * Return the color value of a pixel from the texture.
 * @param x Specifies the x-coordinate of the pixel in the texture.
 * @param y Specifies the y-coordinate of the pixel in the texture.
 * @return The color of a pixel in RGBA format.
 */
u32 Texture::GetPixel(const s32 x, const s32 y)
{
    return GRRLIB_GetPixelFromtexImg(x, y, this);
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
    GRRLIB_SetPixelTotexImg(x, y, this, color);
}

/**
 * Write the contents of a texture in the data cache down to main memory.
 * For performance the CPU holds a data cache where modifications are stored before they get written down to main memory.
 */
void Texture::Refresh()
{
    DCFlushRange(data, w * h * 4);
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
    GRRLIB_DrawImg(xpos, ypos, this, degrees, scaleX, scaleY, color);
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
 * Draw a tile.
 * @param xpos Specifies the x-coordinate of the upper-left corner.
 * @param ypos Specifies the y-coordinate of the upper-left corner.
 * @param degrees Angle of rotation.
 * @param scaleX Specifies the x-coordinate scale. -1 could be used for flipping the texture horizontally.
 * @param scaleY Specifies the y-coordinate scale. -1 could be used for flipping the texture vertically.
 * @param color Color in RGBA format.
 * @param frame Specifies the frame to draw.
 */
void Texture::DrawTile(const f32 xpos, const f32 ypos, const f32 degrees,
                   const f32 scaleX, const f32 scaleY, const u32 color, int frame)
{
    GRRLIB_DrawTile(xpos, ypos, this, degrees, scaleX, scaleY, color, frame);
}

/**
 * Make a snapshot of the screen in a texture WITHOUT ALPHA LAYER.
 * @param posx top left corner of the grabbed part. Default is 0.
 * @param posy top left corner of the grabbed part. Default is 0.
 * @param clear When this flag is set to true, the screen is cleared after copy. Set to false by default.
 */
void Texture::CopyScreen(u16 posx, u16 posy, bool clear)
{
    GRRLIB_Screen2Texture(posx, posy, this, clear);
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
u32 Texture::GetColor()
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
u8 Texture::GetAlpha()
{
    return A(_Color);
}



/**
 * Initialize video library. Call this once at the beginning your code.
 * @return A integer representing a code:
 *         -     0 : The operation completed successfully.
 *         -    -1 : Not enough memory is available to initialize video library.
 *         -    -2 : Failed to add the fat device driver to the devoptab.
 *         -    -3 : Failed to initialize the font engine.
 * @see Exit
 */
s32 Screen::Initialize()
{
    return GRRLIB_Init();
}

/**
 * Call this before exiting your application.
 * Ensure this function is only ever called once
 * and only if the setup function has been called.
 */
void Screen::Exit()
{
    GRRLIB_Exit();
}

/**
 * Set the background parameter when screen is cleared.
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 */
void Screen::SetBackgroundColor(u8 r, u8 g, u8 b, u8 a)
{
    GRRLIB_SetBackgroundColour(r, g, b, a);
}

/**
 * Set the background parameter when screen is cleared.
 * @param color The color to use to set the background.
 */
void Screen::SetBackgroundColor(const u32 color)
{
    GRRLIB_SetBackgroundColour(R(color), G(color), B(color), A(color));
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
 * Call this function after drawing.
 */
void Screen::Render()
{
    GRRLIB_Render();
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
 * @param filled Set to @c true to fill the rectangle.
 */
void Screen::Rectangle(const f32 x, const f32 y, const f32 width, const f32 height, const u32 color, const bool filled)
{
    GRRLIB_Rectangle(x, y, width,  height, color, filled);
}

/**
 * Draw a circle.
 * @param x Specifies the x-coordinate of the circle.
 * @param y Specifies the y-coordinate of the circle.
 * @param radius The radius of the circle.
 * @param color The color of the circle in RGBA format.
 * @param filled Set to @c true to fill the circle.
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
u16 Screen::GetWidth()
{
    return rmode->fbWidth;
}

/**
 * Return the height of the screen in pixels.
 * @return The height in pixels.
 */
u16 Screen::GetHeight()
{
    return rmode->efbHeight;
}



/**
 * Flip texture horizontal.
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 */
void FX::FlipH(const Texture *texsrc, Texture *texdest)
{
    GRRLIB_BMFX_FlipH(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest));
}
/**
 * Flip texture vertical.
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 */
void FX::FlipV(const Texture *texsrc, Texture *texdest)
{
    GRRLIB_BMFX_FlipV(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest));
}
/**
 * Change a texture to gray scale.
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture grayscaled destination.
 */
void FX::Grayscale(const Texture *texsrc, Texture *texdest)
{
    GRRLIB_BMFX_Grayscale(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest));
}
/**
 * Change a texture to sepia (old photo style).
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 */
void FX::Sepia(const Texture *texsrc, Texture *texdest)
{
    GRRLIB_BMFX_Sepia(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest));
}
/**
 * Invert colors of the texture.
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 */
void FX::Invert(const Texture *texsrc, Texture *texdest)
{
    GRRLIB_BMFX_Invert(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest));
}
/**
 * A texture effect (Blur).
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 * @param factor The blur factor.
 */
void FX::Blur(const Texture *texsrc, Texture *texdest, const u32 factor)
{
    GRRLIB_BMFX_Blur(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest), factor);
}
/**
 * A texture effect (Scatter).
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 * @param factor The factor level of the effect.
 */
void FX::Scatter(const Texture *texsrc, Texture *texdest, const u32 factor)
{
    GRRLIB_BMFX_Scatter(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest), factor);
}
/**
 * A texture effect (Pixelate).
 * @see Texture::Refresh()
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 * @param factor The factor level of the effect.
 */
void FX::Pixelate(const Texture *texsrc, Texture *texdest, const u32 factor)
{
    GRRLIB_BMFX_Pixelate(reinterpret_cast<const GRRLIB_texImg *>(texsrc), reinterpret_cast<GRRLIB_texImg *>(texdest), factor);
}
