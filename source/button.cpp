#include <string>
#include "button.h"

// Graphics
#include "button_on_png.h"
#include "button_off_png.h"
#include "home_button_png.h"
#include "button_home_png.h"
#include "button_select_png.h"

/**
 * Constructor for the Button class.
 * @param[in] NewType Button type.
 */
Button::Button(buttonType NewType) : Object(),
    Focused(false),
    Selected(false),
    Caption(""),
    Font(nullptr),
    TextWidth(100), // random value
    TextHeight(14),
    TextTop(0),
    TextLeft(0),
    TextColor(0x000000FF),
    Type(NewType)
{
    switch(Type)
    {
        case buttonType::HomeMenu:
            ButtonImgOff = Texture::CreateFromPNG(button_home_png);
            break;
        case buttonType::Home:
            ButtonImgOff = Texture::CreateFromPNG(home_button_png);
            break;
        default:
            ButtonImgOn = Texture::CreateFromPNG(button_on_png);
            ButtonImgOff = Texture::CreateFromPNG(button_off_png);
            ButtonSelected = Texture::CreateFromPNG(button_select_png);
    }

    Width = ButtonImgOff->GetWidth();
    Height = ButtonImgOff->GetHeight();
}

/**
 * Draw the button to screen.
 */
void Button::Paint()
{
    if(Type == buttonType::HomeMenu)
    {   // Draw shadow
        ButtonImgOff->Draw(Left + 4.0f, Top + 5.0f, 0, 1.0f, 1.0f, 0x00000055);
    }
    ButtonImgOff->Draw(Left, Top, 0, 1.0f, 1.0f, 0xFFFFFFFF);
    GRRLIB_PrintfTTF(TextLeft, TextTop, Font, Caption.c_str(), TextHeight, TextColor);
    if(Focused == true && Type == buttonType::StdMenu)
    {   // Hover color
        ButtonImgOn->Draw(Left, Top, 0, 1.0f, 1.0f, 0xFFFFFFFF);
    }
    else if(Focused == true && Type == buttonType::HomeMenu)
    {   // Hover color
        ButtonImgOff->Draw(Left, Top, 0, 1.0f, 1.0f, 0x0000FF33);
    }
    if(Selected == true && ButtonSelected != nullptr)
    {   // Select button
        ButtonSelected->Draw(Left - 8.0f, Top - 6.0f);
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
 * Set the button state, focused or not.
 * @param[in] IsFocused Set to true to set focus to the button, false otherwise.
 */
void Button::SetFocused(bool IsFocused)
{
    Focused = IsFocused;
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
 * @param[in] NewColor Color of the text when the button is not focused.
 */
void Button::SetTextColor(u32 NewColor)
{
    TextColor = NewColor;
}
