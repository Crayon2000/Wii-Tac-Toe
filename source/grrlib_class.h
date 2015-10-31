/**
 * @file grrlib_class.h
 * Contains the GRRLIB wrapper class.
 */

#ifndef GRRLIB_WrapperH
#define GRRLIB_WrapperH
//---------------------------------------------------------------------------

#include <GRRLIB.h>
#include <string>

/**
 * Namespace containing all GRRLIB code.
 * @author Crayon
 */
namespace GRRLIBpp
{

/**
 * This class is a wrapper for GRRLIB_texImg.
 * @author Crayon
 */
class Texture : protected GRRLIB_texImg
{
public:
    Texture();
    Texture(const u8 *Buffer, const u32 Size = 0);
    Texture(const char *filename);
    Texture(const std::string &filename);
    Texture(const u32 w, const u32 h);
    void Initialize(void);
    ~Texture();

    u32 GetWidth();
    u32 GetHeight();
    u32 GetOffsetX();
    u32 GetOffsetY();
    void SetOffset(u32 X, u32 Y);
    void SetOffsetX(u32 X);
    void SetOffsetY(u32 Y);
    u32 GetHandleX();
    u32 GetHandleY();
    void SetHandle(u32 X, u32 Y);
    u32 GetPixel(const s32 x, const s32 y);
    void SetPixel(const s32 x, const s32 y, const u32 color);
    void Refresh();
    void Load(const u8 *Buffer, const u32 Size = 0);
    void Load(const char *filename);
    void Load(const std::string &filename);
    void Create(const u32 w, const u32 h, const u32 Color = 0x00000000);
    void Draw(const f32 xpos, const f32 ypos, const f32 degrees,
              const f32 scaleX, const f32 scaleY, const u32 color);
    void Draw(const f32 xpos, const f32 ypos, const f32 degrees,
              const f32 scaleX, const f32 scaleY);
    void Draw(const f32 xpos, const f32 ypos, const f32 degrees);
    void Draw(const f32 xpos, const f32 ypos);
    void CopyScreen(s32 posx = 0, s32 posy = 0, bool clear = false);
    void SetColor(u32);
    u32 GetColor(void);
    void SetAlpha(u8);
    u8 GetAlpha(void);
private:
    void Assign(GRRLIB_texImg *other);
    u32 _Color;  /**< The color used to draw the texture. By default it is set to 0xFFFFFFFF. */
    f32 _ScaleX; /**< The X scale used to draw the texture. By default it is set to 1.0. */
    f32 _ScaleY; /**< The Y scale used to draw the texture. By default it is set to 1.0. */
    f32 _Angle;  /**< The angle used to draw the texture. By default it is set to 0. */
};

/**
 * Namespace containing functions related to the screen.
 * @author Crayon
 */
namespace Screen
{
    s8 Initialize(void);
    void Exit(void);
    void Render(void);

    void SetBackgroundColor(u8 r, u8 g, u8 b, u8 a = 0xFF);
    void SetBackgroundColor(const u32 color);
    void FillScreen(const u32 color);
    void SetPixel(const f32 x, const f32 y, const u32 color);
    void Line(const f32 x1, const f32 y1, const f32 x2, const f32 y2, const u32 color);
    void Rectangle(const f32 x, const f32 y, const f32 width, const f32 height, const u32 color, const u8 filled);
    void Circle(const f32 x,  const f32 y,  const f32 radius, const u32 color, const u8 filled);

    u32 GetWidth(void);
    u32 GetHeight(void);

    bool ScreenShot(const char* filename);
    bool ScreenShot(const std::string &filename);
}   /* namespace Screen */
using namespace Screen;

/**
 * Namespace containing FX functions.
 * @author Crayon
 */
namespace FX
{
    void FlipH(const Texture *texsrc, Texture *texdest);
    void FlipV(const Texture *texsrc, Texture *texdest);
    void Grayscale(const Texture *texsrc, Texture *texdest);
    void Sepia(const Texture *texsrc, Texture *texdest);
    void Invert(const Texture *texsrc, Texture *texdest);
    void Blur(const Texture *texsrc, Texture *texdest, const u32 factor);
    void Scatter(const Texture *texsrc, Texture *texdest, const u32 factor);
    void Pixelate(const Texture *texsrc, Texture *texdest, const u32 factor);
}   /* namespace FX */
using namespace FX;

}   /* namespace GRRLIBpp */
using namespace GRRLIBpp;
//---------------------------------------------------------------------------
#endif
