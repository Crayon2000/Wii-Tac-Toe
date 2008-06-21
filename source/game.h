//---------------------------------------------------------------------------
#ifndef GameH
#define GameH
//---------------------------------------------------------------------------

#include "grid.h"
#include "button.h"
#include "cursor.h"
#include "player.h"

/**
 * This is the main class of this project. This is where the magic happens.
 * @author Crayon
 */
class Game
{
public:
	Game();
	~Game();
	void Paint();
	bool ControllerManager(unsigned int);
private:
	void StartSreen();
	void MenuScreen();
	void GameScreen();
	void ExitScreen();
	void Clear();
	void TurnIsOver();
	void NewGame();
	void PrintText(u16, u16, u16, u16, const char*, const u16 [], u16, u16, int, int);
	void ChangeScreen(u8 NewScreen);

	Cursor *Hand;
	signed char HandX, HandY;

	bool CurrentPlayer;
	bool PlayerToStart;
	Player *WTTPlayer;
	u8 CurrentScreen;
	signed char SelectedButton;
	char text[50];

	int button_text_height;
	u8 scrore_height;
	u8 scrore_width;
	
	u16 TieGame;
	bool RoundFinished;

	Button *ExitButton;
	Button *MenuButton;
	Grid *GameGrid;
	u16 *ScreenCopy;
};
//---------------------------------------------------------------------------
#endif
