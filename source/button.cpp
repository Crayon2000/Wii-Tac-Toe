#include <string.h>
#include "button.h"

// Graphics
#include "button_on_png.h"
#include "button_off_png.h"
#include "home_button_png.h"
#include "button_home_png.h"

/**
 * Constructor for the Button class.
 * @param[in] NewType Button type.
 */
Button::Button(buttonType NewType) : Object(),
    Selected(false),
    Caption(""),
    Font(NULL),
    TextColor(0x000000),
    Type(NewType)
{
    switch(Type)
    {
        case buttonType::HomeMenu:
            ButtonImgOn = NULL;
            ButtonImgOff = new Texture(button_home_png, button_home_png_size);
            break;
        case buttonType::Home:
            ButtonImgOn = NULL;
            ButtonImgOff = new Texture(home_button_png, home_button_png_size);
            break;
        default:
            ButtonImgOn = new Texture(button_on_png, button_on_png_size);
            ButtonImgOff = new Texture(button_off_png, button_off_png_size);
    }

    Width = ButtonImgOff->GetWidth();
    Height = ButtonImgOff->GetHeight();

    TextHeight = 14;
    TextWidth = 100; // random value
}

/**
 * Destructor for the Button class.
 */
Button::~Button()
{
    delete ButtonImgOn;
    delete ButtonImgOff;
}

/**
 * Draw the button to screen.
 */
void Button::Paint()
{
    if(Type == buttonType::HomeMenu)
    {   // Draw shadow
        ButtonImgOff->Draw(Left + 4, Top + 5, 0, 1.0, 1.0, 0x00000055);
    }
    ButtonImgOff->Draw(Left, Top, 0, 1.0, 1.0, 0xFFFFFFFF);
    GRRLIB_PrintfTTF(TextLeft, TextTop, Font, Caption.c_str(), TextHeight, TextColor);
    if(Type == buttonType::StdMenu && Selected == true)
    {   // Hover color
        ButtonImgOn->Draw(Left, Top, 0, 1.0, 1.0, 0xFFFFFFFF);
    }
    else if(Type == buttonType::HomeMenu && Selected == true)
    {   // Hover color
        ButtonImgOff->Draw(Left, Top, 0, 1.0, 1.0, 0x0000FF33);
    }
}

/**
 * Set text height of the button.
 * @param[in] NewHeight height of the button.
 */
void Button::SetTextHeight(unsigned int NewHeight)
{
    TextHeight = NewHeight;
}

/**
 * Set caption on the button.
 * @param[in] NewCaption Text to put on the button.
 */
void Button::SetCaption(const std::string &NewCaption)
{
    Caption = NewCaption;
    TextWidth = GRRLIB_WidthTTF(Font, Caption.c_str(), TextHeight);
    TextTop = Top + (Height / 2) - (TextHeight / 2);
    TextLeft = Left + (Width / 2) - (TextWidth / 2);
    if(Type == buttonType::Home)
    {
        TextLeft += 20;
    }
}

/**
 * Set the font to use for the text on the button.
 * @param[in] AFont Font to use for the text on the button.
 */
void Button::SetFont(GRRLIB_ttfFont *AFont)
{
    Font = AFont;
}

/**
 * Set the button state, selected or not.
 * @param[in] IsSelected Set to true to select the button, false otherwise.
 */
void Button::SetSelected(bool IsSelected)
{
    Selected = IsSelected;
}

/**
 * Set the text color on the button.
 * @param[in] NewColor Color of the text when the button is not selected.
 */
void Button::SetTextColor(u32 NewColor)
{
    TextColor = NewColor;
}
