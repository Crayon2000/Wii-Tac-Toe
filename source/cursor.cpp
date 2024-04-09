#include "cursor.h"

// Graphics
#include "hands_png.h"

/**
 * Constructor for the Cursor class.
 */
Cursor::Cursor() : Object()
{
    Width = 96;
    Height = 96;

    // Load textures
    Cursors = new Texture(hands_png, hands_png_size);
    GRRLIB_InitTileSet(reinterpret_cast<GRRLIB_texImg *>(Cursors), Width, Height, 0);

    // Default values
    SetPlayer(cursorType::X);
}

/**
 * Destructor for the Cursor class.
 */
Cursor::~Cursor()
{
    delete Cursors;
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
    if(Visible == true)
    {
        // Draw the shadow
        Cursors->DrawTile(Left + 3, Top + 3, Angle, 1, 1, 0x00000000 | ((A(Color) == 0xFF) ? 0x44 : 0x11), Frame);
        // Draw the cursor
        Cursors->DrawTile(Left, Top, Angle, 1, 1, Color, Frame);
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
        case cursorType::P1:
            Frame = 0;
            Cursors->SetOffset(48, 48);
            break;
        case cursorType::P2:
            Frame = 1;
            Cursors->SetOffset(48, 48);
            break;
        case cursorType::P3:
            Frame = 2;
            Cursors->SetOffset(48, 48);
            break;
        case cursorType::P4:
            Frame = 3;
            Cursors->SetOffset(48, 48);
            break;
        case cursorType::X:
            Frame = 4;
            Cursors->SetOffset(48, 45);
            break;
        case cursorType::O:
            Frame = 5;
            Cursors->SetOffset(48, 45);
            break;
        default:
            break;
    }
    Cursors->SetHandle(Cursors->GetOffsetX(), Cursors->GetOffsetY());
    return PreviousType;
}
