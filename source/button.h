//---------------------------------------------------------------------------
#ifndef ButtonH
#define ButtonH
//---------------------------------------------------------------------------

#include "object.h"

/**
 * Types of button that could be used.
 */
enum buttonType {
    btnHome,     /**< Home button with a small house. */
    btnHomeMenu, /**< Button to go in the HOME menu. A shadow is always present under the button. */
    btnStdMenu   /**< Button to go in a menu. Change color when the cursor over the button. */
};

/**
 * This a class to make a button on the screen with two states (selected or not).
 * @author Crayon
 */
class Button : public Object
{
public:
	Button();
	Button(buttonType);
	~Button();
	void Paint();
	void SetCaption(const char *);
	void SetSelected(bool);
	void SetTextColor(u32);
    void SetTextHeight(unsigned int);
private:
    void Init();
	bool Selected;
	char Caption[50 + 1];
	unsigned int TextWidth;
	unsigned int TextHeight;
	unsigned int TextTop;
	unsigned int TextLeft;
	u32 TextColor;
    buttonType Type;
	GRRLIB_texImg *ButtonImgOn;
	GRRLIB_texImg *ButtonImgOff;
};
//---------------------------------------------------------------------------
#endif
