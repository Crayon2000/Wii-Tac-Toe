//---------------------------------------------------------------------------
#ifndef ObjectH
#define ObjectH
//---------------------------------------------------------------------------

extern "C"
{
	#include "grrlib/GRRLIB.h"
}

/**
 * The Object class is a the basis for every thing you need to draw on the screen.
 * @author Crayon
 */
class Object
{
public:
	Object();
	~Object();
	unsigned int GetLeft();
	unsigned int GetTop();
	unsigned int GetWidth();
	unsigned int GetHeight();
	float GetAngle();
	bool IsVisible();
	void SetLeft(unsigned int);
	void SetTop(unsigned int);
	void SetWidth(unsigned int);
	void SetHeight(unsigned int);
	void SetSize(unsigned int, unsigned int);
	void SetAngle(float);
	void SetVisible(bool);
	bool IsInside(unsigned int, unsigned int);
protected:
	virtual void Paint();

	unsigned int Left;
	unsigned int Top;
	unsigned int Width;
	unsigned int Height;
	float Angle;
	bool Visible;
	//char *Name;
	//void *Parent;
};
//---------------------------------------------------------------------------
#endif
