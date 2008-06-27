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
    this->Player = 'X';

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
	free(ImgO);
	free(ImgX);
}

/**
 * Draw a symbol to screen.
 */
void Symbol::Paint()
{
	if(Player == 'X' || Player == 'x')
	{
		GRRLIB_DrawImg(Left, Top, Width, Height, ImgX, 0, 1, 1, 255);
	}
	else if(Player == 'O' || Player == 'o')
	{
		GRRLIB_DrawImg(Left, Top, Width, Height, ImgO, 0, 1, 1, 255);
	}
}

/**
 * Set the current player to draw on the screen.
 * @param[in] Player Player sign, either X or O.
 */
void Symbol::SetPlayer(unsigned char Player)
{
    this->Player = Player;
}
