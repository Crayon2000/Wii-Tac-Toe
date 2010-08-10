#include "object.h"

/**
 * Constructor for the Object class.
 */
Object::Object() :
    Left(0),
    Top(0),
    Angle(0.0),
    Color(0xFFFFFFFF)
{
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
float Object::GetLeft()
{
    return Left;
}

/**
 * Set the left position of the object.
 * @param[in] Left Left in pixel.
 * @see GetLeft()
 */
void Object::SetLeft(float Left)
{
    this->Left = Left;
}

/**
 * Get the top position of the object.
 * @see SetTop()
 * @return Top position in pixel.
 */
float Object::GetTop()
{
    return Top;
}

/**
 * Set the top position of the object.
 * @param[in] Top Top in pixel.
 * @see GetTop()
 */
void Object::SetTop(float Top)
{
    this->Top = Top;
}

/**
 * Set the left and top position of the object.
 * @param[in] Left Left in pixel.
 * @param[in] Top Top in pixel.
 * @see SetLeft()
 * @see SetTop()
 */
void Object::SetLocation(float Left, float Top)
{
    this->Left = Left;
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
 * Check if a position is inside the object.
 * @param[in] x X position in pixel.
 * @param[in] y Y position in pixel.
 * @return Return true if a position is inside the object, false otherwize.
 */
bool Object::IsInside(float x, float y)
{
    if (x > Left && x < (Left + Width) && y > Top && y < (Top + Height))
        return true;
    return false;
}

/**
 * Get the angle of the object.
 * @return Angle in user unit.
 */
float Object::GetAngle()
{
    return Angle;
}

/**
 * Set the angle of the object.
 * @param[in] Angle Angle in user unit.
 */
void Object::SetAngle(float Angle)
{
    this->Angle = Angle;
}

/**
 * Get the alpha of the object.
 * @return Alpha color (0-255).
 */
u8 Object::GetAlpha()
{
    return A(Color);
}

/**
 * Set the alpha channel of the object.
 * @param[in] Alpha Alpha color.
 */
void Object::SetAlpha(u8 Alpha)
{
    Color = (Color & 0xFFFFFF00) | (Alpha & 0xFF);
}

/**
 * Get the color of the object.
 * @return The color of the object (RGBA).
 */
u32 Object::GetColor()
{
    return Color;
}

/**
 * Set the color of the texture.
 * @param[in] AColor New color (RGB).
 */
void Object::SetColor(u32 AColor)
{
   Color = AColor;
}
