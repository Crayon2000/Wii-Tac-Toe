#include "symbol.h"

// Fonts
#include "symbols_png.h"

/**
 * Constructor for the Symbol class.
 */
Symbol::Symbol() :
    Object()
{
    Width = 136;
    Height = 100;

    Img = Texture::CreateFromPNG(symbols_png);
    GRRLIB_InitTileSet(reinterpret_cast<GRRLIB_texImg *>(Img.get()), Width, Height, 0);
}

/**
 * Draw a symbol to screen.
 */
void Symbol::Paint()
{
    if(Frame < 0)
    {
        return;
    }
    Img->DrawTile(Left, Top, Angle, 1, 1, Color, Frame);
}

/**
 * Set the current player to draw on the screen.
 * @param[in] APlayer Player sign, either X or O.
 */
void Symbol::SetPlayer(u8 APlayer)
{
    if(APlayer == 'X' || APlayer == 'x')
    {
        Frame = 0;
    }
    else if(APlayer == 'O' || APlayer == 'o')
    {
        Frame = 1;
    }
    else
    {
        Frame = -1;
    }
}

/**
 * Set the left and top position of the object.
 * @param[in] APoint Position in pixel.
 */
void Symbol::SetLocation(Point APoint)
{
    Object::SetLocation(APoint.GetX(), APoint.GetY());
}
