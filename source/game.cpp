#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <ogc/conf.h>
extern "C"
{
	#include "grrlib/GRRLIB.h"
}
#include "tools.h"
#include "main.h"
#include "symbol.h"
#include "game.h"

// Fonts
//#include "../fonts/button_text.h"
//#include "../fonts/font1.h"
#include "../fonts/GRRLIB_font1.h"

// Graphics
#include "../gfx/splash.h"
#include "../gfx/backg.h"
#include "../gfx/hover_o.h"
#include "../gfx/hover_x.h"

#define START_SCREEN 	0
#define GAME_SCREEN 	1
#define HOME_SCREEN 	2
#define MENU_SCREEN 	3

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

	//button_text_height = button_text_high / 256;

	//scrore_height = font1_high / 256;
	//scrore_width = font1_width;

 	GameGrid = new Grid();
	Hand = new Cursor();
	Lang = new Language();

	ExitButton = new Button[3];

	ExitButton[0].SetLeft((640 / 2) - (ExitButton[0].GetWidth() / 2));
	ExitButton[0].SetTop(100);
	ExitButton[0].SetCaption(Lang->Text("Close"));

	ExitButton[1].SetLeft((640 / 2) - (ExitButton[1].GetWidth() / 2));
	ExitButton[1].SetTop(200);
	ExitButton[1].SetCaption(Lang->Text("Reset"));

	ExitButton[2].SetLeft((640 / 2) - (ExitButton[2].GetWidth() / 2));
	ExitButton[2].SetTop(300);
	ExitButton[2].SetCaption(Lang->Text("Return to Loader"));

	MenuButton = new Button[2];

	MenuButton[0].SetLeft((640 / 2) - (MenuButton[0].GetWidth() / 2));
	MenuButton[0].SetTop(150);
	MenuButton[0].SetCaption(Lang->Text("2 Players (1 Wiimote)"));

	MenuButton[1].SetLeft((640 / 2) - (MenuButton[1].GetWidth() / 2));
	MenuButton[1].SetTop(250);
	MenuButton[1].SetCaption(Lang->Text("1 Player (Vs AI)"));

	WTTPlayer = new Player[2];
	WTTPlayer[0].SetSign('O');
	WTTPlayer[0].SetName(Lang->Text("PLAYER 1"));
	WTTPlayer[1].SetSign('X');
	WTTPlayer[1].SetName(Lang->Text("PLAYER 2"));

	GameImg = GRRLIB_LoadTexture(backg);
	SplashImg = GRRLIB_LoadTexture(splash);
	HoverImgO = GRRLIB_LoadTexture(hover_o);
	HoverImgX = GRRLIB_LoadTexture(hover_x);
	CopiedImg = NULL;

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
	free(HoverImgX);
	free(HoverImgO);
	free(TextFont);
	free(CopiedImg);

	delete GameGrid;
	delete Hand;
	delete Lang;
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
/*
		snprintf(text, TEXT_SIZE, "X = %.02f; Y = %.02f",
			Hand->GetLeftCorrected(), Hand->GetTopCorrected());
*/
	}
}

/**
 * Draw the start screen.
 */
void Game::StartSreen()
{
	if(CopiedImg == NULL)
	{	// Copy static element
		GRRLIB_DrawImg(0, 0, 640, 480, SplashImg, 0, 1, 1, 255);

		// Message for synchronization
		//GRRLIB_Printf(395, 40, TextFont, 0xFFFFFFFF, 1, "Version 0.4");
		GRRLIB_Printf(50, 310, TextFont, 0xFFFFFFFF, 1, Lang->Text("Programmer: %s"), "Crayon");
		GRRLIB_Printf(50, 330, TextFont, 0xFFFFFFFF, 1, Lang->Text("Graphics: %s"), "Mr_Nick666");

		int TextLeft = 320 - (strlen(Lang->Text("Press The A Button")) * 8);
		GRRLIB_Printf(TextLeft, 400, TextFont, 0xFF000000, 2, Lang->Text("Press The A Button"));

		CopiedImg = GRRLIB_Screen2Texture();
	}
	else
	{
		GRRLIB_DrawImg(0, 0, 640, 480, CopiedImg, 0, 1, 1, 255);
	}
}

/**
 * Draw the game screen.
 */
void Game::GameScreen()
{
	int TextLeft, StrLenght;

	if(CopiedImg == NULL)
	{	// Copy static element
		// Background image
		GRRLIB_DrawImg(0, 0, 640, 480, GameImg, 0, 1, 1, 255);

		// Player name
		TextLeft = 55;
		PrintText(TextLeft, 50,
			WTTPlayer[0].GetName(), TextFont, 0xFFFFFFFF, 0xCCE6313A, -2, 2, 1.5);
		PrintText(TextLeft, 145,
			WTTPlayer[1].GetName(), TextFont, 0xFFFFFFFF, 0xCC6BB6DE, -2, 2, 1.5);
		PrintText(TextLeft, 250,
			Lang->Text("TIE GAME"), TextFont, 0xFFFFFFFF, 0xCC109642, -2, 2, 1.5);

		CopiedImg = GRRLIB_Screen2Texture();
	}
	else
	{
		GRRLIB_DrawImg(0, 0, 640, 480, CopiedImg, 0, 1, 1, 255);
	}

	// Draw text
	PrintWrapText(130, 420, 390, text,
		TextFont, 0xFF8C8A8C, 0xCC000000, 1, 1, 1.5);

	// Draw score
	char ScoreText[5];
	StrLenght = snprintf(ScoreText, 5, "%d", WTTPlayer[0].GetScore());
	TextLeft = 106 - (StrLenght * 8) / 2;
	PrintText(TextLeft, 85, ScoreText,
		TextFont, 0xFFFFFFFF, 0xCCE6313A, -2, 2, 3);
	StrLenght = snprintf(ScoreText, 5, "%d", WTTPlayer[1].GetScore());
	TextLeft = 106 - (StrLenght * 8) / 2;
	PrintText(TextLeft, 188, ScoreText,
		TextFont, 0xFFFFFFFF, 0xCC6BB6DE, -2, 2, 3);
	StrLenght = snprintf(ScoreText, 5, "%d", TieGame);
	TextLeft = 106 - (StrLenght * 8) / 2;
	PrintText(TextLeft, 291, ScoreText,
		TextFont, 0xFFFFFFFF, 0xCC109642, -2, 2, 3);

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
		if(WTTPlayer[CurrentPlayer].GetSign() == 'X')
		{
			GRRLIB_DrawImg(Table[HandX][HandY].GetX(), Table[HandX][HandY].GetY(),
				140, 100, HoverImgX, 0, 1, 1, 255);
		}
		else
		{
			GRRLIB_DrawImg(Table[HandX][HandY].GetX(), Table[HandX][HandY].GetY(),
				140, 100, HoverImgO, 0, 1, 1, 255);
		}
	}
}

/**
 * Draw the HOME screen.
 */
void Game::ExitScreen()
{
	if(CopiedImg == NULL)
	{	// Copy static element
		GameScreen();	// Show Game screen
		GRRLIB_Rectangle(0, 0, 640, 480, 0xCC000000, 1); // Draw a black rectabgle over it

		GRRLIB_Rectangle(0, 0, 640, 63, 0xFF000000, 1);
		GRRLIB_Rectangle(0, 63, 640, 2, 0xFF848284, 1);

		GRRLIB_Rectangle(0, 383, 640, 2, 0xFF848284, 1);
		GRRLIB_Rectangle(0, 385, 640, 95, 0xFF000000, 1);

		GRRLIB_Printf(40, 17, //32, 17,
			TextFont, 0xFFFFFFFF, 1.8, Lang->Text("HOME Menu"));

		CopiedImg = GRRLIB_Screen2Texture();
	}
	else
	{
		GRRLIB_DrawImg(0, 0, 640, 480, CopiedImg, 0, 1, 1, 255);
	}

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
	if(CopiedImg == NULL)
	{	// Copy static element
		GRRLIB_FillScreen(0XFF000000);	// Clear screen
		for(int y = 0; y <=480; y+=8)
		{
			GRRLIB_Rectangle(0, y, 640, 2, 0x30B0B0B0, 1);
		}

		GRRLIB_Rectangle(0, 0, 640, 63, 0xFF000000, 1);
		GRRLIB_Rectangle(0, 63, 640, 2, 0xFFFFFFFF, 1);


		GRRLIB_Rectangle(0, 383, 640, 2, 0xFFFFFFFF, 1);
		GRRLIB_Rectangle(0, 385, 640, 95, 0xFF000000, 1);

		GRRLIB_Printf(500, 40, TextFont, 0xFFFFFFFF, 1, Lang->Text("Ver. %s"), "0.4");

		CopiedImg = GRRLIB_Screen2Texture();
	}
	else
	{
		GRRLIB_DrawImg(0, 0, 640, 480, CopiedImg, 0, 1, 1, 255);
	}

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
	else if(WPadData0->ir.valid)
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
							ExitScreen();
							Hand->Paint();
							CopiedImg = GRRLIB_Screen2Texture();
							GRRLIB_DrawImg_FadeOut(640, 480, CopiedImg, 1, 1, 5);
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
						Rumble_Wiimote(200);  // 200 ms
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
			WIILIGHT_TurnOn();
			if(GRRLIB_ScrShot("Screenshot.png"))
				strncpy(text, "A screenshot was taken!!!", TEXT_SIZE);
			else
				strncpy(text, "Screenshot did not work!!!", TEXT_SIZE);
			WIILIGHT_TurnOff();
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
	snprintf(text, TEXT_SIZE, Lang->GetRandomTurnOverMessage(), WTTPlayer[CurrentPlayer].GetName());
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
		char *TextToCopy;
		strncpy(text, Lang->GetRandomWinningMessage(), TEXT_SIZE);
		TextToCopy = str_replace(text, "$LOSER$", WTTPlayer[!GameWinner].GetName());
		if(TextToCopy)
		{
			strncpy(text, TextToCopy, TEXT_SIZE);
			free(TextToCopy);
		}
		TextToCopy = str_replace(text, "$WINNER$", WTTPlayer[GameWinner].GetName());
		if(TextToCopy)
		{
			strncpy(text, TextToCopy, TEXT_SIZE);
			free(TextToCopy);
		}
		RoundFinished = true;
	}
	else if(GameGrid->IsFilled())
	{	// Tie game
		TieGame++;
		strncpy(text, Lang->GetRandomTieMessage(), TEXT_SIZE);
		RoundFinished = true;
	}
	else
	{
		CurrentPlayer = !CurrentPlayer; // Change player's turn
		snprintf(text, TEXT_SIZE, Lang->GetRandomTurnOverMessage(), WTTPlayer[CurrentPlayer].GetName());
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
 *
 */
void Game::PrintWrapText(u16 x, u16 y, u16 MaxWidth,
	const char *Text, u8 Font[], u32 TextColor, u32 ShadowColor,
	int ShadowOffsetX, int ShadowOffsetY, f32 zoom)
{
	char NewText[256];
	//u16 Offset = 0;
	
	strncpy(NewText, Text, 256);
	word_wrap(NewText, MaxWidth / (8 * zoom)); //    8 = char width
	
	
	PrintText(x, y, NewText,
		Font, TextColor, ShadowColor,
		ShadowOffsetX, ShadowOffsetY, zoom);
/*
	while()
	{
		strrchr(NewText, '\n');
	
		PrintText(x + Offset, y, NewText,
			Font, TextColor, ShadowColor,
			ShadowOffsetX, ShadowOffsetY, zoom);
		Offset += 16; // 16 = char height
	}
*/
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
		Rumble_Wiimote(50); // 50 ms
	}
}

/**
 * Check if a zone is selected
 */
void Game::SelectZone()
{
	if(!RoundFinished)
	{
		for(int x = 0; x < 3; x++)
		{
			for(int y = 0; y < 3; y++)
			{
				if (Hand->GetLeft() > Table[x][y].GetX() &&
					Hand->GetLeft() < (Table[x][y].GetX() + 136) &&
					Hand->GetTop() > Table[x][y].GetY() &&
					Hand->GetTop() < (Table[x][y].GetY() + 100))
				{
					if(HandX != x || HandY != y)
					{
						Rumble_Wiimote(30);  // 30 ms
						HandX = x;
						HandY = y;
					}
					return;
				}
			}
		}
	}
	HandX = -1;
	HandY = -1;
}
