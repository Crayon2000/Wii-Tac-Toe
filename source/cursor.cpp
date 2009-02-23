#include <string.h>
#include <gccore.h>
#include "../gfx/hand_x.h"
#include "../gfx/hand_o.h"
#include "cursor.h"


#include <math.h>
#define PI 3.1415926
#define DEGTORAD(Deg) ((Deg * PI) / 180.0)
#define RADTODEG(Rad) ((180.0 * Rad) / PI

/**
 * Constructor for the Cursor class.
 */
Cursor::Cursor() : Object()
{
	Left = 0;
	Top = 0;
	Width = 40;
	Height = 56;

	CursorImgO = GRRLIB_LoadTexturePNG(hand_o);
	CursorImgX = GRRLIB_LoadTexturePNG(hand_x);
}

/**
 * Destructor for the Cursor class.
 */
Cursor::~Cursor()
{
	free(CursorImgX.data);
	free(CursorImgO.data);
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
    if(Visible)
    {
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
 * @param[in] Player Player sign, either X or O.
 */
void Cursor::SetPlayer(u8 Player)
{
	this->Player = Player;
}

/**
 * Get the left position corrected of the object.
 * @return Left position corrected in pixel.
 */
float Cursor::GetLeftCorrected()
{
	return Left*cos(Angle) - Top*sin(Angle);
}

/**
 * Get the top position corrected of the object.
 * @return Top position corrected in pixel.
 */
float Cursor::GetTopCorrected()
{
	return Left*sin(Angle) + Top*cos(Angle);
	//x' = cos( Angle ) * Left + sin( Angle ) * Top
	//y' = -sin( Angle ) * Left + cos( Angle ) * Top
	//float px = ir.x + 8.0 + sin(Angle) * 8.0 * 1.41;
    //float py = ir.y + 8.0 - cos(Angle) * 8.0 * 1.41;
    //NewX = Left*cos(Angle) - Top*sin(Angle)
    //NewY = Left*sin(Angle) + Top*cos(Angle)

}
