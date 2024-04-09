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
    P1,  /**< Player 1 cursor. */
    P2,  /**< Player 2 cursor. */
    P3,  /**< Player 3 cursor. */
    P4,  /**< Player 4 cursor. */
    X,   /**< X cursor. */
    O    /**< O cursor. */
};

/**
 * This class is used to draw a cursor on the screen.
 * @author Crayon
 */
class Cursor : public Object
{
public:
    Cursor();
    Cursor(Cursor const&) = delete;
    ~Cursor();
    Cursor& operator=(Cursor const&) = delete;
    void Paint() override;
    cursorType SetPlayer(cursorType NewCType);
private:
    cursorType Type;
    int Frame;
    Texture *Cursors;
};
//---------------------------------------------------------------------------
#endif
