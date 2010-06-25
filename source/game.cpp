#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <ogc/conf.h>
#include <asndlib.h>
#include <ogc/lwp_watchdog.h>
#include "tools.h"
#include "game.h"

// Graphics
#include "../gfx/splash.h"
#include "../gfx/backg.h"
#include "../gfx/hover.h"

// Audio
#include "../audio/button_rollover.h"
#include "../audio/screen_change.h"
#include "../audio/tic-tac.h"

// Font
#include "../fonts/Swis721_Ex_BT.h"

#define MODPLAYER_VOICE 0
#define BUTTON_VOICE    1
#define SCREEN_VOICE    2

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
 */
Game::Game(u16 GameScreenWidth, u16 GameScreenHeight)
{
    srand(time(NULL));

    ScreenWidth = GameScreenWidth;
    ScreenHeight = GameScreenHeight;

    FPS = 0;
    ShowFPS = false;

    GameGrid = new Grid();
    Hand = new Cursor[2];
    Lang = new Language();

    DefaultFont = GRRLIB_LoadTTF(Swis721_Ex_BT, Swis721_Ex_BT_size);

    u8 x, y;
    GridSign = new Symbol*[3];
    for(x = 0; x < 3; x++)
    {
        GridSign[x] = new Symbol[3];
        for(y = 0; y < 3; y++)
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
    MenuButton[0]->SetTop(150);
    MenuButton[0]->SetCaption(Lang->String("2 Players (1 Wiimote)"));

    MenuButton.push_back(new Button());
    MenuButton[1]->SetFont(DefaultFont);
    MenuButton[1]->SetLeft((ScreenWidth / 2) - (MenuButton[1]->GetWidth() / 2));
    MenuButton[1]->SetTop(250);
    MenuButton[1]->SetCaption(Lang->String("1 Player (Vs AI)"));

    WTTPlayer = new Player[2];
    WTTPlayer[0].SetSign('X');
    WTTPlayer[0].SetName(Lang->String("PLAYER 1"));
    WTTPlayer[1].SetSign('O');
    WTTPlayer[1].SetName(Lang->String("PLAYER 2"));

    GameImg = GRRLIB_LoadTexture(backg);
    SplashImg = GRRLIB_LoadTexture(splash);
    HoverImg = GRRLIB_LoadTexture(hover);
    CopiedImg = GRRLIB_CreateEmptyTexture(ScreenWidth, ScreenHeight);

    RUMBLE_Init();
    ASND_Init();
    ASND_Pause(0);
    NewGame();

    // Build Start Screen background
    GRRLIB_DrawImg(0, 0, SplashImg, 0, 1.0, 1.0, 0xFFFFFFFF);
    swprintf(text, TEXT_SIZE, Lang->String("Programmer: %ls").c_str(), L"Crayon");
    GRRLIB_PrintfTTFW(50, 310, DefaultFont, text, 11, 0xFFFFFFFF);
    swprintf(text, TEXT_SIZE, Lang->String("Graphics: %ls").c_str(), L"Mr_Nick666");
    GRRLIB_PrintfTTFW(50, 330, DefaultFont, text, 11, 0xFFFFFFFF);
    wcsncpy(text, Lang->String("Press The A Button").c_str(), TEXT_SIZE);
    GRRLIB_PrintfTTFW((ScreenWidth / 2) - (GRRLIB_WidthTTFW(DefaultFont, text, 20) / 2),
                    400, DefaultFont, text, 20, 0x000000FF);
    GRRLIB_Screen2Texture(0, 0, SplashImg, true);
}

/**
 * Destructor for the Game class.
 */
Game::~Game()
{
    GRRLIB_FreeTexture(GameImg);
    GRRLIB_FreeTexture(SplashImg);
    GRRLIB_FreeTexture(HoverImg);
    GRRLIB_FreeTexture(CopiedImg);
    GRRLIB_FreeTTF(DefaultFont);

    delete GameGrid;
    delete[] Hand;
    delete Lang;
    delete[] WTTPlayer;

    for(u8 i=0; i<ExitButton.size(); i++)
    {
        delete ExitButton[i];
    }
    ExitButton.clear();

    for(u8 i=0; i<MenuButton.size(); i++)
    {
        delete MenuButton[i];
    }
    MenuButton.clear();

    for(u8 i = 0; i < 3; i++)
    {
        delete[] GridSign[i];
    }
    delete[] GridSign;

    MODPlay_Unload(&ModTrack);
    ASND_Pause(1);
    ASND_End();
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
                    AIThinkLoop++;
                }
            }
            break;
        default:
            GRRLIB_FillScreen(0x000000FF);
    }

    if(CurrentScreen != START_SCREEN &&
        WPAD_Probe(WPAD_CHAN_0, NULL) == WPAD_ERR_NO_CONTROLLER)
    {   // Controller is disconnected
        GRRLIB_Rectangle(0, 0, ScreenWidth, ScreenHeight, 0x000000B2, 1);
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
    GRRLIB_DrawImg(0, 0, SplashImg, 0, 1, 1, 0xFFFFFFFF);
}

/**
 * Draw the game screen.
 * @param[in] CopyScreen If true the screen is copied to a buffer.
 */
void Game::GameScreen(bool CopyScreen)
{
    int TextLeft;

    if(!Copied)
    {   // Copy static element
        // Background image
        GRRLIB_DrawImg(0, 0, GameImg, 0, 1, 1, 0xFFFFFFFF);

        // Player name with a shadow offset of -2, 2
        PrintWrapText(44, 48, 125, WTTPlayer[0].GetName(), 15, 0xFFFFFFFF, 0x6BB6DEFF, -2, 2);
        PrintWrapText(44, 143, 125, WTTPlayer[1].GetName(), 15, 0xFFFFFFFF, 0xE6313AFF, -2, 2);
        PrintWrapText(44, 248, 125, Lang->String("TIE GAME"), 15, 0xFFFFFFFF, 0x109642FF, -2, 2);

        // Draw score with a shadow offset of -2, 2
        wchar_t ScoreText[5];
        swprintf(ScoreText, 5, L"%d", WTTPlayer[0].GetScore());
        TextLeft = 104 - GRRLIB_WidthTTFW(DefaultFont, ScoreText, 35) / 2;
        GRRLIB_PrintfTTFW(TextLeft, 77, DefaultFont, ScoreText, 35, 0x6BB6DEFF);
        GRRLIB_PrintfTTFW(TextLeft - 2, 75, DefaultFont, ScoreText, 35, 0xFFFFFFFF);

        swprintf(ScoreText, 5, L"%d", WTTPlayer[1].GetScore());
        TextLeft = 104 - GRRLIB_WidthTTFW(DefaultFont, ScoreText, 35) / 2;
        GRRLIB_PrintfTTFW(TextLeft, 177, DefaultFont, ScoreText, 35, 0xE6313AFF);
        GRRLIB_PrintfTTFW(TextLeft - 2, 175, DefaultFont, ScoreText, 35, 0xFFFFFFFF);

        swprintf(ScoreText, 5, L"%d", TieGame);
        TextLeft = 104 - GRRLIB_WidthTTFW(DefaultFont, ScoreText, 35) / 2;
        GRRLIB_PrintfTTFW(TextLeft, 282, DefaultFont, ScoreText, 35, 0x109642FF);
        GRRLIB_PrintfTTFW(TextLeft - 2, 280, DefaultFont, ScoreText, 35, 0xFFFFFFFF);

        // Draw text at the bottom with a shadow offset of 1, 1
        PrintWrapText(130, 420, 390, text, 15, 0x8C8A8CFF, 0x111111FF, 1, 1);

        if(CopyScreen)
        {
            GRRLIB_Screen2Texture(0, 0, CopiedImg, false);
            Copied = true;
        }
    }
    if(CopyScreen)
    {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
    }

    u32 HoverColor = (WTTPlayer[CurrentPlayer].GetSign() == 'X') ? 0x0093DDFF : 0xDA251DFF;

    // Draw grid content
    if(RoundFinished)
    {
        if(AlphaDirection)
        {
            SymbolAlpha += 2;
            if(SymbolAlpha > 128)
                AlphaDirection = !AlphaDirection;
        }
        else
        {
            SymbolAlpha -= 2;
            if(SymbolAlpha < 5)
                AlphaDirection = !AlphaDirection;
        }
    }
    u8 x, y;
    for(x = 0; x < 3; x++)
    {
        for(y = 0; y < 3; y++)
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
        GRRLIB_DrawImg(Table[HandX][HandY].GetX(), Table[HandX][HandY].GetY(),
            HoverImg, 0, 1, 1, HoverColor);
    }
}

/**
 * Draw the HOME screen.
 */
void Game::ExitScreen()
{
    MODPlay_Pause(&ModTrack, true);
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

        GRRLIB_Rectangle(0, 0, ScreenWidth, ScreenHeight, 0x000000CC, 1); // Draw a black rectangle over it

        GRRLIB_Rectangle(0, 78, ScreenWidth, 2, 0x848284FF, 1);

        GRRLIB_Rectangle(0, 383, ScreenWidth, 2, 0x848284FF, 1);
        GRRLIB_Rectangle(0, 385, ScreenWidth, 95, 0x000000FF, 1);

        GRRLIB_Screen2Texture(0, 0, CopiedImg, false);
        Copied = true;
    }
    else
    {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
    }

    if(GRRLIB_PtInRect(0, 0, ScreenWidth, 78, Hand[0].GetLeft(), Hand[0].GetTop()))
        GRRLIB_Rectangle(0, 0, ScreenWidth, 78, 0x30B6EBFF, 1);
    else
        GRRLIB_Rectangle(0, 0, ScreenWidth, 78, 0x000000FF, 1);

    GRRLIB_PrintfTTFW(30, 20, DefaultFont, Lang->String("HOME Menu").c_str(), 30, 0xFFFFFFFF);

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
        GRRLIB_FillScreen(0x000000FF);  // Clear screen
        for(int y = 0; y <=ScreenHeight; y+=8)
        {
            GRRLIB_Rectangle(0, y, ScreenWidth, 2, 0xB0B0B030, 1);
        }

        GRRLIB_Rectangle(0, 0, ScreenWidth, 63, 0x000000FF, 1);
        GRRLIB_Rectangle(0, 63, ScreenWidth, 2, 0xFFFFFFFF, 1);

        GRRLIB_Rectangle(0, 383, ScreenWidth, 2, 0xFFFFFFFF, 1);
        GRRLIB_Rectangle(0, 385, ScreenWidth, 95, 0x000000FF, 1);

        wchar_t VersionText[TEXT_SIZE] = L"";
        swprintf(VersionText, TEXT_SIZE, Lang->String("Ver. %ls").c_str(), L"0.8");
        GRRLIB_PrintfTTFW(500, 40, DefaultFont, VersionText, 12, 0xFFFFFFFF);

        if(CopyScreen)
        {
            GRRLIB_Screen2Texture(0, 0, CopiedImg, false);
            Copied = true;
        }
    }
    else
    {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
    }

    MenuButton[0]->SetSelected(false);
    MenuButton[1]->SetSelected(false);
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
    else
    {
        SelectedButton = -1;
    }
    MenuButton[0]->Paint();
    MenuButton[1]->Paint();
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
    unsigned int Buttons0 = WPAD_ButtonsDown(WPAD_CHAN_0);
    unsigned int Buttons1 = WPAD_ButtonsDown(WPAD_CHAN_1);

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
    if(WPadData1->ir.valid && CurrentScreen == HOME_SCREEN)
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
                    MODPlay_Pause(&ModTrack, false);
                    ChangeScreen(LastScreen);
                }
                else if((Buttons0 & WPAD_BUTTON_A))
                {
                    switch(SelectedButton)
                    {
                        case 0:
                            MODPlay_Pause(&ModTrack, false);
                            ChangeScreen(LastScreen);
                            break;
                        case 1:
                            NewGame();
                            break;
                        case 2:
                            ExitScreen();
                            Hand[1].Paint();
                            Hand[0].Paint();
                            GRRLIB_Screen2Texture(0, 0, CopiedImg, false);
                            WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off, just in case
                            WPAD_Rumble(WPAD_CHAN_1, 0); // Rumble off, just in case
                            GRRLIB_DrawImg_FadeOut(CopiedImg, 1, 1, 3);
                            return true; // Exit to loader
                    }
                }
                break;
            default:
                if((Buttons0 & WPAD_BUTTON_A))
                {
                    if(RoundFinished)
                    {
                        Clear();
                    }
                    else if(GameGrid->SetPlayer(WTTPlayer[CurrentPlayer].GetSign(), HandX, HandY))
                    {
                        TurnIsOver();
                    }
                    else
                    {   // Position is invalid
                        RUMBLE_Wiimote(WPAD_CHAN_0, 200);  // 200 ms
                    }
                }
                else if((Buttons0 & WPAD_BUTTON_HOME) || (Buttons1 & WPAD_BUTTON_HOME))
                {
                    ChangeScreen(HOME_SCREEN);
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

        if(GRRLIB_ScrShot(path))
            wcsncpy(text, L"A screenshot was taken!!!", TEXT_SIZE);
        else
            wcsncpy(text, L"Screenshot did not work!!!", TEXT_SIZE);

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
    swprintf(text, TEXT_SIZE, Lang->GetRandomTurnOverMessage().c_str(), WTTPlayer[CurrentPlayer].GetName().c_str());
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
        wstring TextToCopy;
        wcsncpy(text, Lang->GetRandomWinningMessage().c_str(), TEXT_SIZE);
        TextToCopy = str_replaceW(text, L"$LOSER$", WTTPlayer[!GameWinner].GetName());
        TextToCopy = str_replaceW(TextToCopy, L"$WINNER$", WTTPlayer[GameWinner].GetName());
        wcsncpy(text, TextToCopy.c_str(), TEXT_SIZE);
        RoundFinished = true;
        SymbolAlpha = 5;
        AlphaDirection = 0;
    }
    else if(GameGrid->IsFilled())
    {   // Tie game
        TieGame++;
        wcsncpy(text, Lang->GetRandomTieMessage().c_str(), TEXT_SIZE);
        RoundFinished = true;
    }
    else
    {
        CurrentPlayer = !CurrentPlayer; // Change player's turn
        swprintf(text, TEXT_SIZE, Lang->GetRandomTurnOverMessage().c_str(), WTTPlayer[CurrentPlayer].GetName().c_str());
    }

    Copied = false;
    ChangeCursor();
}

/**
 * Start a new game, initialize variables.
 */
void Game::NewGame()
{
    MODPlay_Init(&ModTrack);
    MODPlay_SetMOD(&ModTrack, tic_tac);
    MODPlay_SetVolume(&ModTrack, 48, 48); // Maximum volume is 64
    MODPlay_SetStereo(&ModTrack, true);
    MODPlay_Start(&ModTrack);

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
    const wstring &input, unsigned int fontSize, unsigned int TextColor,
    unsigned int ShadowColor, s8 OffsetX, s8 OffsetY)
{
    wstring tmp = input + L" ", // Make local copy
            tmp2;
    wstring::iterator startIndex = tmp.begin(),
                      lastSpace = tmp.begin(),
                      i = tmp.begin();
    int ypos = y,
        z = 0,
        textLeft,
        stepSize = (fontSize * 1.2);

    while(i != tmp.end())
    {
        if(*i == L' ')
        {
            tmp2.assign(startIndex, i);
            z = GRRLIB_WidthTTFW(DefaultFont, tmp2.c_str(), fontSize);

            if(z >= maxLineWidth)
            {
                *lastSpace = 0;
                textLeft = x + (maxLineWidth / 2.0) -
                    (GRRLIB_WidthTTFW(DefaultFont, &(*startIndex), fontSize) / 2.0);
                GRRLIB_PrintfTTFW(textLeft + OffsetX, ypos + OffsetY, DefaultFont, &(*startIndex),
                    fontSize, ShadowColor);
                GRRLIB_PrintfTTFW(textLeft, ypos, DefaultFont, &(*startIndex),
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
            (GRRLIB_WidthTTFW(DefaultFont, &(*startIndex), fontSize) / 2.0);
        GRRLIB_PrintfTTFW(textLeft + OffsetX, ypos + OffsetY, DefaultFont, &(*startIndex),
            fontSize, ShadowColor);
        GRRLIB_PrintfTTFW(textLeft, ypos, DefaultFont, &(*startIndex),
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
    if(PlaySound)
    {
        ASND_SetVoice(SCREEN_VOICE, VOICE_MONO_16BIT, 44100, 0,
            (void *)screen_change, screen_change_size, 100, 100, NULL);
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
        ASND_SetVoice(BUTTON_VOICE, VOICE_MONO_16BIT, 44100, 0,
            (void *)button_rollover, button_rollover_size, 80, 80, NULL);
        RUMBLE_Wiimote(WPAD_CHAN_0, 50); // 50 ms
    }
}

/**
 * Check if a zone is selected.
 * @return true if in a zone, false otherwize
 */
bool Game::SelectZone()
{
    if(!RoundFinished && AIThinkLoop == 0)
    {
        for(int x = 0; x < 3; x++)
        {
            for(int y = 0; y < 3; y++)
            {
                if (Hand[0].GetLeft() > Table[x][y].GetX() &&
                    Hand[0].GetLeft() < (Table[x][y].GetX() + 136) &&
                    Hand[0].GetTop() > Table[x][y].GetY() &&
                    Hand[0].GetTop() < (Table[x][y].GetY() + 100))
                {
                    if(HandX != x || HandY != y)
                    {
                        HandX = x;
                        HandY = y;
                        if(GameGrid->GetPlayerAtPos(HandX, HandY) == ' ')
                        {   // Zone is empty
                            ASND_SetVoice(BUTTON_VOICE, VOICE_MONO_16BIT, 44100, 0,
                                (void *)button_rollover, button_rollover_size, 90, 90, NULL);
                            RUMBLE_Wiimote(WPAD_CHAN_0, 30);  // 30 ms
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
        Hand[1].SetAlpha(0xFF);
    }
    else if(CurrentScreen == GAME_SCREEN)
    {
        if(GameMode == modeVsHuman1)
        {
            if(WTTPlayer[CurrentPlayer].GetSign() == 'O')
                Hand[0].SetPlayer(curO);
            else if(WTTPlayer[CurrentPlayer].GetSign() == 'X')
                Hand[0].SetPlayer(curX);
            Hand[0].SetAlpha(0xFF);
            Hand[1].SetVisible(false);
        }
        else
        {
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
    {
        Hand[0].SetPlayer(curX);
        Hand[0].SetAlpha(0xFF);
        Hand[1].SetPlayer(curO);
        Hand[1].SetAlpha(0xFF);
    }
    Hand[1].SetAlpha(0x55);
}

/**
 * This function calculates the number of frames we render each second.
 */
void Game::CalculateFrameRate() {
    static u8 frameCount = 0;
    static u32 lastTime;
    u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
}
