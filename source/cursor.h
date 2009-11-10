/**
 * @file cursor.h
 * Contains the Cursor class.
 */

#ifndef CursorH
#define CursorH
//---------------------------------------------------------------------------

#include "object.h"

/**
 * Types of cursor that could be used.
 */
enum cursorType {
    curO,   /**< O cursor. */
    curX,   /**< X cursor. */
    curP1,  /**< Player 1 cursor. */
    curP2   /**< Player 2 cursor. */
};

/**
 * This class is used to draw a cursor on the screen.
 * @author Crayon
 */
class Cursor : public Object
{
public:
    Cursor();
    ~Cursor();
    void Paint();
    cursorType SetPlayer(cursorType);
private:
    cursorType Type;
    GRRLIB_texImg *CursorImgX;
    GRRLIB_texImg *CursorImgO;
    GRRLIB_texImg *CursorMenu1;
    GRRLIB_texImg *CursorMenu2;
    GRRLIB_texImg *CurrentCursor;
};
//---------------------------------------------------------------------------
#endif
