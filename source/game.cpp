#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <ogc/conf.h>
#include <ogc/lwp_watchdog.h>
#include <format.hpp>
#include <algorithm/string/replace.hpp>
#include "grrlib/GRRLIB.h"
#include "grrlib_class.h"
#include "tools.h"
#include "grid.h"
#include "audio.h"
#include "symbol.h"
#include "button.h"
#include "cursor.h"
#include "player.h"
#include "language.h"
#include "types.h"
#include "game.h"

// Graphics
#include "../gfx/splash.h"
#include "../gfx/backg.h"
#include "../gfx/hover.h"

// Font
#include "../fonts/Swis721_Ex_BT.h"

#define START_SCREEN    0
#define GAME_SCREEN     1
#define HOME_SCREEN     2
#define MENU_SCREEN     3

/**
 * Array to hold the position of each zone.
 */
static Point Table[3][3] = {
    {Point(180, 28), Point(180, 131), Point(180, 233)},
    {Point(322, 28), Point(322, 131), Point(322, 233)},
    {Point(464, 28), Point(464, 131), Point(464, 233)}};

/**
 * Constructor for the Game class.
 * @param[in] GameScreenWidth Screen width.
 * @param[in] GameScreenHeight Screen height.
 */
Game::Game(u16 GameScreenWidth, u16 GameScreenHeight) :
    FPS(0),
    ShowFPS(false)
{
    srand(time(NULL));

    ScreenWidth = GameScreenWidth;
    ScreenHeight = GameScreenHeight;

    GameGrid = new Grid();
    Hand = new Cursor[2];
    Lang = new Language();

    DefaultFont = GRRLIB_LoadTTF(Swis721_Ex_BT, Swis721_Ex_BT_size);

    u8 x, y;
    GridSign = new Symbol*[3];
    for(x = 0; x < 3; ++x)
    {
        GridSign[x] = new Symbol[3];
        for(y = 0; y < 3; ++y)
        {
            GridSign[x][y].SetLocation(Table[x][y]);
        }
    }

    Hand[0].SetVisible(false);
    Hand[1].SetVisible(false);

    ExitButton.push_back(new Button(btnHome));
    ExitButton[0]->SetFont(DefaultFont);
    ExitButton[0]->SetLeft(430);
    ExitButton[0]->SetTop(20);
    ExitButton[0]->SetTextHeight(20);
    ExitButton[0]->SetCaption(Lang->String("Close"));

    ExitButton.push_back(new Button(btnHomeMenu));
    ExitButton[1]->SetFont(DefaultFont);
    ExitButton[1]->SetLeft((ScreenWidth / 2) + 20);
    ExitButton[1]->SetTop(165);
    ExitButton[1]->SetCaption(Lang->String("Reset"));

    ExitButton.push_back(new Button(btnHomeMenu));
    ExitButton[2]->SetFont(DefaultFont);
    ExitButton[2]->SetLeft((ScreenWidth / 2) - ExitButton[1]->GetWidth() - 20);
    ExitButton[2]->SetTop(165);
    //if(!!*(u32 *)0x80001800) // that returns true for hbc, false for load-from-trucha-signed-disc. think it also returns false for tp hack
    //    ExitButton[2]->SetCaption("Return to HBC");
    //else
        ExitButton[2]->SetCaption(Lang->String("Return to Loader"));

    MenuButton.push_back(new Button());
    MenuButton[0]->SetFont(DefaultFont);
    MenuButton[0]->SetLeft((ScreenWidth / 2) - (MenuButton[0]->GetWidth() / 2));
    MenuButton[0]->SetTop(92);
    MenuButton[0]->SetCaption(Lang->String("2 Players (1 Wiimote)"));

    MenuButton.push_back(new Button());
    MenuButton[1]->SetFont(DefaultFont);
    MenuButton[1]->SetLeft((ScreenWidth / 2) - (MenuButton[1]->GetWidth() / 2));
    MenuButton[1]->SetTop(292);
    MenuButton[1]->SetCaption(Lang->String("1 Player (Vs AI)"));

    MenuButton.push_back(new Button());
    MenuButton[2]->SetFont(DefaultFont);
    MenuButton[2]->SetLeft((ScreenWidth / 2) - (MenuButton[2]->GetWidth() / 2));
    MenuButton[2]->SetTop(192);
    MenuButton[2]->SetCaption(Lang->String("2 Players (2 Wiimote)"));

    WTTPlayer = new Player[2];
    WTTPlayer[0].SetSign('X');
    WTTPlayer[0].SetName(Lang->String("PLAYER 1"));
    WTTPlayer[1].SetSign('O');
    WTTPlayer[1].SetName(Lang->String("PLAYER 2"));

    GameImg = new Texture(backg, backg_size);
    SplashImg = new Texture(splash, splash_size);
    HoverImg = new Texture(hover, hover_size);
    CopiedImg = new Texture(ScreenWidth, ScreenHeight);
    GameText = new Texture(ScreenWidth, ScreenHeight);

    // Player name with a shadow offset of -2, 2 (includes game background)
    // GameText should only be modified when player names changed.
    GameImg->Draw(0, 0);
    PrintWrapText(44, 48, 125, WTTPlayer[0].GetName(), 15, 0xFFFFFFFF, 0x6BB6DEFF, -2, 2);
    PrintWrapText(44, 143, 125, WTTPlayer[1].GetName(), 15, 0xFFFFFFFF, 0xE6313AFF, -2, 2);
    PrintWrapText(44, 248, 125, Lang->String("TIE GAME"), 15, 0xFFFFFFFF, 0x109642FF, -2, 2);
    GameText->CopyScreen(0, 0, true);

    // Build Start Screen background
    SplashImg->Draw(0, 0);
    GRRLIB_PrintfTTF(50, 310, DefaultFont,
        boost::str(boost::format(Lang->String("Programmer: %s")) %"Crayon").c_str(),
        11, 0xFFFFFFFF);
    GRRLIB_PrintfTTF(50, 330, DefaultFont,
        boost::str(boost::format(Lang->String("Graphics: %s")) %"Mr_Nick666").c_str(),
        11, 0xFFFFFFFF);
    text = Lang->String("Press The A Button");
    GRRLIB_PrintfTTF((ScreenWidth / 2) - (GRRLIB_WidthTTF(DefaultFont, text.c_str(), 20) / 2),
                    400, DefaultFont, text.c_str(), 20, 0x000000FF);
    SplashImg->CopyScreen(0, 0, true);

    GameAudio = new Audio();

    RUMBLE_Init();
    NewGame();
}

/**
 * Destructor for the Game class.
 */
Game::~Game()
{
    delete GameText;
    delete GameImg;
    delete SplashImg;
    delete HoverImg;
    delete CopiedImg;
    GRRLIB_FreeTTF(DefaultFont);

    delete GameGrid;
    delete[] Hand;
    delete Lang;
    delete[] WTTPlayer;

    for(u8 i=0; i < ExitButton.size(); ++i)
    {
        delete ExitButton[i];
    }
    ExitButton.clear();

    for(u8 i=0; i < MenuButton.size(); ++i)
    {
        delete MenuButton[i];
    }
    MenuButton.clear();

    for(u8 i = 0; i < 3; ++i)
    {
        delete[] GridSign[i];
    }
    delete[] GridSign;

    delete GameAudio;
}

/**
 * Draw the proper screen.
 */
void Game::Paint()
{
    switch(CurrentScreen)
    {
        case START_SCREEN:
            StartSreen();
            break;
        case MENU_SCREEN:
            MenuScreen(true);
            break;
        case HOME_SCREEN:
            ExitScreen();
            break;
        case GAME_SCREEN:
            GameScreen(true);
            // AI
            if(!RoundFinished && WTTPlayer[CurrentPlayer].GetType() == PLAYER_CPU)
            {   // AI
                if(AIThinkLoop > (rand() % 10 + 20))
                {
                    GameGrid->SetPlayerAI(WTTPlayer[CurrentPlayer].GetSign());
                    TurnIsOver();
                    AIThinkLoop = 0;
                }
                else
                {
                    ++AIThinkLoop;
                }
            }
            break;
        default:
            GRRLIB_FillScreen(0x000000FF);
    }

    if(CurrentScreen != START_SCREEN &&
        WPAD_Probe(WPAD_CHAN_0, NULL) == WPAD_ERR_NO_CONTROLLER)
    {   // Controller is disconnected
        Rectangle(0, 0, ScreenWidth, ScreenHeight, 0x000000B2, 1);
    }
    else
    {
        Hand[1].Paint();
        Hand[0].Paint();
    }
    if(ShowFPS)
    {
        CalculateFrameRate();
        char strFPS[10];
        snprintf(strFPS, 10, "FPS: %d", FPS);
        GRRLIB_PrintfTTF(14, 444, DefaultFont, strFPS, 17, 0xFFFFFFFF);
        GRRLIB_PrintfTTF(16, 446, DefaultFont, strFPS, 17, 0x808080FF);
        GRRLIB_PrintfTTF(15, 445, DefaultFont, strFPS, 17, 0x000000FF);
    }
}

/**
 * Draw the start screen.
 */
void Game::StartSreen()
{
    SplashImg->Draw(0, 0);
}

/**
 * Draw the game screen.
 * @param[in] CopyScreen If true the screen is copied to a buffer.
 */
void Game::GameScreen(bool CopyScreen)
{
    if(!Copied)
    {   // Copy static element
        GameText->Draw(0, 0); // Background image with some text

        // Draw score with a shadow offset of -2, 2
        char ScoreText[5];
        snprintf(ScoreText, 5, "%d", WTTPlayer[0].GetScore());
        int TextLeft = 104 - GRRLIB_WidthTTF(DefaultFont, ScoreText, 35) / 2;
        GRRLIB_PrintfTTF(TextLeft, 77, DefaultFont, ScoreText, 35, 0x6BB6DEFF);
        GRRLIB_PrintfTTF(TextLeft - 2, 75, DefaultFont, ScoreText, 35, 0xFFFFFFFF);

        snprintf(ScoreText, 5, "%d", WTTPlayer[1].GetScore());
        TextLeft = 104 - GRRLIB_WidthTTF(DefaultFont, ScoreText, 35) / 2;
        GRRLIB_PrintfTTF(TextLeft, 177, DefaultFont, ScoreText, 35, 0xE6313AFF);
        GRRLIB_PrintfTTF(TextLeft - 2, 175, DefaultFont, ScoreText, 35, 0xFFFFFFFF);

        snprintf(ScoreText, 5, "%d", TieGame);
        TextLeft = 104 - GRRLIB_WidthTTF(DefaultFont, ScoreText, 35) / 2;
        GRRLIB_PrintfTTF(TextLeft, 282, DefaultFont, ScoreText, 35, 0x109642FF);
        GRRLIB_PrintfTTF(TextLeft - 2, 280, DefaultFont, ScoreText, 35, 0xFFFFFFFF);

        // Draw text at the bottom with a shadow offset of 1, 1
        PrintWrapText(130, 420, 390, text, 15, 0x8C8A8CFF, 0x111111FF, 1, 1);

        if(CopyScreen)
        {
            CopiedImg->CopyScreen();
            Copied = true;
        }
    }
    if(CopyScreen)
    {
        CopiedImg->Draw(0, 0);
    }

    u32 HoverColor = (WTTPlayer[CurrentPlayer].GetSign() == 'X') ? 0x0093DDFF : 0xDA251DFF;

    // Draw grid content
    if(RoundFinished)
    {
        if(AlphaDirection)
        {
            SymbolAlpha += 2;
            if(SymbolAlpha > 128)
            {
                AlphaDirection = !AlphaDirection;
            }
        }
        else
        {
            SymbolAlpha -= 2;
            if(SymbolAlpha < 5)
            {
                AlphaDirection = !AlphaDirection;
            }
        }
    }
    u8 y;
    for(u8 x = 0; x < 3; ++x)
    {
        for(y = 0; y < 3; ++y)
        {
            GridSign[x][y].SetPlayer(GameGrid->GetPlayerAtPos(x, y));
            GridSign[x][y].SetColor(0xFFFFFFFF);
            GridSign[x][y].Paint();
            if(GameGrid->WinningBoard[x][y])
            {
                GridSign[x][y].SetColor(HoverColor);
                GridSign[x][y].SetAlpha(SymbolAlpha);
                GridSign[x][y].Paint();
            }
        }
    }

    // Draw selection box
    if(SelectZone() && GameGrid->GetPlayerAtPos(HandX, HandY) == ' ')
    {
        HoverImg->Draw(Table[HandX][HandY].GetX(), Table[HandX][HandY].GetY(),
            0, 1, 1, HoverColor);
    }
}

/**
 * Draw the HOME screen.
 */
void Game::ExitScreen()
{
    GameAudio->PauseMusic(true);
    if(!Copied)
    {   // Copy static element
        switch(LastScreen)
        {
            case GAME_SCREEN:
                GameScreen(false);
                break;
            case MENU_SCREEN:
                MenuScreen(false);
                break;
        }

        Rectangle(0, 0, ScreenWidth, ScreenHeight, 0x000000CC, 1); // Draw a black rectangle over it

        Rectangle(0, 78, ScreenWidth, 2, 0x848284FF, 1);

        Rectangle(0, 383, ScreenWidth, 2, 0x848284FF, 1);
        Rectangle(0, 385, ScreenWidth, 95, 0x000000FF, 1);

        CopiedImg->CopyScreen();
        Copied = true;
    }
    else
    {
        CopiedImg->Draw(0, 0);
    }

    if(GRRLIB_PtInRect(0, 0, ScreenWidth, 78, Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        Rectangle(0, 0, ScreenWidth, 78, 0x30B6EBFF, 1);
    }
    else
    {
        Rectangle(0, 0, ScreenWidth, 78, 0x000000FF, 1);
    }

    GRRLIB_PrintfTTF(30, 20, DefaultFont, Lang->String("HOME Menu").c_str(), 30, 0xFFFFFFFF);

    ExitButton[0]->SetSelected(false);
    ExitButton[1]->SetSelected(false);
    ExitButton[2]->SetSelected(false);
    if(GRRLIB_PtInRect(0, 0, ScreenWidth, 78, Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton[0]->SetSelected(true);
        ButtonOn(0);
        SelectedButton = 0;
    }
    else if(ExitButton[1]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton[1]->SetSelected(true);
        ButtonOn(1);
        SelectedButton = 1;
    }
    else if(ExitButton[2]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton[2]->SetSelected(true);
        ButtonOn(2);
        SelectedButton = 2;
    }
    else
    {
        SelectedButton = -1;
    }
    ExitButton[0]->Paint();
    ExitButton[1]->Paint();
    ExitButton[2]->Paint();
}

/**
 * Draw the menu screen.
 * @param[in] CopyScreen If true the screen is copied to a buffer.
 */
void Game::MenuScreen(bool CopyScreen)
{
    if(!Copied)
    {   // Copy static element
        FillScreen(0x000000FF); // Clear screen
        for(int y = 0; y <=ScreenHeight; y+=8)
        {
            Rectangle(0, y, ScreenWidth, 2, 0xB0B0B030, 1);
        }

        Rectangle(0, 0, ScreenWidth, 63, 0x000000FF, 1);
        Rectangle(0, 63, ScreenWidth, 2, 0xFFFFFFFF, 1);

        Rectangle(0, 383, ScreenWidth, 2, 0xFFFFFFFF, 1);
        Rectangle(0, 385, ScreenWidth, 95, 0x000000FF, 1);

        GRRLIB_PrintfTTF(500, 40, DefaultFont,
            boost::str(boost::format(Lang->String("Ver. %s")) %"0.8").c_str(),
            12, 0xFFFFFFFF);

        if(CopyScreen)
        {
            CopiedImg->CopyScreen();
            Copied = true;
        }
    }
    else
    {
        CopiedImg->Draw(0, 0);
    }

    MenuButton[0]->SetSelected(false);
    MenuButton[1]->SetSelected(false);
    MenuButton[2]->SetSelected(false);
    if(MenuButton[0]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        MenuButton[0]->SetSelected(true);
        ButtonOn(0);
        SelectedButton = 0;
    }
    else if(MenuButton[1]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        MenuButton[1]->SetSelected(true);
        ButtonOn(1);
        SelectedButton = 1;
    }
    else if(MenuButton[2]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        MenuButton[2]->SetSelected(true);
        ButtonOn(2);
        SelectedButton = 2;
    }
    else
    {
        SelectedButton = -1;
    }
    MenuButton[0]->Paint();
    MenuButton[1]->Paint();
    MenuButton[2]->Paint();
}

/**
 * Controls all inputs.
 * @return True to exit to loader, false otherwise.
 */
bool Game::ControllerManager()
{
    RUMBLE_Verify();
    WPADData *WPadData0 = WPAD_Data(WPAD_CHAN_0);
    WPADData *WPadData1 = WPAD_Data(WPAD_CHAN_1);
    u32 Buttons0 = WPadData0->btns_d;
    u32 Buttons1 = WPadData1->btns_d;

    if(WPadData0->ir.valid)
    {
        // I don't understand this calculation but it works
        Hand[0].SetLeft((WPadData0->ir.x / ScreenWidth * (ScreenWidth + Hand[0].GetWidth() * 2)) - Hand[0].GetWidth());
        Hand[0].SetTop((WPadData0->ir.y / ScreenHeight * (ScreenHeight + Hand[0].GetHeight() * 2)) - Hand[0].GetHeight());
        Hand[0].SetAngle(WPadData0->orient.roll);
        Hand[0].SetVisible(true);
    }
    else
    {   // Hide cursor
        Hand[0].SetVisible(false);
    }
    if(WPadData1->ir.valid && (CurrentScreen == HOME_SCREEN ||
        CurrentScreen == START_SCREEN || CurrentScreen == MENU_SCREEN ||
        (CurrentScreen == GAME_SCREEN && GameMode == modeVsHuman2) ))
    {
        // I don't understand this calculation but it works
        Hand[1].SetLeft((WPadData1->ir.x / ScreenWidth * (ScreenWidth + Hand[1].GetWidth() * 2)) - Hand[1].GetWidth());
        Hand[1].SetTop((WPadData1->ir.y / ScreenHeight * (ScreenHeight + Hand[1].GetHeight() * 2)) - Hand[1].GetHeight());
        Hand[1].SetAngle(WPadData1->orient.roll);
        Hand[1].SetVisible(true);
    }
    else
    {   // Hide cursor
        Hand[1].SetVisible(false);
    }

    if(Buttons0 || Buttons1)
    {
        switch(CurrentScreen)
        {
            case START_SCREEN:
                if((Buttons0 & WPAD_BUTTON_A))
                {
                    ChangeScreen(MENU_SCREEN);
                }
                break;
            case MENU_SCREEN:
                if((Buttons0 & WPAD_BUTTON_A))
                {
                    switch(SelectedButton)
                    {
                        case 0:
                            WTTPlayer[1].SetType(PLAYER_HUMAN);
                            GameMode = modeVsHuman1;
                            ChangeScreen(GAME_SCREEN);
                            break;
                        case 1:
                            WTTPlayer[1].SetType(PLAYER_CPU);
                            GameMode = modeVsAI;
                            ChangeScreen(GAME_SCREEN);
                            break;
                        case 2:
                            WTTPlayer[1].SetType(PLAYER_HUMAN);
                            GameMode = modeVsHuman2;
                            ChangeScreen(GAME_SCREEN);
                            break;
                    }
                }
                else if((Buttons0 & WPAD_BUTTON_B))
                {
                    ChangeScreen(START_SCREEN);
                }
                else if((Buttons0 & WPAD_BUTTON_HOME) || (Buttons1 & WPAD_BUTTON_HOME))
                {
                    ChangeScreen(HOME_SCREEN);
                }
                break;
            case HOME_SCREEN:
                if((Buttons0 & WPAD_BUTTON_HOME) || (Buttons1 & WPAD_BUTTON_HOME))
                {
                    GameAudio->PauseMusic(false);
                    ChangeScreen(LastScreen);
                }
                else if((Buttons0 & WPAD_BUTTON_A))
                {
                    switch(SelectedButton)
                    {
                        case 0:
                            GameAudio->PauseMusic(false);
                            ChangeScreen(LastScreen);
                            break;
                        case 1:
                            NewGame();
                            break;
                        case 2:
                            ExitScreen();
                            Hand[1].Paint();
                            Hand[0].Paint();
                            CopiedImg->CopyScreen();
                            WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off, just in case
                            WPAD_Rumble(WPAD_CHAN_1, 0); // Rumble off, just in case
                            Draw_FadeOut(CopiedImg, 1, 1, 3);
                            return true; // Exit to loader
                    }
                }
                break;
            default:
                if((Buttons0 & WPAD_BUTTON_HOME) || (Buttons1 & WPAD_BUTTON_HOME))
                {
                    ChangeScreen(HOME_SCREEN);
                }

                if((Buttons0 & WPAD_BUTTON_A))
                {
                    if(RoundFinished)
                    {
                        Clear();
                    }
                    else if(GameMode == modeVsHuman2)
                    {
                        if(CurrentPlayer == 0)
                        {
                            if(GameGrid->SetPlayer(WTTPlayer[0].GetSign(), HandX, HandY))
                            {
                                TurnIsOver();
                            }
                            else
                            {   // Position is invalid
                                RUMBLE_Wiimote(WPAD_CHAN_0, 200); // 200 ms
                            }
                        }
                    }
                    else
                    {
                        if(GameGrid->SetPlayer(WTTPlayer[CurrentPlayer].GetSign(), HandX, HandY))
                        {
                            TurnIsOver();
                        }
                        else
                        {   // Position is invalid
                            RUMBLE_Wiimote(WPAD_CHAN_0, 200); // 200 ms
                        }
                    }
                }

                if((Buttons1 & WPAD_BUTTON_A) && GameMode == modeVsHuman2)
                {
                    if(RoundFinished)
                    {
                        Clear();
                    }
                    else if(CurrentPlayer == 1)
                    {
                        if(GameGrid->SetPlayer(WTTPlayer[1].GetSign(), HandX, HandY))
                        {
                            TurnIsOver();
                        }
                        else
                        {   // Position is invalid
                            RUMBLE_Wiimote(WPAD_CHAN_1, 200); // 200 ms
                        }
                    }
                }
        }
    }
    if(((WPadData0->btns_h & WPAD_BUTTON_1) && (WPadData0->btns_h & WPAD_BUTTON_2)) ||
        ((WPadData1->btns_h & WPAD_BUTTON_1) && (WPadData1->btns_h & WPAD_BUTTON_2)))
    {
        WPAD_Rumble(WPAD_CHAN_0, 1); // Rumble on
        WPAD_Rumble(WPAD_CHAN_1, 1); // Rumble on
        WIILIGHT_TurnOn();

        char path[255];
        time_t now = time(NULL);
        struct tm *ti = localtime(&now);
        sprintf(path, "sd:/Screenshot %d-%02d-%02d %02d%02d%02d.png",
            ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);

        if(ScreenShot(path))
        {
            text = "A screenshot was taken!!!";
        }
        else
        {
            text = "Screenshot did not work!!!";
        }

        WIILIGHT_TurnOff();
        WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off
        WPAD_Rumble(WPAD_CHAN_1, 0); // Rumble off
        Copied = false;
    }
    if((Buttons0 & WPAD_BUTTON_PLUS) || (Buttons1 & WPAD_BUTTON_PLUS))
    {
        ShowFPS = !ShowFPS;
    }
    return false;
}

/**
 * Start a new round.
 */
void Game::Clear()
{
    GameGrid->Clear();
    CurrentPlayer = PlayerToStart;
    PlayerToStart = !PlayerToStart; // Next other player will start
    text = boost::str(boost::format(Lang->GetTurnOverMessage()) %WTTPlayer[CurrentPlayer].GetName());
    RoundFinished = false;
    Copied = false;
    ChangeCursor();
}

/**
 * Things to do when a turn is over.
 */
void Game::TurnIsOver()
{
    u8 GameWinner = GameGrid->GetWinner();
    if(GameWinner != ' ')
    {   // A winner is declare
        GameWinner = (GameWinner == WTTPlayer[0].GetSign()) ? 0 : 1;
        WTTPlayer[GameWinner].IncScore();
        text = Lang->GetWinningMessage();
        boost::replace_all(text, "$LOSER$", WTTPlayer[!GameWinner].GetName());
        boost::replace_all(text, "$WINNER$", WTTPlayer[GameWinner].GetName());
        RoundFinished = true;
        SymbolAlpha = 5;
        AlphaDirection = 0;
    }
    else if(GameGrid->IsFilled())
    {   // Tie game
        ++TieGame;
        text = Lang->GetTieMessage();
        RoundFinished = true;
    }
    else
    {
        CurrentPlayer = !CurrentPlayer; // Change player's turn
        text = boost::str(boost::format(Lang->GetTurnOverMessage()) %WTTPlayer[CurrentPlayer].GetName());
    }

    Copied = false;
    ChangeCursor();
}

/**
 * Start a new game, initialize variables.
 */
void Game::NewGame()
{
    GameAudio->LoadMusic();

    PlayerToStart = rand() & 1; // 0 or 1

    ChangeScreen(START_SCREEN, false);

    WTTPlayer[0].ResetScore();
    WTTPlayer[1].ResetScore();
    TieGame = 0;

    AIThinkLoop = 0;

    Clear();
}

/**
 * Print the text with multiple lines if needed.
 * @param[in] x Specifies the x-coordinate of the upper-left corner of the text.
 * @param[in] y Specifies the y-coordinate of the upper-left corner of the text.
 * @param[in] maxLineWidth Maximum width of the string.
 * @param[in] input Text to draw.
 * @param[in] fontSize Size of the text.
 * @param[in] TextColor Text color in RGBA format. The alpha channel is used to change the opacity of the text.
 * @param[in] ShadowColor Shadow color in RGBA format. The alpha channel is used to change the opacity of the text.
 * @param[in] OffsetX Shadow offset for the x-coordinate.
 * @param[in] OffsetY Shadow offset for the y-coordinate.
 */
void Game::PrintWrapText(u16 x, u16 y, u16 maxLineWidth,
    const std::string &input, u32 fontSize, u32 TextColor,
    u32 ShadowColor, s8 OffsetX, s8 OffsetY)
{
    std::string tmp = input + " "; // Make local copy
    std::string tmp2;
    std::string::iterator startIndex = tmp.begin(),
                           lastSpace = tmp.begin(),
                           i = tmp.begin();
    int ypos = y,
        z = 0,
        textLeft;
    const int stepSize = (fontSize * 1.2);

    while(i != tmp.end())
    {
        if(*i == L' ')
        {
            z = GRRLIB_WidthTTF(DefaultFont, tmp2.assign(startIndex, i).c_str(), fontSize);

            if(z >= maxLineWidth)
            {
                *lastSpace = 0;
                textLeft = x + (maxLineWidth / 2.0) -
                    (GRRLIB_WidthTTF(DefaultFont, &(*startIndex), fontSize) / 2.0);
                GRRLIB_PrintfTTF(textLeft + OffsetX, ypos + OffsetY, DefaultFont, &(*startIndex),
                    fontSize, ShadowColor);
                GRRLIB_PrintfTTF(textLeft, ypos, DefaultFont, &(*startIndex),
                    fontSize, TextColor);
                startIndex = lastSpace + 1;
                ypos += stepSize;
                z = 0;
            }
            lastSpace = i;
        }
        ++i;
    }
    if(z <= maxLineWidth)
    {
        textLeft = x + (maxLineWidth / 2.0) -
            (GRRLIB_WidthTTF(DefaultFont, &(*startIndex), fontSize) / 2.0);
        GRRLIB_PrintfTTF(textLeft + OffsetX, ypos + OffsetY, DefaultFont, &(*startIndex),
            fontSize, ShadowColor);
        GRRLIB_PrintfTTF(textLeft, ypos, DefaultFont, &(*startIndex),
            fontSize, TextColor);
    }
}

/**
 * Change the screen.
 * @param[in] NewScreen New screen to show.
 * @param[in] PlaySound Set to true to play a sound. Default value is true.
 */
void Game::ChangeScreen(u8 NewScreen, bool PlaySound)
{
    if(PlaySound == true)
    {
        GameAudio->PlaySoundScreenChange(100);
    }

    SelectedButton = -1;
    LastScreen = CurrentScreen;
    CurrentScreen = NewScreen;

    Copied = false;
    ChangeCursor();
}

/**
 * Rumble if on button.
 * @param[in] NewSelectedButton New button to select.
 */
void Game::ButtonOn(s8 NewSelectedButton)
{
    if(SelectedButton != NewSelectedButton)
    {
        GameAudio->PlaySoundButton(80);
        RUMBLE_Wiimote(WPAD_CHAN_0, 50); // 50 ms
    }
}

/**
 * Check if a zone is selected.
 * @return true if in a zone, false otherwise
 */
bool Game::SelectZone()
{
    u8 HandID = 0;
    if(GameMode == modeVsHuman2 && CurrentPlayer == 1)
    {
        HandID = 1;
    }
    if(!RoundFinished && AIThinkLoop == 0)
    {
        for(int x = 0; x < 3; ++x)
        {
            for(int y = 0; y < 3; ++y)
            {
                if (Hand[HandID].GetLeft() > Table[x][y].GetX() &&
                    Hand[HandID].GetLeft() < (Table[x][y].GetX() + 136) &&
                    Hand[HandID].GetTop() > Table[x][y].GetY() &&
                    Hand[HandID].GetTop() < (Table[x][y].GetY() + 100))
                {
                    if(HandX != x || HandY != y)
                    {
                        HandX = x;
                        HandY = y;
                        if(GameGrid->GetPlayerAtPos(HandX, HandY) == ' ')
                        {   // Zone is empty
                            GameAudio->PlaySoundButton(90);
                            RUMBLE_Wiimote(HandID, 30);  // 30 ms
                        }
                    }
                    return true;
                }
            }
        }
    }
    HandX = -1;
    HandY = -1;
    return false;
}

/**
 * Change the cursor.
 */
void Game::ChangeCursor()
{
    if(CurrentScreen == HOME_SCREEN)
    {
        Hand[0].SetPlayer(curP1);
        Hand[0].SetAlpha(0xFF);
        Hand[1].SetPlayer(curP2);
        Hand[1].SetAlpha(0x55);
    }
    else if(CurrentScreen == GAME_SCREEN)
    {
        if(GameMode == modeVsHuman1)
        {
            if(WTTPlayer[CurrentPlayer].GetSign() == 'O')
            {
                Hand[0].SetPlayer(curO);
            }
            else if(WTTPlayer[CurrentPlayer].GetSign() == 'X')
            {
                Hand[0].SetPlayer(curX);
            }
            Hand[0].SetAlpha(0xFF);
        }
        else if(GameMode == modeVsHuman2)
        {
            Hand[0].SetPlayer(curX);
            Hand[1].SetPlayer(curO);
            if(RoundFinished)
            {
                Hand[0].SetAlpha(0xFF);
                Hand[1].SetAlpha(0xFF);
            }
            else
            {
                Hand[CurrentPlayer].SetAlpha(0xFF);
                Hand[!CurrentPlayer].SetAlpha(0x55);
            }
        }
        else
        {   // modeVsAI
            Hand[0].SetPlayer(curX);
            if(CurrentPlayer == 0 || RoundFinished)
            {
                Hand[0].SetAlpha(0xFF);
                Hand[1].SetAlpha(0x55);
            }
            else
            {
                Hand[0].SetAlpha(0x55);
                Hand[1].SetAlpha(0xFF);
            }
        }
    }
    else
    {   //START_SCREEN or MENU_SCREEN
        Hand[0].SetPlayer(curX);
        Hand[0].SetAlpha(0xFF);
        Hand[1].SetPlayer(curO);
        Hand[1].SetAlpha(0x55);
    }
}

/**
 * This function calculates the number of frames we render each second.
 */
void Game::CalculateFrameRate()
{
    static u8 frameCount = 0;
    static u32 lastTime;
    u32 currentTime = ticks_to_millisecs(gettime());

    ++frameCount;
    if(currentTime - lastTime > 1000)
    {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
}
