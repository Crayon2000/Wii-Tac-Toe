/**
 * @file game.h
 * Contains the Game class.
 */

#ifndef GameH
#define GameH
//---------------------------------------------------------------------------

#include "grrlib_class.h"
#include "grrlib/GRRLIB.h"
#include <gcmodplay.h>
#include <vector>

#define TEXT_SIZE 100   /**< Maximum length of the text. */

// Forward declarations
class Grid;
class Symbol;
class Button;
class Cursor;
class Player;
class Language;

/**
 * This is the main class of this project. This is where the magic happens.
 * @author Crayon
 */
class Game
{
public:
    Game(u16, u16);
    ~Game();
    void Paint();
    bool ControllerManager();
private:
    /**
     * Types of game.
     */
    enum gameMode {
        modeVsAI,       /**< Play vs AI. */
        modeVsHuman1,   /**< Play vs a human with 1 Wii Remote. */
        modeVsHuman2    /**< Play vs a human with 2 Wii Remote. */
    };

    void StartSreen();
    void MenuScreen(bool);
    void GameScreen(bool);
    void ExitScreen();
    void Clear();
    void TurnIsOver();
    void NewGame();
    void PrintWrapText(u16, u16, u16, const std::wstring &, unsigned int, unsigned int, unsigned int, s8, s8);
    void ChangeScreen(u8 NewScreen, bool = true);
    void ButtonOn(s8);
    bool SelectZone();
    void ChangeCursor();
    void CalculateFrameRate();

    Cursor *Hand;
    s8 HandX, HandY;

    bool CurrentPlayer;
    bool PlayerToStart;
    Player *WTTPlayer;
    u8 CurrentScreen;
    u8 LastScreen;
    s8 SelectedButton;
    wchar_t text[TEXT_SIZE];

    u16 TieGame;
    bool RoundFinished;
    u8 FPS;
    bool ShowFPS;

    std::vector<Button *> ExitButton;
    std::vector<Button *> MenuButton;
    Grid *GameGrid;
    Language *Lang;
    Symbol **GridSign;

    Texture *GameImg;     /**< Background texture for the game. */
    Texture *SplashImg;   /**< Splash screen texture. */
    Texture *HoverImg;    /**< Texture to put over a symbol when selected. */
    Texture *CopiedImg;   /**< Texture to store a temporary copy of the screen. */

    bool Copied;

    u16 ScreenWidth, ScreenHeight;

    u8 AIThinkLoop;
    gameMode GameMode;

    u8 SymbolAlpha;
    bool AlphaDirection;

    MODPlay ModTrack;

    GRRLIB_ttfFont *DefaultFont;
};
//---------------------------------------------------------------------------
#endif
