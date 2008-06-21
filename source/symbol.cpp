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
}

/**
 * Destructor for the Symbol class.
 */
Symbol::~Symbol()
{
}

/**
 * Draw a symbol to screen.
 */
void Symbol::Paint()
{
	if(Player == 'X' || Player == 'x')
		GRRLIB_DrawImg(Left, Top, x_width, x_high, x_img, 0, 1, 100);
	else if(Player == 'O' || Player == 'o')
		GRRLIB_DrawImg(Left, Top, o_width, o_high, o_img, 0, 1, 100);
}

/**
 * Set the current player to draw on the screen.
 * @param[in] Player Player sign, either X or O.
 */
void Symbol::SetPlayer(unsigned char Player)
{
    this->Player = Player;
}
