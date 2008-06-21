#include <string.h>
#include <gccore.h>
#include "../gfx/button_on.h"
#include "../gfx/button_off.h"
#include "../fonts/button_text.h"
#include "button.h"

/**
 * Constructor for the Button class.
 */
Button::Button() : Object()
{
	Selected = false;
	Left = 0;
	Top = 0;
	Width = button_on_width;
	Height = button_on_high;
	TextHeight = button_text_high / 256;
	TextColorOn = 0x212C;
	TextColorOff = BLACK;
}

/**
 * Destructor for the Button class.
 */
Button::~Button()
{
}

/**
 * Draw the button to screen.
 */
void Button::Paint()
{
	if(Selected)
	{
		GRRLIB_DrawImg(Left, Top, Width, Height, button_on_img, 0, 1, 100);
		GRRLIB_Print(TextLeft, TextTop, button_text_width, TextHeight, Caption,
			button_text_img, TextColorOn, TRANSPARENT);
	}
	else
	{
		GRRLIB_DrawImg(Left, Top, Width, Height, button_off_img, 0, 1, 100);
		GRRLIB_Print(TextLeft, TextTop, button_text_width, TextHeight, Caption,
			button_text_img, TextColorOff, TRANSPARENT);
	}
}

/**
 * Set caption on the button.
 * @param[in] Caption Text to put on the button.
 */
void Button::SetCaption(const char *Caption)
{
	strncpy(this->Caption, Caption, 50);
	TextWidth = button_text_width * strlen(this->Caption);
	TextTop = Top + (Height / 2) - (TextHeight / 2);
	TextLeft = Left + (Width / 2) - (TextWidth / 2);
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
 * @param[in] TextColorOn Color of the text when the button is selected.
 * @param[in] TextColorOff Color of the text when the button is not selected.
 */
void Button::SetTextColor(u16 TextColorOn, u16 TextColorOff)
{
	this->TextColorOn = TextColorOn;
	this->TextColorOff = TextColorOff;
}
