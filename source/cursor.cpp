#include "cursor.h"

// Graphics
#include "hand_x_png.h"
#include "hand_o_png.h"
#include "player1_point_png.h"
#include "player2_point_png.h"

/**
 * Constructor for the Cursor class.
 */
Cursor::Cursor() : Object()
{
    // Load textures
    CursorImgO = new Texture(hand_o_png, hand_o_png_size);
    CursorImgX = new Texture(hand_x_png, hand_x_png_size);
    CursorMenu1 = new Texture(player1_point_png, player1_point_png_size);
    CursorMenu2 = new Texture(player2_point_png, player2_point_png_size);

    // Set hotspot
    CursorImgO->SetOffset(48, 45);
    CursorImgO->SetHandle(CursorImgO->GetOffsetX(), CursorImgO->GetOffsetY());
    CursorImgX->SetOffset(48, 45);
    CursorImgX->SetHandle(CursorImgX->GetOffsetX(), CursorImgX->GetOffsetY());
    CursorMenu1->SetOffset(48, 48);
    CursorMenu1->SetHandle(CursorMenu1->GetOffsetX(), CursorMenu1->GetOffsetY());
    CursorMenu2->SetOffset(48, 48);
    CursorMenu2->SetHandle(CursorMenu2->GetOffsetX(), CursorMenu2->GetOffsetY());

    // Default values
    SetPlayer(cursorType::X);
}

/**
 * Destructor for the Cursor class.
 */
Cursor::~Cursor()
{
    delete CursorImgX;
    delete CursorImgO;
    delete CursorMenu1;
    delete CursorMenu2;
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
    if(Visible == true)
    {
        // Draw the shadow
        CurrentCursor->Draw(Left + 3, Top + 3, Angle, 1, 1, 0x00000000 | ((A(Color) == 0xFF) ? 0x44 : 0x11));
        // Draw the cursor
        CurrentCursor->Draw(Left, Top, Angle, 1, 1, Color);
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
        case cursorType::O:
            CurrentCursor = CursorImgO;
            break;
        case cursorType::X:
            CurrentCursor = CursorImgX;
            break;
        case cursorType::P1:
            CurrentCursor = CursorMenu1;
            break;
        case cursorType::P2:
            CurrentCursor = CursorMenu2;
            break;
    }
    Width = CurrentCursor->GetWidth();
    Height = CurrentCursor->GetHeight();
    return PreviousType;
}

