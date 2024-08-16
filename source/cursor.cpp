#include "cursor.h"

#include <utility>

// Graphics
#include "hands_png.h"

/**
 * Constructor for the Cursor class.
 */
Cursor::Cursor() : Object(),
    Cursors(std::make_unique<Texture>(hands_png, hands_png_size))
{
    Width = 96;
    Height = 96;

    // Load textures
    GRRLIB_InitTileSet(reinterpret_cast<GRRLIB_texImg *>(Cursors.get()), Width, Height, 0);

    // Set hotspot
    Cursors->SetOffset(48, 45);
    Cursors->SetHandle(Cursors->GetOffsetX(), Cursors->GetOffsetY());

    // Default values
    SetPlayer(cursorType::X);
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
 */
void Cursor::SetPlayer(cursorType NewCType)
{
    Frame = std::to_underlying(NewCType);
}
