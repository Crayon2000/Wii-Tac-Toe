/**
 * @file button.h
 * Contains the Button class.
 */

#ifndef ButtonH
#define ButtonH
//---------------------------------------------------------------------------

#include <string>
#include "object.h"
#include "grrlib_class.h"

/**
 * Types of button that could be used.
 */
enum class buttonType : u8 {
    Home,     /**< Home button with a small house. */
    HomeMenu, /**< Button to go in the HOME menu. A shadow is always present under the button. */
    StdMenu   /**< Button to go in a menu. Change color when the cursor over the button. */
};

/**
 * This is a class to make a button on the screen with two states (selected or not).
 * @author Crayon
 */
class Button : public Object
{
public:
    Button(buttonType = buttonType::StdMenu);
    ~Button();
    void Paint() override;
    void SetCaption(const std::string &NewCaption);
    void SetFont(GRRLIB_ttfFont *AFont);
    void SetSelected(bool IsSelected);
    void SetTextColor(u32 NewColor);
    void SetTextHeight(unsigned int NewHeight);
private:
    bool Selected;
    std::string Caption;
    GRRLIB_ttfFont *Font;
    unsigned int TextWidth;
    unsigned int TextHeight;
    unsigned int TextTop;
    unsigned int TextLeft;
    u32 TextColor;
    buttonType Type;
    Texture *ButtonImgOn;
    Texture *ButtonImgOff;
};
//---------------------------------------------------------------------------
#endif
