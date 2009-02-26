//---------------------------------------------------------------------------
#ifndef CursorH
#define CursorH
//---------------------------------------------------------------------------

#include "object.h"

/**
 * This class is used to draw a cursor on the screen.
 * @author Crayon
 */
class Cursor : public Object
{
public:
	Cursor();
	~Cursor();
	void Paint();
	void SetPlayer(u8);
	float GetLeftCorrected();
	float GetTopCorrected();
private:
	u8 Player;
	GRRLIB_texImg CursorImgX;
	GRRLIB_texImg CursorImgO;
    GRRLIB_texImg CursorShadow;
};
//---------------------------------------------------------------------------
#endif
