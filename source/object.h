//---------------------------------------------------------------------------
#ifndef ObjectH
#define ObjectH
//---------------------------------------------------------------------------

#include "grrlib/GRRLIB.h"

/**
 * The Object class is a the basis for every thing you need to draw on the screen.
 * @author Crayon
 */
class Object
{
public:
	Object();
	~Object();
	float GetLeft();
	float GetTop();
	unsigned int GetWidth();
	unsigned int GetHeight();
	float GetAngle();
	bool IsVisible();
	void SetLeft(float);
	void SetTop(float);
	void SetWidth(unsigned int);
	void SetHeight(unsigned int);
	void SetSize(unsigned int, unsigned int);
	void SetAngle(float);
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
	//char *Name;
	//void *Parent;
};
//---------------------------------------------------------------------------
#endif
