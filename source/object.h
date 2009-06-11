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
    void SetLocation(float, float);
    void SetWidth(unsigned int);
    void SetHeight(unsigned int);
    void SetSize(unsigned int, unsigned int);
    void SetAngle(float);
    void SetAlpha(u8);
    void SetVisible(bool);
    bool IsInside(float, float);
protected:
    virtual void Paint() = 0; /**< Draw an image on screen. */

    float Left;         /**< Specifies the x-coordinate of the upper-left corner of the object. */
    float Top;          /**< Specifies the y-coordinate of the upper-left corner of the object. */
    unsigned int Width; /**< The width of the object. */
    unsigned int Height;/**< The height of the object. */
    float Angle;        /**< The angle in degree of the object. */
    bool Visible;       /**< Control the visibility of the objet. If Visible is true, the objet appears. If Visible is false, the objet is not visible. */
    u8 Alpha;           /**< Change the opacity of the object (0-255). */
    //char *Name;
    //void *Parent;
};
//---------------------------------------------------------------------------
#endif
