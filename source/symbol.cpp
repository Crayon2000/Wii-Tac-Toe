#include "symbol.h"

// Fonts
#include "x_png.h"
#include "o_png.h"

/**
 * Constructor for the Symbol class.
 */
Symbol::Symbol() :
    Object(),
    Current(NULL)
{
    ImgO = new Texture(o_png);
    ImgX = new Texture(x_png);

    Width = 136;
    Height = 100;
}

/**
 * Destructor for the Symbol class.
 */
Symbol::~Symbol()
{
    delete ImgO;
    delete ImgX;
}

/**
 * Draw a symbol to screen.
 */
void Symbol::Paint()
{
    if(Current)
    {
        Current->Draw(Left, Top, Angle, 1, 1, Color);
    }
}

/**
 * Set the current player to draw on the screen.
 * @param[in] APlayer Player sign, either X or O.
 */
void Symbol::SetPlayer(u8 APlayer)
{
    if(APlayer == 'X' || APlayer == 'x')
    {
        Current = ImgX;
    }
    else if(APlayer == 'O' || APlayer == 'o')
    {
        Current = ImgO;
    }
    else
    {
        Current = NULL;
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
