/**
 * @file symbol.h
 * Contains the Symbol class.
 */

#ifndef SymbolH
#define SymbolH
//---------------------------------------------------------------------------

#include "object.h"
#include "types.h"

/**
 * This class is used to draw X or O on the screen.
 * @author Crayon
 */
class Symbol : public Object
{
public:
    Symbol();
    ~Symbol();
    void Paint();
    void SetPlayer(unsigned char);
    void SetLocation(Point);
private:
    unsigned char Player;
    GRRLIB_texImg *ImgX;
    GRRLIB_texImg *ImgO;
};
//---------------------------------------------------------------------------
#endif
