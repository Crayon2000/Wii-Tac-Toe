/**
 * @file cursor.h
 * Contains the Cursor class.
 */

#ifndef CursorH
#define CursorH
//---------------------------------------------------------------------------

#include <memory>
#include "object.h"
#include "grrlib_class.h"

/**
 * Types of cursor that could be used.
 * Item order must match cursor in hands.png image.
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
    ~Cursor() = default;
    Cursor& operator=(Cursor const&) = delete;
    void Paint() override;
    void SetPlayer(cursorType NewCType);
private:
    int Frame;
    std::unique_ptr<Texture> Cursors;
};
//---------------------------------------------------------------------------
#endif
