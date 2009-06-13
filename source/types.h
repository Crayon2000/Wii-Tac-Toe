//---------------------------------------------------------------------------
#ifndef TypesH
#define TypesH
//---------------------------------------------------------------------------

#include <gccore.h>

/**
 * Class used to defines a pixel location onscreen.
 * @author Crayon
 */
class Point
{
private:
    u16 x, y;
public:
    /**
     * Constructor for the Point class.
     * Initializes a point at the origin (0, 0) of the coordinate space.
     */
    Point()
    {
        this->x = 0;
        this->y = 0;
    }
    /**
     * Constructor for the Point class.
     * @param[in] x Specifies the x-coordinate.
     * @param[in] y Specifies the y-coordinate.
     */
    Point(u16 x, u16 y)
    {
        this->x = x;
        this->y = y;
    }
    /**
     * Get the x position.
     */
    u16 GetX()
    {
        return x;
    }
    /**
     * Get the y position.
     */
    u16 GetY()
    {
        return y;
    }
    /**
     * Set the new position.
     * @param[in] x Specifies the x-coordinate.
     * @param[in] y Specifies the y-coordinate.
     */
    void SetLocation(u16 x, u16 y)
    {
        this->x = x;
        this->y = y;
    }
};
//---------------------------------------------------------------------------
#endif
