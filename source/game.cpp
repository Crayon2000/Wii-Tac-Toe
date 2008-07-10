#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
extern "C"
{
	#include "grrlib/GRRLIB.h"
}
#include "main.h"
#include "symbol.h"
#include "game.h"

// Fonts
#include "../fonts/button_text.h"
#include "../fonts/font1.h"
#include "../fonts/GRRLIB_font1.h"

// Graphics
#include "../gfx/splash.h"
#include "../gfx/backg.h"
#include "../gfx/hover.h"

#define START_SCREEN 	0
#define GAME_SCREEN 	1
#define HOME_SCREEN 	2
#define MENU_SCREEN 	3

#define IDS_PRESS_A  	"Press The A Button"

#define IDS_PLAYERTURN1 "%s, it's your turn."
#define IDS_PLAYERTURN2 "Waiting for %s."
#define IDS_PLAYERTURN3 "%s is thinking of is next move."
#define IDS_PLAYERTURN4 "%s should take the Wiimote and play."
#define IDS_PLAYERTURN5 "%s, let's play."
#define IDS_PLAYERTURN6 "%s should play now."
#define IDS_PLAYERWIN1  "%s won this round."
#define IDS_PLAYERWIN2  "Is %s the best?"
#define IDS_PLAYERWIN3  "The game is yours, %s."
#define IDS_PLAYERWIN4  "%s rules."
#define IDS_PLAYERLOSE1 "Too bad for %s."
#define IDS_PLAYERLOSE2 "Better luck next time, %s!"
#define IDS_PLAYERLOSE3 "%s you really suck."
#define IDS_PLAYERLOSE4 "%s should practice a little more."
#define IDS_PLAYERTIE1  "Tie game, that sucks."
#define IDS_PLAYERTIE2  "You both lose this one."
#define IDS_PLAYERTIE3  "No winner, no loser."
#define IDS_PLAYERTIE4  "You both wasted your time, it's a tie."

#define IDS_PLAYERTURN 	IDS_PLAYERTURN1, IDS_PLAYERTURN2, IDS_PLAYERTURN3, \
						IDS_PLAYERTURN4, IDS_PLAYERTURN5, IDS_PLAYERTURN6
#define IDS_PLAYERWIN  	IDS_PLAYERWIN1, IDS_PLAYERWIN2, IDS_PLAYERWIN3, \
						IDS_PLAYERWIN3
#define IDS_PLAYERLOSE  IDS_PLAYERLOSE1, IDS_PLAYERLOSE2, IDS_PLAYERLOSE3, \
						IDS_PLAYERLOSE4
#define IDS_PLAYERTIE  	IDS_PLAYERTIE1, IDS_PLAYERTIE2, IDS_PLAYERTIE3, IDS_PLAYERTIE4

static Point Table[3][3] = {
	{Point(180, 27), Point(180, 130), Point(180, 232)},
	{Point(322, 27), Point(322, 130), Point(322, 232)},
	{Point(464, 27), Point(464, 130), Point(464, 232)}};

/**
 * Constructor for the Game class.
 */
Game::Game()
{
	srand(time(NULL));

	button_text_height = button_text_high / 256;

	scrore_height = font1_high / 256;
	scrore_width = font1_width;

	ExitButton = new Button[3];

	ExitButton[0].SetLeft((640 / 2) - (ExitButton[0].GetWidth() / 2));
	ExitButton[0].SetTop(100);
	ExitButton[0].SetCaption("Close");

	ExitButton[1].SetLeft((640 / 2) - (ExitButton[1].GetWidth() / 2));
	ExitButton[1].SetTop(200);
	ExitButton[1].SetCaption("Reset");

	ExitButton[2].SetLeft((640 / 2) - (ExitButton[2].GetWidth() / 2));
	ExitButton[2].SetTop(300);
	ExitButton[2].SetCaption("Return to Loader");

	MenuButton = new Button[2];

	MenuButton[0].SetLeft((640 / 2) - (MenuButton[0].GetWidth() / 2));
	MenuButton[0].SetTop(150);
	MenuButton[0].SetCaption("2 Players (1 Wiimote)");

	MenuButton[1].SetLeft((640 / 2) - (MenuButton[1].GetWidth() / 2));
	MenuButton[1].SetTop(250);
	MenuButton[1].SetCaption("1 Player (Vs AI)");

 	GameGrid = new Grid();
	Hand = new Cursor();

	WTTPlayer = new Player[2];
	WTTPlayer[0].SetSign('O');
	WTTPlayer[0].SetName("Player 1");
	WTTPlayer[1].SetSign('X');
	WTTPlayer[1].SetName("Player 2");

	GameImg = GRRLIB_LoadTexture(backg);
	SplashImg = GRRLIB_LoadTexture(splash);
	HoverImg = GRRLIB_LoadTexture(hover);
	CopiedImg = GRRLIB_LoadTexture(splash);

	TextFont = GRRLIB_LoadTexture(GRRLIB_font1);

	NewGame();
}

/**
 * Destructor for the Game class.
 */
Game::~Game()
{
	free(GameImg);
	free(SplashImg);
	free(HoverImg);
	free(TextFont);
	if(CopiedImg)
		free(CopiedImg);

	delete GameGrid;
	delete Hand;
	delete[] ExitButton;
	delete[] WTTPlayer;
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
			MenuScreen();
			break;
		case HOME_SCREEN:
			ExitScreen();
			break;
		case GAME_SCREEN:
			GameScreen();
			// AI
			if(!RoundFinished && WTTPlayer[CurrentPlayer].GetType() == PLAYER_CPU)
			{	// AI
				GameGrid->SetPlayerAI(WTTPlayer[CurrentPlayer].GetSign());
				TurnIsOver();
				//GRRLIB_Render();
				//usleep(900000); // Wait 0.9 sec
			}
			break;
		default:
			GRRLIB_FillScreen(0XFF000000);
	}

	if(CurrentScreen != START_SCREEN && 
		WPAD_Probe(WPAD_CHAN_0, NULL) == WPAD_ERR_NO_CONTROLLER)
	{	// Controller is disconnected
		GRRLIB_Rectangle(0, 0, 640, 480, 0xB2000000, 1);
	}
	else
	{
		// Draw Cursor
		Hand->SetPlayer(WTTPlayer[CurrentPlayer].GetSign());
		Hand->Paint();
	}
}

/**
 * Draw the start screen.
 */
void Game::StartSreen()
{
	GRRLIB_DrawImg(0, 0, 640, 480, SplashImg, 0, 1, 1, 255);

	// Message for synchronization
	//GRRLIB_Printf(395, 40, TextFont, 0xFFFFFFFF, 1, "Version 0.1");
	GRRLIB_Printf(50, 310, TextFont, 0xFFFFFFFF, 1, "Programmer: Crayon");
	GRRLIB_Printf(50, 330, TextFont, 0xFFFFFFFF, 1, "Graphics: Mr_Nick666");
	
	int TextLeft = 320 - (strlen(IDS_PRESS_A) * 8);
	GRRLIB_Printf(TextLeft, 400, TextFont, 0xFF000000, 2, IDS_PRESS_A);
}

/**
 * Draw the game screen.
 */
void Game::GameScreen()
{
	int TextLeft;
	//int TextTop;

	// Background image
	GRRLIB_DrawImg(0, 0, 640, 480, GameImg, 0, 1, 1, 255);

	// Draw text
	PrintText(130, 420, text,
		TextFont, 0xFF8C8A8C, 0xCC000000, 1, 1, 1.5);

	// Draw score
	char ScoreText[5];
	sprintf(ScoreText, "%d", WTTPlayer[0].GetScore());
	TextLeft = 106 - (strlen(ScoreText) * 8) / 2;
	PrintText(TextLeft, 85, ScoreText,
		TextFont, 0xFFFFFFFF, 0xCCE6313A, -2, 2, 3);
	sprintf(ScoreText, "%d", WTTPlayer[1].GetScore());
	TextLeft = 106 - (strlen(ScoreText) * 8) / 2;
	PrintText(TextLeft, 188, ScoreText,
		TextFont, 0xFFFFFFFF, 0xCC6BB6DE, -2, 2, 3);
	sprintf(ScoreText, "%d", TieGame);
	TextLeft = 106 - (strlen(ScoreText) * 8) / 2;
	PrintText(TextLeft, 291, ScoreText,
		TextFont, 0xFFFFFFFF, 0xCC109642, -2, 2, 3);

	// Player name
/*
	TextLeft = 40;
	TextTop = 17;
	PrintText(TextLeft, TextTop,
		WTTPlayer[0].GetName(), button_text_img, WHITE, 0xE187, -2, 2, 1);
	TextTop = 143;
	PrintText(TextLeft, TextTop,
		WTTPlayer[1].GetName(), button_text_img, WHITE, 0x6DBB, -2, 2, 1);
	TextTop = 270;
	PrintText(TextLeft, TextTop,
		"Tie Game", button_text_img, WHITE, 0x14A8, -2, 2, 1);

*/
	// Draw grid content
	Symbol *Sign = new Symbol;
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			Sign->SetPlayer(GameGrid->GetPlayerAtPos(x, y));
			Sign->SetLeft(180 + x * 142);
			Sign->SetTop(29 + y * 103);
			Sign->Paint();
		}
	}
	delete Sign;

	SelectZone();

	if(HandX >= 0 && HandY >= 0 && GameGrid->GetPlayerAtPos(HandX, HandY) == ' ')
	{
		GRRLIB_DrawImg(Table[HandX][HandY].GetX(), Table[HandX][HandY].GetY(),
			136, 100, HoverImg, 0, 1, 1, 255);
	}
}

/**
 * Draw the HOME screen.
 */
void Game::ExitScreen()
{
	if(CopiedImg == NULL)
	{
		GameScreen();
		GRRLIB_Rectangle(0, 0, 640, 480, 0xCC000000, 1);
		CopiedImg = GRRLIB_Screen2Texture();
	}
	else
	{
		GRRLIB_DrawImg(0, 0, 640, 480, CopiedImg, 0, 1, 1, 255);
	}

	GRRLIB_Rectangle(0, 0, 640, 63, 0xFF000000, 1);
	GRRLIB_Rectangle(0, 63, 640, 2, 0xFF848284, 1);

	GRRLIB_Rectangle(0, 383, 640, 2, 0xFF848284, 1);
	GRRLIB_Rectangle(0, 385, 640, 95, 0xFF000000, 1);

	GRRLIB_Printf(40, 17, //32, 17,
		TextFont, 0xFFFFFFFF, 1.8, "HOME Menu");

	ExitButton[0].SetSelected(false);
	ExitButton[1].SetSelected(false);
	ExitButton[2].SetSelected(false);
	if(ExitButton[0].IsInside(Hand->GetLeft(), Hand->GetTop()))
	{
		ExitButton[0].SetSelected(true);
		ButtonOn(0);
		SelectedButton = 0;
	}
	else if(ExitButton[1].IsInside(Hand->GetLeft(), Hand->GetTop()))
	{
		ExitButton[1].SetSelected(true);
		ButtonOn(1);
		SelectedButton = 1;
	}
	else if(ExitButton[2].IsInside(Hand->GetLeft(), Hand->GetTop()))
	{
		ExitButton[2].SetSelected(true);
		ButtonOn(2);
		SelectedButton = 2;
	}
	else
	{
		SelectedButton = -1;
	}
	ExitButton[0].Paint();
	ExitButton[1].Paint();
	ExitButton[2].Paint();
}

/**
 * Draw the menu screen.
 */
void Game::MenuScreen()
{
	GRRLIB_FillScreen(0XFF000000);	// Clear screen
	for(int y = 0; y <=480; y+=8)
	{
		GRRLIB_Rectangle(0, y, 640, 2, 0x30B0B0B0, 1);
	}

	GRRLIB_Rectangle(0, 0, 640, 63, 0xFF000000, 1);
	GRRLIB_Rectangle(0, 63, 640, 2, 0xFFFFFFFF, 1);


	GRRLIB_Rectangle(0, 383, 640, 2, 0xFFFFFFFF, 1);
	GRRLIB_Rectangle(0, 385, 640, 95, 0xFF000000, 1);

	GRRLIB_Printf(500, 40, TextFont, 0xFFFFFFFF, 1, "Ver. 0.1");

	MenuButton[0].SetSelected(false);
	MenuButton[1].SetSelected(false);
	if(MenuButton[0].IsInside(Hand->GetLeft(), Hand->GetTop()))
	{
		MenuButton[0].SetSelected(true);
		ButtonOn(0);
		SelectedButton = 0;
	}
	else if(MenuButton[1].IsInside(Hand->GetLeft(), Hand->GetTop()))
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
}

/**
 * Controls all inputs.
 * @param[in] Buttons Buttons down.
 * @return True to exit to loader, false otherwise.
 */
bool Game::ControllerManager()
{
	WPADData *WPadData0 = WPAD_Data(WPAD_CHAN_0);
	unsigned int Buttons = WPAD_ButtonsDown(WPAD_CHAN_0);

	if(WPadData0->ir.smooth_valid)
	{
		Hand->SetLeft(WPadData0->ir.sx);
		Hand->SetTop(WPadData0->ir.sy);
		Hand->SetAngle(WPadData0->ir.angle);
	}
	else
	if(WPadData0->ir.valid)
	{
		Hand->SetLeft(WPadData0->ir.x);
		Hand->SetTop(WPadData0->ir.y);
		Hand->SetAngle(WPadData0->ir.angle);
	}

	if(Buttons)
	{
		switch(CurrentScreen)
		{
			case START_SCREEN:
				if((Buttons & WPAD_BUTTON_A))
				{
					ChangeScreen(MENU_SCREEN);
				}
				break;
			case MENU_SCREEN:
				if((Buttons & WPAD_BUTTON_A))
				{
					switch(SelectedButton)
					{
						case 0:
							WTTPlayer[1].SetType(PLAYER_HUMAN);
							ChangeScreen(GAME_SCREEN);
							break;
						case 1:
							WTTPlayer[1].SetType(PLAYER_CPU);
							ChangeScreen(GAME_SCREEN);
							break;
					}
				}
				break;
			case HOME_SCREEN:
				if((Buttons & WPAD_BUTTON_HOME))
				{
					ChangeScreen(GAME_SCREEN);
				}
				else if((Buttons & WPAD_BUTTON_A))
				{
					switch(SelectedButton)
					{
						case 0:
							ChangeScreen(GAME_SCREEN);
							break;
						case 1:
							NewGame();
							break;
						case 2:
							return true; // Exit to loader
					}
				}
				break;
			default:
				if((Buttons & WPAD_BUTTON_A))
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
					{	// Position is invalid
						WiimoteRumble(200000);  // 200 ms
					}
				}
				else if((Buttons & WPAD_BUTTON_HOME))
				{
					ChangeScreen(HOME_SCREEN);
				}
		}
		if((Buttons & WPAD_BUTTON_1) && (Buttons & WPAD_BUTTON_2))
		{
			WPAD_Rumble(WPAD_CHAN_0, 1); // Rumble on
			if(GRRLIB_ScrShot("Screenshot.png"))
				strcpy(text, "A screenshot was taken!!!");
			else
				strcpy(text, "Screenshot did not work!!!");
			WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off
		}
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
	const char *TextTurn[] = {IDS_PLAYERTURN};
	sprintf(text, TextTurn[rand() % (sizeof(TextTurn) / sizeof(*TextTurn))],
		WTTPlayer[CurrentPlayer].GetName());
	RoundFinished = false;
}

/**
 * Things to do when a turn is over.
 */
void Game::TurnIsOver()
{
	u8 GameWinner = GameGrid->GetWinner();
	if(GameWinner != ' ')
	{	// A winner is declare
		GameWinner = (GameWinner == WTTPlayer[0].GetSign()) ? 0 : 1;
		WTTPlayer[GameWinner].IncScore();
		if(rand() & 1)
		{
			const char *TextWinGame[] = {IDS_PLAYERWIN};
			sprintf(text, TextWinGame[rand() % (sizeof(TextWinGame) / sizeof(*TextWinGame))],
				WTTPlayer[GameWinner].GetName());
		}
		else
		{
			const char *TextLoseGame[] = {IDS_PLAYERLOSE};
			sprintf(text, TextLoseGame[rand() % (sizeof(TextLoseGame) / sizeof(*TextLoseGame))],
				WTTPlayer[!GameWinner].GetName());
		}
		RoundFinished = true;
	}
	else if(GameGrid->IsFilled())
	{	// Tie game
		TieGame++;
		const char *TextTieGame[] = {IDS_PLAYERTIE};
		strcpy(text, TextTieGame[rand() % (sizeof(TextTieGame) / sizeof(*TextTieGame))]);
		RoundFinished = true;
	}
	else
	{
		CurrentPlayer = !CurrentPlayer; // Change player's turn
		const char *TextTurn[] = {IDS_PLAYERTURN};
		sprintf(text, TextTurn[rand() % (sizeof(TextTurn) / sizeof(*TextTurn))],
			WTTPlayer[CurrentPlayer].GetName());
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
	
	Clear();
}

/**
 * Print text on the screen with a shadow.
 */
void Game::PrintText(u16 x, u16 y,
	const char *Text, u8 Font[], u32 TextColor, u32 ShadowColor,
	int ShadowOffsetX, int ShadowOffsetY, f32 zoom)
{
	GRRLIB_Printf(x + ShadowOffsetX, y + ShadowOffsetY, Font, ShadowColor, zoom, Text);
	GRRLIB_Printf(x, y, Font, TextColor, zoom, Text);
}

/**
 * Change the screen.
 * @param[in] NewScreen New screen to show.
 */
void Game::ChangeScreen(u8 NewScreen)
{
	SelectedButton = -1;
	CurrentScreen = NewScreen;

	if(CopiedImg)
	{
		free(CopiedImg);
		CopiedImg = NULL;
	}
}

/**
 * Rumble if on button.
 * @param[in] NewSelectedButton New button to select.
 */
void Game::ButtonOn(signed char NewSelectedButton)
{
	if(SelectedButton != NewSelectedButton)
	{
		WiimoteRumble(50000); // 50 ms
	}
}

/**
 * Check if a zone is selected
 */
void Game::SelectZone()
{
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			if (Hand->GetLeft() > Table[x][y].GetX() &&
				Hand->GetLeft() < (unsigned)(Table[x][y].GetX() + 136) &&
			    Hand->GetTop() > Table[x][y].GetY() &&
				Hand->GetTop() < (unsigned)(Table[x][y].GetY() + 100))
			{
				if(HandX != x || HandY != y)
				{
					WiimoteRumble(30000);  // 30 ms
					HandX = x;
					HandY = y;
				}
				return;
			}
		}
	}

	HandX = -1;
	HandY = -1;
}
