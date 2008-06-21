#include <string.h>
#include <gccore.h>
#include "../gfx/hand_x.h"
#include "../gfx/hand_o.h"
#include "cursor.h"

/**
 * Constructor for the Cursor class.
 */
Cursor::Cursor() : Object()
{
	Left = 0;
	Top = 0;
	Width = hand_x_width;
	Height = hand_x_high;
}

/**
 * Destructor for the Cursor class.
 */
Cursor::~Cursor()
{
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
	if(Player == 'O')
		GRRLIB_DrawImg(Left, Top, Width, Height, hand_o_img, 0, 1, 100);
	else
		GRRLIB_DrawImg(Left, Top, Width, Height, hand_x_img, 0, 1, 100);
}

/**
 * Set player.
 * @param[in] Player Player sign, either X or O.
 */
void Cursor::SetPlayer(u8 Player)
{
	this->Player = Player;
}
