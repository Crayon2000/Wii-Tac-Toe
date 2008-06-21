//---------------------------------------------------------------------------
#ifndef ButtonH
#define ButtonH
//---------------------------------------------------------------------------

#include "object.h"

/**
 * This a class to make a button on the screen with two states (selected or not).
 * @author Crayon
 */
class Button : public Object
{
public:
	Button();
	~Button();
	void Paint();
	void SetCaption(const char *);
	void SetSelected(bool);
	void SetTextColor(u16, u16);
private:
	bool Selected;
	char Caption[50 + 1];
	unsigned int TextWidth;
	unsigned int TextHeight;
	unsigned int TextTop;
	unsigned int TextLeft;
	u16 TextColorOn;
	u16 TextColorOff;
};
//---------------------------------------------------------------------------
#endif
