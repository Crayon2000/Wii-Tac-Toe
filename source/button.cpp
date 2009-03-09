#include <string.h>
#include <gccore.h>
#include "../gfx/button_on.h"
#include "../gfx/button_off.h"
#include "../gfx/home_button.h"
#include "../gfx/button_home.h"
#include "button.h"

/**
 * Constructor for the Button class.
 */
Button::Button() : Object()
{
    this->Type = btnStdMenu;
    Init();
}

/**
 * Constructor for the Button class.
  * @param[in] Type Button type.
 */
Button::Button(buttonType Type) : Object()
{
    this->Type = Type;
    Init();
}

/**
 * Destructor for the Button class.
 */
Button::~Button()
{
    if(ButtonImgOn.data)
        free(ButtonImgOn.data);
    if(ButtonImgOff.data)
        free(ButtonImgOff.data);
}

/**
 * Initialize the Button class.
 */
void Button::Init()
{
    switch(Type)
    {
        case btnHomeMenu:
            ButtonImgOn.data = NULL;
            ButtonImgOff = GRRLIB_LoadTexture(button_home);
            break;
        case btnHome:
            ButtonImgOn.data = NULL;
            ButtonImgOff = GRRLIB_LoadTexture(home_button);
            break;
        default:
            ButtonImgOn = GRRLIB_LoadTexture(button_on);
            ButtonImgOff = GRRLIB_LoadTexture(button_off);
    }

	Selected = false;

	Left = 0;
	Top = 0;
	Width = ButtonImgOff.w;
	Height = ButtonImgOff.h;

	TextColor = 0x000000;
	TextHeight = 14;
	TextWidth = 100; // random value
}

/**
 * Draw the button to screen.
 */
void Button::Paint()
{
    if(Type == btnHomeMenu)
    {   // Draw shadow
        GRRLIB_DrawImg(Left + 4, Top + 5, ButtonImgOff, 0, 1.0, 1.0, 0x00000055);
    }
    GRRLIB_DrawImg(Left, Top, ButtonImgOff, 0, 1.0, 1.0, 0xFFFFFFFF);
	GRRLIB_Printf2(TextLeft, TextTop, Caption, TextHeight, TextColor);
	if(Type == btnStdMenu && Selected)
	{   // Hover color
		GRRLIB_DrawImg(Left, Top, ButtonImgOn, 0, 1.0, 1.0, 0xFFFFFFFF);
	}
    else if(Type == btnHomeMenu && Selected)
    {   // Hover color
        GRRLIB_DrawImg(Left, Top, ButtonImgOff, 0, 1.0, 1.0, 0x0000FF33);
    }
}

/**
 * Set text height of the button.
 * @param[in] Text height of the button.
 */
void Button::SetTextHeight(unsigned int TextHeight)
{
    this->TextHeight = TextHeight;
}

/**
 * Set caption on the button.
 * @param[in] Caption Text to put on the button.
 */
void Button::SetCaption(const char *Caption)
{
	strncpy(this->Caption, Caption, 50);
	TextWidth = GRRLIB_TextWidth(this->Caption, TextHeight);
	TextTop = Top + (Height / 2) - (TextHeight / 2);
	TextLeft = Left + (Width / 2) - (TextWidth / 2);
    if(Type == btnHome)
    {
        TextLeft += 20;
    }
}

/**
 * Set the button state, selected or not.
 * @param[in] Selected Set to true to select the button, false otherwise.
 */
void Button::SetSelected(bool Selected)
{
	this->Selected = Selected;
}

/**
 * Set the text color on the button.
 * @param[in] TextColor Color of the text when the button is not selected.
 */
void Button::SetTextColor(u32 TextColor)
{
	this->TextColor = TextColor;
}
