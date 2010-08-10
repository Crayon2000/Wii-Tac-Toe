#include <string.h>
#include "../gfx/button_on.h"
#include "../gfx/button_off.h"
#include "../gfx/home_button.h"
#include "../gfx/button_home.h"
#include "button.h"

/**
 * Constructor for the Button class.
  * @param[in] NewType Button type.
 */
Button::Button(buttonType NewType) : Object(),
    Selected(false),
    Caption(L""),
    Font(NULL)
{
    Type = NewType;

    switch(Type)
    {
        case btnHomeMenu:
            ButtonImgOn = NULL;
            ButtonImgOff = new Texture(button_home);
            break;
        case btnHome:
            ButtonImgOn = NULL;
            ButtonImgOff = new Texture(home_button);
            break;
        default:
            ButtonImgOn = new Texture(button_on);
            ButtonImgOff = new Texture(button_off);
    }

    Width = ButtonImgOff->GetWidth();
    Height = ButtonImgOff->GetHeight();

    TextColor = 0x000000;
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
    if(Type == btnHomeMenu)
    {   // Draw shadow
        ButtonImgOff->Draw(Left + 4, Top + 5, 0, 1.0, 1.0, 0x00000055);
    }
    ButtonImgOff->Draw(Left, Top, 0, 1.0, 1.0, 0xFFFFFFFF);
    GRRLIB_PrintfTTFW(TextLeft, TextTop, Font, Caption.c_str(), TextHeight, TextColor);
    if(Type == btnStdMenu && Selected)
    {   // Hover color
        ButtonImgOn->Draw(Left, Top, 0, 1.0, 1.0, 0xFFFFFFFF);
    }
    else if(Type == btnHomeMenu && Selected)
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
void Button::SetCaption(const char *NewCaption)
{
    size_t length = strlen(NewCaption);
    wchar_t *utf32 = (wchar_t*)malloc(length * sizeof(wchar_t));
    mbstowcs(utf32, NewCaption, length);
    SetCaption(utf32);
}

/**
 * Set caption on the button.
 * @param[in] NewCaption Text to put on the button.
 */
void Button::SetCaption(const wstring &NewCaption)
{
    Caption = NewCaption;
    TextWidth = GRRLIB_WidthTTFW(Font, Caption.c_str(), TextHeight);
    TextTop = Top + (Height / 2) - (TextHeight / 2);
    TextLeft = Left + (Width / 2) - (TextWidth / 2);
    if(Type == btnHome)
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
