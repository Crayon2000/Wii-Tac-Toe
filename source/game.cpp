#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/conf.h>
#include <asndlib.h>
#include "grrlib/GRRLIB.h"
#include "tools.h"
#include "symbol.h"
#include "game.h"

// Graphics
#include "../gfx/splash.h"
#include "../gfx/backg.h"
#include "../gfx/hover.h"

// Audio
#include "../audio/button_rollover.h"

#define BUTTON_VOICE    0

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

    GameGrid = new Grid();
    Hand = new Cursor[2];
    Lang = new Language();

    Hand[0].SetVisible(false);
    Hand[1].SetVisible(false);

    ExitButton1 = new Button(btnHome);
    ExitButton1->SetLeft(430);
    ExitButton1->SetTop(20);
    ExitButton1->SetTextHeight(20);
    ExitButton1->SetCaption(Lang->Text("Close"));

    ExitButton2 = new Button(btnHomeMenu);
    ExitButton2->SetLeft((ScreenWidth / 2) + 20);
    ExitButton2->SetTop(165);
    ExitButton2->SetCaption(Lang->Text("Reset"));

    ExitButton3 = new Button(btnHomeMenu);
    ExitButton3->SetLeft((ScreenWidth / 2) - ExitButton2->GetWidth() - 20);
    ExitButton3->SetTop(165);
    //if(!!*(u32 *)0x80001800) // that returns true for hbc, false for load-from-trucha-signed-disc. think it also returns false for tp hack
    //    ExitButton3->SetCaption("Return to HBC");
    //else
        ExitButton3->SetCaption(Lang->Text("Return to Loader"));

    MenuButton = new Button[2];

    MenuButton[0].SetLeft((ScreenWidth / 2) - (MenuButton[0].GetWidth() / 2));
    MenuButton[0].SetTop(150);
    MenuButton[0].SetCaption(Lang->Text("2 Players (1 Wiimote)"));

    MenuButton[1].SetLeft((ScreenWidth / 2) - (MenuButton[1].GetWidth() / 2));
    MenuButton[1].SetTop(250);
    MenuButton[1].SetCaption(Lang->Text("1 Player (Vs AI)"));

    WTTPlayer = new Player[2];
    WTTPlayer[0].SetSign('O');
    WTTPlayer[0].SetName(Lang->Text("PLAYER 1"));
    WTTPlayer[1].SetSign('X');
    WTTPlayer[1].SetName(Lang->Text("PLAYER 2"));

    GameImg = GRRLIB_LoadTexture(backg);
    SplashImg = GRRLIB_LoadTexture(splash);
    HoverImg = GRRLIB_LoadTexture(hover);
    CopiedImg = NULL;

    RUMBLE_Init();
    ASND_Init();
    ASND_Pause(0);
    NewGame();
}

/**
 * Destructor for the Game class.
 */
Game::~Game()
{
    GRRLIB_FreeTexture(GameImg);
    GRRLIB_FreeTexture(SplashImg);
    GRRLIB_FreeTexture(HoverImg);
    FreeMemImg();

    delete GameGrid;
    delete[] Hand;
    delete Lang;
    delete ExitButton1;
    delete ExitButton2;
    delete ExitButton3;
    delete[] WTTPlayer;
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
}

/**
 * Draw the start screen.
 */
void Game::StartSreen()
{
    if(CopiedImg == NULL)
    {   // Copy static element
        GRRLIB_initTexture();   // Init text layer
        GRRLIB_DrawImg(0, 0, SplashImg, 0, 1, 1, 0xFFFFFFFF);

        char TempText[TEXT_SIZE];
        sprintf(TempText, Lang->Text("Programmer: %s"), "Crayon");
        GRRLIB_Printf2(50, 310, TempText, 11, 0xFFFFFF);
        sprintf(TempText, Lang->Text("Graphics: %s"), "Mr_Nick666");
        GRRLIB_Printf2(50, 330, TempText, 11, 0xFFFFFF);

        strncpy(TempText, Lang->Text("Press The A Button"), TEXT_SIZE);
        int TextLeft = 320 - (GRRLIB_TextWidth(TempText, 20) / 2);
        GRRLIB_Printf2(TextLeft, 400, TempText, 20, 0x000000);

        GRRLIB_DrawImg(0, 0, GRRLIB_GetTexture(), 0, 1.0, 1.0, 0xFFFFFFFF);
        CopiedImg = GRRLIB_Screen2Texture();
    }
    else
    {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
    }
}

/**
 * Draw the game screen.
 * @param[in] CopyScreen If true the screen is copied to a buffer.
 */
void Game::GameScreen(bool CopyScreen)
{
    int TextLeft;

    if(CopiedImg == NULL)
    {   // Copy static element
        GRRLIB_initTexture();   // Init text layer
        // Background image
        GRRLIB_DrawImg(0, 0, GameImg, 0, 1, 1, 0xFFFFFFFF);

        // Player name: Offset -2, 2
        PrintWrapText(42, 50, 125, WTTPlayer[0].GetName(), 0xE6313A, 15);
        PrintWrapText(42, 145, 125, WTTPlayer[1].GetName(), 0x6BB6DE, 15);
        PrintWrapText(42, 250, 125, Lang->Text("TIE GAME"), 0x109642, 15);

        // Draw score: Offset -2, 2
        char ScoreText[5];
        snprintf(ScoreText, 5, "%d", WTTPlayer[0].GetScore());
        TextLeft = 104 - GRRLIB_TextWidth(ScoreText, 35) / 2;
        GRRLIB_Printf2(TextLeft, 77, ScoreText, 35, 0xE6313A);
        snprintf(ScoreText, 5, "%d", WTTPlayer[1].GetScore());
        TextLeft = 104 - GRRLIB_TextWidth(ScoreText, 35) / 2;
        GRRLIB_Printf2(TextLeft, 177, ScoreText, 35, 0x6BB6DE);
        snprintf(ScoreText, 5, "%d", TieGame);
        TextLeft = 104 - GRRLIB_TextWidth(ScoreText, 35) / 2;
        GRRLIB_Printf2(TextLeft, 282, ScoreText, 35, 0x109642);

        // Draw text at the bottom: Offet 1, 1
        PrintWrapText(131, 421, 390, text, 0x111111, 15);

        // Draw grid content
        Symbol *Sign = new Symbol;
        u8 x, y;
        for(x = 0; x < 3; x++)
        {
            for(y = 0; y < 3; y++)
            {
                Sign->SetPlayer(GameGrid->GetPlayerAtPos(x, y));
                Sign->SetLocation(Table[x][y]);
                Sign->Paint();
            }
        }
        delete Sign;

        // Draw text shadow
        GRRLIB_DrawImg(0, 0, GRRLIB_GetTexture(), 0, 1.0, 1.0, 0xFFFFFFF0);
        GRRLIB_initTexture();

        // Player name
        PrintWrapText(44, 48, 125, WTTPlayer[0].GetName(), 0xFFFFFF, 15);
        PrintWrapText(44, 143, 125, WTTPlayer[1].GetName(), 0xFFFFFF, 15);
        PrintWrapText(44, 248, 125, Lang->Text("TIE GAME"), 0xFFFFFF, 15);

        // Draw score
        snprintf(ScoreText, 5, "%d", WTTPlayer[0].GetScore());
        TextLeft = 106 - GRRLIB_TextWidth(ScoreText, 35) / 2;
        GRRLIB_Printf2(TextLeft, 75, ScoreText, 35, 0xFFFFFF);
        snprintf(ScoreText, 5, "%d", WTTPlayer[1].GetScore());
        TextLeft = 106 - GRRLIB_TextWidth(ScoreText, 35) / 2;
        GRRLIB_Printf2(TextLeft, 175, ScoreText, 35, 0xFFFFFF);
        snprintf(ScoreText, 5, "%d", TieGame);
        TextLeft = 106 - GRRLIB_TextWidth(ScoreText, 35) / 2;
        GRRLIB_Printf2(TextLeft, 280, ScoreText, 35, 0xFFFFFF);

        // Draw text at the bottom
        PrintWrapText(130, 420, 390, text, 0x8C8A8C, 15);

        GRRLIB_DrawImg(0, 0, GRRLIB_GetTexture(), 0, 1.0, 1.0, 0xFFFFFFFF);
        if(CopyScreen)
        {
            CopiedImg = GRRLIB_Screen2Texture();
        }
    }
    if(CopyScreen)
    {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
    }

    if(SelectZone() && GameGrid->GetPlayerAtPos(HandX, HandY) == ' ')
    {
        if(WTTPlayer[CurrentPlayer].GetSign() == 'X')
        {
            GRRLIB_DrawImg(Table[HandX][HandY].GetX(), Table[HandX][HandY].GetY(),
                HoverImg, 0, 1, 1, 0x0093DDFF);
        }
        else
        {
            GRRLIB_DrawImg(Table[HandX][HandY].GetX(), Table[HandX][HandY].GetY(),
                HoverImg, 0, 1, 1, 0xDA251DFF);
        }
    }
}

/**
 * Draw the HOME screen.
 */
void Game::ExitScreen()
{
    if(CopiedImg == NULL)
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

        CopiedImg = GRRLIB_Screen2Texture();
    }
    else
    {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
    }

    if(GRRLIB_PtInRect(0, 0, ScreenWidth, 78, Hand[0].GetLeft(), Hand[0].GetTop()))
        GRRLIB_Rectangle(0, 0, ScreenWidth, 78, 0x30B6EBFF, 1);
    else
        GRRLIB_Rectangle(0, 0, ScreenWidth, 78, 0x000000FF, 1);

    GRRLIB_initTexture();   // Init text layer

    GRRLIB_Printf2(30, 20, Lang->Text("HOME Menu"), 30, 0xFFFFFF);

    ExitButton1->SetSelected(false);
    ExitButton2->SetSelected(false);
    ExitButton3->SetSelected(false);
    if(GRRLIB_PtInRect(0, 0, ScreenWidth, 78, Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton1->SetSelected(true);
        ButtonOn(0);
        SelectedButton = 0;
    }
    else if(ExitButton2->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton2->SetSelected(true);
        ButtonOn(1);
        SelectedButton = 1;
    }
    else if(ExitButton3->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton3->SetSelected(true);
        ButtonOn(2);
        SelectedButton = 2;
    }
    else
    {
        SelectedButton = -1;
    }
    ExitButton1->Paint();
    ExitButton2->Paint();
    ExitButton3->Paint();
    GRRLIB_DrawImg(0, 0, GRRLIB_GetTexture(), 0, 1.0, 1.0, 0xFFFFFFFF);
}

/**
 * Draw the menu screen.
 * @param[in] CopyScreen If true the screen is copied to a buffer.
 */
void Game::MenuScreen(bool CopyScreen)
{
    if(CopiedImg == NULL)
    {   // Copy static element
        GRRLIB_initTexture();   // Init text layer
        GRRLIB_FillScreen(0x000000FF);  // Clear screen
        for(int y = 0; y <=ScreenHeight; y+=8)
        {
            GRRLIB_Rectangle(0, y, ScreenWidth, 2, 0xB0B0B030, 1);
        }

        GRRLIB_Rectangle(0, 0, ScreenWidth, 63, 0x000000FF, 1);
        GRRLIB_Rectangle(0, 63, ScreenWidth, 2, 0xFFFFFFFF, 1);

        GRRLIB_Rectangle(0, 383, ScreenWidth, 2, 0xFFFFFFFF, 1);
        GRRLIB_Rectangle(0, 385, ScreenWidth, 95, 0x000000FF, 1);

        char VersionText[TEXT_SIZE] = "";
        sprintf(VersionText, Lang->Text("Ver. %s"), "0.6");
        GRRLIB_Printf2(500, 40, VersionText, 12, 0xFFFFFF);

        GRRLIB_DrawImg(0, 0, GRRLIB_GetTexture(), 0, 1.0, 1.0, 0xFFFFFFFF);
        if(CopyScreen)
        {
            CopiedImg = GRRLIB_Screen2Texture();
        }
    }
    else
    {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
    }

    GRRLIB_initTexture();   // Init text layer

    MenuButton[0].SetSelected(false);
    MenuButton[1].SetSelected(false);
    if(MenuButton[0].IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        MenuButton[0].SetSelected(true);
        ButtonOn(0);
        SelectedButton = 0;
    }
    else if(MenuButton[1].IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        MenuButton[1].SetSelected(true);
        ButtonOn(1);
        SelectedButton = 1;
    }
    else
    {
        SelectedButton = -1;
    }
    MenuButton[0].Paint();
    MenuButton[1].Paint();
    GRRLIB_DrawImg(0, 0, GRRLIB_GetTexture(), 0, 1.0, 1.0, 0xFFFFFFFF);
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
                    ChangeScreen(LastScreen);
                }
                else if((Buttons0 & WPAD_BUTTON_A))
                {
                    switch(SelectedButton)
                    {
                        case 0:
                            ChangeScreen(LastScreen);
                            break;
                        case 1:
                            NewGame();
                            break;
                        case 2:
                            ExitScreen();
                            Hand[1].Paint();
                            Hand[0].Paint();
                            if(CopiedImg)
                                GRRLIB_FreeTexture(CopiedImg);
                            CopiedImg = GRRLIB_Screen2Texture();
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
        if(GRRLIB_ScrShot("sd:/Screenshot.png"))
            strncpy(text, "A screenshot was taken!!!", TEXT_SIZE);
        else
            strncpy(text, "Screenshot did not work!!!", TEXT_SIZE);
        WIILIGHT_TurnOff();
        WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off
        WPAD_Rumble(WPAD_CHAN_1, 0); // Rumble off
        FreeMemImg();
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
    snprintf(text, TEXT_SIZE, Lang->GetRandomTurnOverMessage(), WTTPlayer[CurrentPlayer].GetName());
    RoundFinished = false;
    FreeMemImg();
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
        string TextToCopy;
        strncpy(text, Lang->GetRandomWinningMessage(), TEXT_SIZE);
        TextToCopy = str_replace(text, "$LOSER$", WTTPlayer[!GameWinner].GetName());
        TextToCopy = str_replace(TextToCopy, "$WINNER$", WTTPlayer[GameWinner].GetName());
        strncpy(text, TextToCopy.c_str(), TEXT_SIZE);
        RoundFinished = true;
    }
    else if(GameGrid->IsFilled())
    {   // Tie game
        TieGame++;
        strncpy(text, Lang->GetRandomTieMessage(), TEXT_SIZE);
        RoundFinished = true;
    }
    else
    {
        CurrentPlayer = !CurrentPlayer; // Change player's turn
        snprintf(text, TEXT_SIZE, Lang->GetRandomTurnOverMessage(), WTTPlayer[CurrentPlayer].GetName());
    }

    FreeMemImg();
    ChangeCursor();
}

/**
 * Free the texture.
 */
void Game::FreeMemImg()
{
    if(CopiedImg != NULL)
    {
        GRRLIB_FreeTexture(CopiedImg);
        CopiedImg = NULL;
    }
}

/**
 * Start a new game, initialize variables.
 */
void Game::NewGame()
{
    PlayerToStart = rand() & 1; // 0 or 1

    ChangeScreen(START_SCREEN);

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
 * @param[in] TextColor Text color in RGBA format. The alpha channel is used to change the opacity of the text.
 * @param[in] fontSize Size of the text.
 */
void Game::PrintWrapText(u16 x, u16 y, u16 maxLineWidth,
    const char *input, unsigned int TextColor, unsigned int fontSize)
{
    char tmp[TEXT_SIZE], tmp2[TEXT_SIZE];
    int ypos = y,
        i,
        z = 0,
        textLeft,
        startIndex = 0,
        lastSpace = 0,
        stepSize = (fontSize * 1.2);
    int endIndex = strlen(input) + 1;

    // Make local copy
    strncpy(tmp, input, TEXT_SIZE);

    for(i=0; i<endIndex; i++)
    {
        if(tmp[i] == ' ' || tmp[i] == '\0')
        {
            strncpy(tmp2, input+startIndex, TEXT_SIZE);
            tmp2[i] = 0;
            z = GRRLIB_TextWidth(tmp2, fontSize);

            if(z >= maxLineWidth)
            {
                tmp[lastSpace] = 0;
                textLeft = x + (maxLineWidth / 2.0) -
                    (GRRLIB_TextWidth(tmp+startIndex, fontSize) / 2.0);
                GRRLIB_Printf2(textLeft, ypos, tmp+startIndex,
                    fontSize, TextColor);
                startIndex = lastSpace + 1;
                ypos += stepSize;
                z = 0;
            }
            lastSpace = i;
        }
    }
    if(z <= maxLineWidth)
    {
        textLeft = x + (maxLineWidth / 2.0) -
            (GRRLIB_TextWidth(tmp+startIndex, fontSize) / 2.0);
        GRRLIB_Printf2(textLeft, ypos, tmp+startIndex,
            fontSize, TextColor);
    }
}

/**
 * Change the screen.
 * @param[in] NewScreen New screen to show.
 */
void Game::ChangeScreen(u8 NewScreen)
{
    SelectedButton = -1;
    LastScreen = CurrentScreen;
    CurrentScreen = NewScreen;

    FreeMemImg();
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
        ASND_SetVoice(BUTTON_VOICE, VOICE_MONO_16BIT, 44100, 0, (void *)button_rollover, button_rollover_size, 32, 32, NULL);
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
                            ASND_SetVoice(BUTTON_VOICE, VOICE_MONO_16BIT, 44100, 0, (void *)button_rollover,
                                button_rollover_size, 32, 32, NULL);
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
            Hand[0].SetPlayer(curO);
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
        Hand[0].SetPlayer(curO);
        Hand[0].SetAlpha(0xFF);
        Hand[1].SetPlayer(curX);
        Hand[1].SetAlpha(0xFF);
    }
    Hand[1].SetAlpha(0x55);
}
