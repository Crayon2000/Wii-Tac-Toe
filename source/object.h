//---------------------------------------------------------------------------
#ifndef ObjectH
#define ObjectH
//---------------------------------------------------------------------------

#include <stdlib.h>
#include "grrlib/GRRLIB.h"

/**
 * The Object class is a the basis for every thing you need to draw on the screen.
 * @author Crayon
 */
class Object
{
public:
	Object();
	virtual ~Object();
	float GetLeft();
	float GetTop();
	unsigned int GetWidth();
	unsigned int GetHeight();
	float GetAngle();
    u8 GetAlpha();
	bool IsVisible();
	void SetLeft(float);
	void SetTop(float);
	void SetWidth(unsigned int);
	void SetHeight(unsigned int);
	void SetSize(unsigned int, unsigned int);
	void SetAngle(float);
    void SetAlpha(u8);
	void SetVisible(bool);
	bool IsInside(float, float);
protected:
	virtual void Paint();

	float Left;
	float Top;
	unsigned int Width;
	unsigned int Height;
	float Angle;
	bool Visible;
    u8 Alpha;
	//char *Name;
	//void *Parent;
};
//---------------------------------------------------------------------------
#endif
