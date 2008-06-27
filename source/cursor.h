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
private:
	u8 Player;
	u8 *CursorImgX;
	u8 *CursorImgO;
};
//---------------------------------------------------------------------------
#endif
