#include <gccore.h>
#include "object.h"
extern "C"
{
	#include "grrlib/GRRLIB.h"
}

/**
 * Constructor for the Object class.
 */
Object::Object()
{
	Left = 0;
	Top = 0;
}

/**
 * Destructor for the Object class.
 */
Object::~Object()
{
}

/**
 * Get the left position of the object.
 * @see SetLeft() 
 * @return Left position in pixel.
 */
unsigned int Object::GetLeft()
{
	return Left;
}

/**
 * Set the left position of the object.
 * @param[in] Left Left in pixel.
 * @see GetLeft()
 */
void Object::SetLeft(unsigned int Left)
{
	this->Left = Left;
}

/**
 * Get the top position of the object.
 * @see SetTop()
 * @return Top position in pixel.
 */
unsigned int Object::GetTop()
{
	return Top;
}

/**
 * Set the top position of the object.
 * @param[in] Top Top in pixel.
 * @see GetTop()
 */
void Object::SetTop(unsigned int Top)
{
	this->Top = Top;
}

/**
 * Get the width of the object.
 * @see SetWidth()
 * @return Width of the object.
 */
unsigned int Object::GetWidth()
{
	return Width;
}

/**
 * Set the width of the object.
 * @param[in] Width Width in pixel.
 * @see GetWidth()
 */
void Object::SetWidth(unsigned int Width)
{
	this->Width = Width;
}

/**
 * Get the height of the object.
 * @see SetHeight()
 * @return Height of the object.
 */
unsigned int Object::GetHeight()
{
	return Height;
}

/**
 * Set the height of the object.
 * @param[in] Height Height in pixel.
 * @see GetHeight()
 */
void Object::SetHeight(unsigned int Height)
{
	this->Height = Height;
}

/**
 * Set the width and height of the object.
 * @param[in] Width Width in pixel.
 * @param[in] Height Height in pixel.
 * @see SetWidth()
 * @see SetHeight()
 */
void Object::SetSize(unsigned int Width, unsigned int Height)
{
	this->Width = Width;
	this->Height = Height;
}

/**
 * Get visible.
 * @see SetVisible()
 * @return true if visible, false otherwise.
 */
bool Object::IsVisible()
{
	return Visible;
}

/**
 * Set visible.
 * @param[in] Visible Set to true to show object.
 * @see IsVisible()
 */
void Object::SetVisible(bool Visible)
{
	this->Visible = Visible;
}

/**
 * Draw an image on screen.
 */
void Object::Paint()
{
}
