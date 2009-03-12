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
	Width = 40;
	Height = 56;
    Player = 'X';

	CursorImgO = GRRLIB_LoadTexture(hand_o);
	CursorImgX = GRRLIB_LoadTexture(hand_x);
    CursorShadow = GRRLIB_LoadTexture(hand_x);
}

/**
 * Destructor for the Cursor class.
 */
Cursor::~Cursor()
{
	free(CursorImgX.data);
	free(CursorImgO.data);
    free(CursorShadow.data);
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
    if(Visible)
    {
        GRRLIB_DrawImg(Left + 3, Top + 3, CursorShadow, Angle, 1, 1, 0x00000044);
        if(Player == 'O')
        {
            GRRLIB_DrawImg(Left, Top, CursorImgO, Angle, 1, 1, 0xFFFFFFFF);
        }
        else
        {
            GRRLIB_DrawImg(Left, Top, CursorImgX, Angle, 1, 1, 0xFFFFFFFF);
        }
    }
}

/**
 * Set player.
 * @param[in] PlayerToSet Player sign, either X or O.
 */
void Cursor::SetPlayer(u8 PlayerToSet)
{
	Player = PlayerToSet;
}

