/**
 * @file symbol.h
 * Contains the Symbol class.
 */

#ifndef SymbolH
#define SymbolH
//---------------------------------------------------------------------------

#include "object.h"
#include "types.h"
#include "grrlib_class.h"

/**
 * This class is used to draw X or O on the screen.
 * @author Crayon
 */
class Symbol : public Object
{
public:
    Symbol();
    ~Symbol();
    void Paint() override;
    void SetPlayer(u8 APlayer);
    void SetLocation(Point APoint);
private:
    Texture *Current;
    Texture *ImgX;
    Texture *ImgO;
};
//---------------------------------------------------------------------------
#endif
