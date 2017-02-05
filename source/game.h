/**
 * @file game.h
 * Contains the Game class.
 */

#ifndef GameH
#define GameH
//---------------------------------------------------------------------------

#include <array>
#include <string>
#include "cursor.h"
#include "player.h"
#include "button.h"
#include "symbol.h"

// Forward declarations
class Grid;
class Language;
class Audio;
struct GRRLIB_Font;

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
    enum class gameMode : u8 {
        VsAI,       /**< Play vs AI. */
        VsHuman1,   /**< Play vs a human with 1 Wii Remote. */
        VsHuman2    /**< Play vs a human with 2 Wii Remote. */
    };

    void StartSreen();
    void MenuScreen(bool);
    void GameScreen(bool);
    void ExitScreen();
    void Clear();
    void TurnIsOver();
    void NewGame();
    void PrintWrapText(u16, u16, u16, const std::string &, u32, u32, u32, s8, s8);
    void ChangeScreen(u8 NewScreen, bool = true);
    void ButtonOn(s8);
    bool SelectZone();
    void ChangeCursor();
    void CalculateFrameRate();

    std::array<Cursor, 2> Hand;
    s8 HandX;
    s8 HandY;

    bool CurrentPlayer;
    bool PlayerToStart;
    std::array<Player, 2> WTTPlayer;
    u8 CurrentScreen;
    u8 LastScreen;
    s8 SelectedButton;
    std::string text;

    u16 TieGame;
    bool RoundFinished;
    u8 FPS;
    bool ShowFPS;

    std::array<Button *, 3> ExitButton;
    std::array<Button *, 3> MenuButton;
    Grid *GameGrid;
    Language *Lang;
    std::array<std::array<Symbol, 3>, 3> GridSign;
    Audio *GameAudio;

    Texture *GameImg; /**< Background texture for the game. */
    Texture *GameHoverImg; /**< Hover texture for the game buttons. */
    Texture *SplashImg; /**< Splash screen texture. */
    Texture *SplashArmImg; /**< Arm texture for splash screen. */
    Texture *HoverImg; /**< Texture to put over a symbol when selected. */
    Texture *CopiedImg; /**< Texture to store a temporary copy of the screen. */
    Texture *GameText; /**< Game text that does not changed including background. */

    bool Copied;

    u16 ScreenWidth;
    u16 ScreenHeight;

    u8 AIThinkLoop;
    gameMode GameMode;

    u8 SymbolAlpha;
    bool AlphaDirection;

    GRRLIB_Font *DefaultFont;
};
//---------------------------------------------------------------------------
#endif
