/**
 * @file cursor.h
 * Contains the Cursor class.
 */

#ifndef CursorH
#define CursorH
//---------------------------------------------------------------------------

#include "object.h"
#include "grrlib_class.h"

/**
 * Types of cursor that could be used.
 */
enum class cursorType : u8 {
    O,   /**< O cursor. */
    X,   /**< X cursor. */
    P1,  /**< Player 1 cursor. */
    P2   /**< Player 2 cursor. */
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
    Texture *CursorImgX;
    Texture *CursorImgO;
    Texture *CursorMenu1;
    Texture *CursorMenu2;
    Texture *CurrentCursor;
};
//---------------------------------------------------------------------------
#endif
