/**
 * @file object.h
 * Contains the Object class.
 */

#ifndef ObjectH
#define ObjectH
//---------------------------------------------------------------------------

#include <cstdlib>
#include <gccore.h>

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
    u32 GetColor();
    bool IsVisible();
    void SetLeft(float Left);
    void SetTop(float Top);
    void SetLocation(float Left, float Top);
    void SetWidth(unsigned int Width);
    void SetHeight(unsigned int Height);
    void SetSize(unsigned int Width, unsigned int Height);
    void SetAngle(float Angle);
    void SetAlpha(u8 Alpha);
    void SetColor(u32 AColor);
    void SetVisible(bool Visible);
    bool IsInside(float x, float y);
protected:
    virtual void Paint() = 0; /**< Draw an image on screen. */

    float Left;         /**< Specifies the x-coordinate of the upper-left corner of the object. */
    float Top;          /**< Specifies the y-coordinate of the upper-left corner of the object. */
    unsigned int Width; /**< The width of the object. */
    unsigned int Height;/**< The height of the object. */
    float Angle;        /**< The angle in degree of the object. */
    bool Visible;       /**< Control the visibility of the object. If Visible is true, the object appears. If Visible is false, the object is not visible. */
    u32 Color;          /**< Color of the object (RGBA). */
    //char *Name;
    //void *Parent;
};
//---------------------------------------------------------------------------
#endif
