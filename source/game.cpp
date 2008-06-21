//#include <malloc.h>
//#include <math.h>
//#include <ogcsys.h>
#include <unistd.h> // usleep
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

// Graphics
//#include "../gfx/entry.h"
#include "../gfx/game.h"

#define START_SCREEN 	0
#define GAME_SCREEN 	1
#define HOME_SCREEN 	2
#define MENU_SCREEN 	3

#define IDS_PLAYERTURN1 "%s, it's your turn."
#define IDS_PLAYERTURN2 "We are waiting for %s."
#define IDS_PLAYERTURN3 "%s is thinking of is next move."
#define IDS_PLAYERTURN4 "%s should take the Wiimote and play."
#define IDS_PLAYERWIN1  "%s won this round."
#define IDS_PLAYERWIN2  "Is %s the best?"
#define IDS_PLAYERTIE1  "Tie game, that sucks."
#define IDS_PLAYERTIE2  "You both loose this one."

#define IDS_PLAYERTURN IDS_PLAYERTURN1, IDS_PLAYERTURN2, IDS_PLAYERTURN3, \
						IDS_PLAYERTURN4
#define IDS_PLAYERWIN IDS_PLAYERWIN1, IDS_PLAYERWIN2
#define IDS_PLAYERTIE IDS_PLAYERTIE1, IDS_PLAYERTIE2

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
	MenuButton[1].SetCaption("1 Player (Vs Stupid CPU)");

 	GameGrid = new Grid();
	Hand = new Cursor();

	WTTPlayer = new Player[2];
	WTTPlayer[0].SetSign('O');
	WTTPlayer[0].SetName("Player 1");
	WTTPlayer[1].SetSign('X');
	WTTPlayer[1].SetName("Player 2");

	ScreenCopy = NULL;

	NewGame();
}

/**
 * Destructor for the Game class.
 */
Game::~Game()
{
	delete GameGrid;
	delete Hand;
	delete[] ExitButton;
	delete[] WTTPlayer;
	if(ScreenCopy)
		delete[] ScreenCopy;
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
		default:
			GameScreen();
			// AI
			if(!RoundFinished && WTTPlayer[CurrentPlayer].GetType() == PLAYER_CPU)
			{	// AI
				GameGrid->SetPlayerRandom(WTTPlayer[CurrentPlayer].GetSign());
				//HandX = GameGrid->;
				//HandY = ;
				TurnIsOver();
				GRRLIB_Render();
				VIDEO_WaitVSync();
				usleep(900000); // Wait 0.9 sec
			}
	}
	if(CurrentScreen != START_SCREEN && 
		WPAD_Probe(WPAD_CHAN_0, NULL) == WPAD_ERR_NO_CONTROLLER)
	{	// Controller is disconnected
		GRRLIB_DrawRectangle(0, 0, 640, 480, 0x0000, true, 70);
	}
}

/**
 * Draw the start screen.
 */
void Game::StartSreen()
{
	GRRLIB_FillScreen(BLACK);
/*
	GRRLIB_DrawImg(0, 0, entry_width, entry_high, entry_img, 0, 1, 100);
*/	

	// Message for synchronization
//int font1_height = font1_high / 256;

	GRRLIB_Print(0, 50,
		button_text_width, button_text_height, "Wii-Tac-Toe 0.1",
		button_text_img, WHITE, TRANSPARENT);
	GRRLIB_Print(0, 50 + button_text_height,
		button_text_width, button_text_height, "Programmed by Crayon and designed by Mr_Nick666",
		button_text_img, WHITE, TRANSPARENT);
	GRRLIB_Print(0, 50 + button_text_height * 2,
		button_text_width, button_text_height, "Press The A Button",
		button_text_img, WHITE, TRANSPARENT);
}

/**
 * Draw the game screen.
 */
void Game::GameScreen()
{
	int TextLeft;
	//int TextTop;
	
	// Background image
	GRRLIB_DrawImg(0, 0, game_width, game_high, game_img, 0, 1, 100);

	// Draw text
	PrintText(130, 420, button_text_width, button_text_height, text,
		button_text_img, 0x8C51, BLACK, 1, 1);

	// Draw score
	char ScoreText[5];
	sprintf(ScoreText, "%d", WTTPlayer[0].GetScore());
	TextLeft = 106 - (strlen(ScoreText) * scrore_width) / 2;
	PrintText(TextLeft, 75, scrore_width, scrore_height, ScoreText,
		font1_img, WHITE, 0xE187, -2, 2);
	sprintf(ScoreText, "%d", WTTPlayer[1].GetScore());
	TextLeft = 106 - (strlen(ScoreText) * scrore_width) / 2;
	PrintText(TextLeft, 178, scrore_width, scrore_height, ScoreText,
		font1_img, WHITE, 0x6DBB, -2, 2);
	sprintf(ScoreText, "%d", TieGame);
	TextLeft = 106 - (strlen(ScoreText) * scrore_width) / 2;
	PrintText(TextLeft, 281, scrore_width, scrore_height, ScoreText,
		font1_img, WHITE, 0x14A8, -2, 2);

	// Player name
/*
	TextLeft = 40;
	TextTop = 17;
	PrintText(TextLeft, TextTop, button_text_width, button_text_height,
		WTTPlayer[0].GetName(), button_text_img, WHITE, 0xE187, -2, 2);
	TextTop = 143;
	PrintText(TextLeft, TextTop, button_text_width, button_text_height,
		WTTPlayer[1].GetName(), button_text_img, WHITE, 0x6DBB, -2, 2);
	TextTop = 270;
	PrintText(TextLeft, TextTop, button_text_width, button_text_height,
		"Tie", button_text_img, WHITE, 0x14A8, -2, 2);

*/
	// Draw grid content
	Symbol Sign;
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			Sign.SetPlayer(GameGrid->GetPlayerAtPos(x, y));
			Sign.SetLeft(180 + x * 142);
			Sign.SetTop(29 + y * 103);
			Sign.Paint();
		}
	}

	// Draw Cursor
	Hand->SetPlayer(WTTPlayer[CurrentPlayer].GetSign());
	Hand->Paint();
	Hand->SetLeft(265 + HandX * 142);
	Hand->SetTop(80 + HandY * 103);
}

/**
 * Draw the HOME screen.
 */
void Game::ExitScreen()
{
	if(ScreenCopy == NULL)
	{	// Part to design once
		GameScreen();

		GRRLIB_DrawRectangle(0, 0, 640, 62, BLACK, true, 100);
		GRRLIB_DrawRectangle(0, 63, 640, 2, 0x8410, true, 100);

		GRRLIB_DrawRectangle(0, 65, 640, 317, BLACK, true, 80);

		GRRLIB_DrawRectangle(0, 383, 640, 2, 0x8410, true, 100);
		GRRLIB_DrawRectangle(0, 385, 640, 480, BLACK, true, 100);

		GRRLIB_Print(32, 17,
			button_text_width, button_text_height, "HOME Menu",
			button_text_img, WHITE, TRANSPARENT);

		ScreenCopy = new u16[640 * 480 * 2];
		memcpy(ScreenCopy, GRRLIB_buffer, 640*480*2);
	}
	else
	{
		memcpy(GRRLIB_buffer, ScreenCopy, 640*480*2);
	}

	ExitButton[0].SetSelected((SelectedButton == 0) ? true : false);
	ExitButton[0].Paint();

	ExitButton[1].SetSelected((SelectedButton == 1) ? true : false);
	ExitButton[1].Paint();

	ExitButton[2].SetSelected((SelectedButton == 2) ? true : false);
	ExitButton[2].Paint();
}

/**
 * Draw the menu screen.
 */
void Game::MenuScreen()
{
	GRRLIB_FillScreen(BLACK);	// Clear screen

	MenuButton[0].SetSelected((SelectedButton == 0) ? true : false);
	MenuButton[0].Paint();

	MenuButton[1].SetSelected((SelectedButton == 1) ? true : false);
	MenuButton[1].Paint();
}

/**
 * Controls all inputs.
 * @param[in] Buttons Buttons down.
 * @return True to exit to loader, false otherwise.
 */
bool Game::ControllerManager(unsigned int Buttons)
{
	if(Buttons)
	{
		switch(CurrentScreen)
		{
			case START_SCREEN:
				if((Buttons & WIIMOTE_BUTTON_A) == WIIMOTE_BUTTON_A)
				{
					SelectedButton = 0;
					ChangeScreen(MENU_SCREEN);
				}
				break;
			case MENU_SCREEN:
				if((Buttons & WIIMOTE_BUTTON_A) == WIIMOTE_BUTTON_A)
				{
					switch(SelectedButton)
					{
						case 1:
							WTTPlayer[1].SetType(PLAYER_CPU);
							break;
						default:
							WTTPlayer[1].SetType(PLAYER_HUMAN);
					}
					ChangeScreen(GAME_SCREEN);
				}
				else if((Buttons & WIIMOTE_BUTTON_UP) == WIIMOTE_BUTTON_UP)
				{
					if(--SelectedButton < 0)
						SelectedButton = 0;
				}
				else if((Buttons & WIIMOTE_BUTTON_DOWN) == WIIMOTE_BUTTON_DOWN)
				{
					if(++SelectedButton > 1)
						SelectedButton = 1;
				}
				break;
			case HOME_SCREEN:
				if((Buttons & WIIMOTE_BUTTON_HOME) == WIIMOTE_BUTTON_HOME)
				{
					ChangeScreen(GAME_SCREEN);
				}
				else if((Buttons & WIIMOTE_BUTTON_A) == WIIMOTE_BUTTON_A)
				{
					switch(SelectedButton)
					{
						case 1:
							NewGame();
							break;
						case 2:
							return true; // Exit to loader
						default:
							ChangeScreen(GAME_SCREEN);
					}
				}
				else if((Buttons & WIIMOTE_BUTTON_UP) == WIIMOTE_BUTTON_UP)
				{
					if(--SelectedButton < 0)
						SelectedButton = 0;
				}
				else if((Buttons & WIIMOTE_BUTTON_DOWN) == WIIMOTE_BUTTON_DOWN)
				{
					if(++SelectedButton > 2)
						SelectedButton = 2;
				}
				break;
			default:
				if((Buttons & WIIMOTE_BUTTON_UP) == WIIMOTE_BUTTON_UP && !RoundFinished)
				{
					if(--HandY < 0)
						HandY = 0;
				}
				else if((Buttons & WIIMOTE_BUTTON_DOWN) == WIIMOTE_BUTTON_DOWN && !RoundFinished)
				{
					if(++HandY > 2)
						HandY = 2;
				}
				else if((Buttons & WIIMOTE_BUTTON_LEFT) == WIIMOTE_BUTTON_LEFT && !RoundFinished)
				{
					if(--HandX < 0)
						HandX = 0;
				}
				else if((Buttons & WIIMOTE_BUTTON_RIGHT) == WIIMOTE_BUTTON_RIGHT && !RoundFinished)
				{
					if(++HandX > 2)
						HandX = 2;
				}
				else if((Buttons & WIIMOTE_BUTTON_A) == WIIMOTE_BUTTON_A)
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
						WPAD_Rumble(WPAD_CHAN_0, 1); // Rumble on
						usleep(200000); // 200 ms
						WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off
					}
				}
				else if((Buttons & WIIMOTE_BUTTON_HOME) == WIIMOTE_BUTTON_HOME)
				{
					SelectedButton = 0;
					ChangeScreen(HOME_SCREEN);
				}
				else if((Buttons & WIIMOTE_BUTTON_B) == WIIMOTE_BUTTON_B)
				{
					//GRRLIB_ScrShot("Screenshot.pnm", 0);
					//GRRLIB_ScrShot("Screenshot.grr", 1);
					if(GRRLIB_ScrShot("Screenshot.bmp", 2))
						strcpy(text, "A screenshot was taken!!!");
					else
						strcpy(text, "Screenshot did not work!!!");
				}
				else if((Buttons & WPAD_BUTTON_PLUS) == WPAD_BUTTON_PLUS)
				{
				}
				else if((Buttons & WPAD_BUTTON_MINUS) == WPAD_BUTTON_MINUS)
				{
				}
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
	HandX = 1;
	HandY = 1;
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
		const char *TextWinGame[] = {IDS_PLAYERWIN};
		sprintf(text, TextWinGame[rand() % (sizeof(TextWinGame) / sizeof(*TextWinGame))],
			WTTPlayer[GameWinner].GetName());
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
void Game::PrintText(u16 x, u16 y, u16 Width, u16 Height,
	const char *Text, const u16 Font[], u16 TextColor, u16 ShadowColor,
	int ShadowOffsetX, int ShadowOffsetY)
{
	GRRLIB_Print(x + ShadowOffsetX, y + ShadowOffsetY, Width, Height, Text,
		Font, ShadowColor, TRANSPARENT);
	GRRLIB_Print(x, y, Width, Height, Text,
		Font, TextColor, TRANSPARENT);
}

/**
 * Change the screen.
 * @param[in] NewScreen New screen to show.
 */
void Game::ChangeScreen(u8 NewScreen)
{
	CurrentScreen = NewScreen;

	if(ScreenCopy)
	{
		delete[] ScreenCopy;
		ScreenCopy = NULL;
	}
}
