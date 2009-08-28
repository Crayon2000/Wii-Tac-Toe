#include <gccore.h>
#include "symbol.h"

// Fonts
#include "../gfx/x.h"
#include "../gfx/o.h"

/**
 * Constructor for the Symbol class.
 */
Symbol::Symbol() : Object()
{
    Player = ' ';

    ImgO = GRRLIB_LoadTexture(o);
    ImgX = GRRLIB_LoadTexture(x);

    Width = 136;
    Height = 100;
}

/**
 * Destructor for the Symbol class.
 */
Symbol::~Symbol()
{
    GRRLIB_FreeTexture(ImgO);
    GRRLIB_FreeTexture(ImgX);
}

/**
 * Draw a symbol to screen.
 */
void Symbol::Paint()
{
    if(Player == 'X' || Player == 'x')
    {
        GRRLIB_DrawImg(Left, Top, ImgX, Angle, 1, 1, Color);
    }
    else if(Player == 'O' || Player == 'o')
    {
        GRRLIB_DrawImg(Left, Top, ImgO, Angle, 1, 1, Color);
    }
}

/**
 * Set the current player to draw on the screen.
 * @param[in] APlayer Player sign, either X or O.
 */
void Symbol::SetPlayer(unsigned char APlayer)
{
    Player = APlayer;
}

/**
 * Set the left and top position of the object.
 * @param[in] APoint Position in pixel.
 */
void Symbol::SetLocation(Point APoint)
{
    Object::SetLocation(APoint.GetX(), APoint.GetY());
}
