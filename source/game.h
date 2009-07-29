/**
 * @file game.h
 * Contains the Game class.
 */
 
#ifndef GameH
#define GameH
//---------------------------------------------------------------------------

#include "grid.h"
#include "symbol.h"
#include "button.h"
#include "cursor.h"
#include "player.h"
#include "language.h"
#include "types.h"

#define TEXT_SIZE 100   /**< Maximum length of the text. */

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
    void PrintWrapText(u16, u16, u16, const char*, unsigned int, unsigned int);
    void ChangeScreen(u8 NewScreen);
    void ButtonOn(s8);
    bool SelectZone();
    void FreeMemImg();
    void ChangeCursor();

    Cursor *Hand;
    signed char HandX, HandY;

    bool CurrentPlayer;
    bool PlayerToStart;
    Player *WTTPlayer;
    u8 CurrentScreen;
    u8 LastScreen;
    signed char SelectedButton;
    char text[TEXT_SIZE];

    u16 TieGame;
    bool RoundFinished;

    Button *ExitButton1;
    Button *ExitButton2;
    Button *ExitButton3;
    Button *MenuButton;
    Grid *GameGrid;
    Language *Lang;
    Symbol **GridSign;

    GRRLIB_texImg *GameImg;
    GRRLIB_texImg *SplashImg;
    GRRLIB_texImg *HoverImg;
    GRRLIB_texImg *CopiedImg;

    u16 ScreenWidth, ScreenHeight;

    u8 AIThinkLoop;
    gameMode GameMode;
};
//---------------------------------------------------------------------------
#endif
