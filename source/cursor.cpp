#include "../gfx/hand_x.h"
#include "../gfx/hand_o.h"
#include "../gfx/player1_point.h"
#include "../gfx/player2_point.h"
#include "cursor.h"

/**
 * Constructor for the Cursor class.
 */
Cursor::Cursor() : Object()
{
    // Load textures
    CursorImgO = GRRLIB_LoadTexture(hand_o);
    CursorImgX = GRRLIB_LoadTexture(hand_x);
    CursorMenu1 = GRRLIB_LoadTexture(player1_point);
    CursorMenu2 = GRRLIB_LoadTexture(player2_point);

    // Set hotspot
    CursorImgO->offsetx = 48;
    CursorImgO->offsety = 45;
    GRRLIB_SetHandle(CursorImgO, CursorImgO->offsetx, CursorImgO->offsety);
    CursorImgX->offsetx = 48;
    CursorImgX->offsety = 45;
    GRRLIB_SetHandle(CursorImgX, CursorImgX->offsetx, CursorImgX->offsety);
    CursorMenu1->offsetx = 48;
    CursorMenu1->offsety = 48;
    GRRLIB_SetHandle(CursorMenu1, CursorMenu1->offsetx, CursorMenu1->offsety);
    CursorMenu2->offsetx = 48;
    CursorMenu2->offsety = 48;
    GRRLIB_SetHandle(CursorMenu2, CursorMenu2->offsetx, CursorMenu2->offsety);

    // Default values
    Left = 0;
    Top = 0;
    SetPlayer(curX);
}

/**
 * Destructor for the Cursor class.
 */
Cursor::~Cursor()
{
    GRRLIB_FreeTexture(CursorImgX);
    GRRLIB_FreeTexture(CursorImgO);
    GRRLIB_FreeTexture(CursorMenu1);
    GRRLIB_FreeTexture(CursorMenu2);
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
    if(Visible)
    {
        // Draw the shadow
        GRRLIB_DrawImg(Left + 3, Top + 3, CurrentCursor, Angle, 1, 1, 0x00000000 | ((A(Color) == 0xFF) ? 0x44 : 0x11));
        // Draw the cursor
        GRRLIB_DrawImg(Left, Top, CurrentCursor, Angle, 1, 1, Color);
    }
}

/**
 * Set cursor.
 * @param[in] NewCType New cursor type to use.
 * @return Previous cursor.
 */
cursorType Cursor::SetPlayer(cursorType NewCType)
{
    cursorType PreviousType = Type;
    Type = NewCType;
    switch(Type)
    {
        case curO:
            CurrentCursor = CursorImgO;
            break;
        case curX:
            CurrentCursor = CursorImgX;
            break;
        case curP1:
            CurrentCursor = CursorMenu1;
            break;
        case curP2:
            CurrentCursor = CursorMenu2;
            break;
    }
    Width = CurrentCursor->w;
    Height = CurrentCursor->h;
    return PreviousType;
}

