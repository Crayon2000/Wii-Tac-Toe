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
	Width = 44;
	Height = 60;
	
	CursorImgO = GRRLIB_LoadTexture(hand_o);
	CursorImgX = GRRLIB_LoadTexture(hand_x);
}

/**
 * Destructor for the Cursor class.
 */
Cursor::~Cursor()
{
	free(CursorImgX);
	free(CursorImgO);
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
	if(Player == 'O')
	{
		GRRLIB_DrawImg(Left, Top, Width, Height, CursorImgO, 0, 1, 1, 255);
	}
	else
	{
		GRRLIB_DrawImg(Left, Top, Width, Height, CursorImgX, 0, 1, 1, 255);
	}
}

/**
 * Set player.
 * @param[in] Player Player sign, either X or O.
 */
void Cursor::SetPlayer(u8 Player)
{
	this->Player = Player;
}
