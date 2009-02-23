//---------------------------------------------------------------------------
#ifndef GameH
#define GameH
//---------------------------------------------------------------------------

#include "grid.h"
#include "button.h"
#include "cursor.h"
#include "player.h"
#include "language.h"

#define TEXT_SIZE 100

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
	bool ControllerManager();
private:
	void StartSreen();
	void MenuScreen(bool);
	void GameScreen(bool);
	void ExitScreen();
	void Clear();
	void TurnIsOver();
	void NewGame();
	void PrintWrapText(u16, u16, u16, const char*, unsigned int, unsigned int);
	void ChangeScreen(u8 NewScreen);
	void ButtonOn(signed char);
	bool SelectZone();
	void FreeMemImg();

	Cursor *Hand;
	signed char HandX, HandY;

	bool CurrentPlayer;
	bool PlayerToStart;
	Player *WTTPlayer;
	u8 CurrentScreen;
	u8 LastScreen;
	signed char SelectedButton;
	char text[TEXT_SIZE];

	int button_text_height;
	u8 scrore_height;
	u8 scrore_width;

	u16 TieGame;
	bool RoundFinished;

	Button *ExitButton;
	Button *MenuButton;
	Grid *GameGrid;
	Language *Lang;

	GRRLIB_texImg GameImg;
	GRRLIB_texImg SplashImg;
	GRRLIB_texImg HoverImgX;
	GRRLIB_texImg HoverImgO;
	GRRLIB_texImg CopiedImg;

//u8 Text[1228800] ATTRIBUTE_ALIGN (32);      // stores (640 * 480 * 4) texture
};

class Point
{
private:
    u16 x, y;
public:
	Point(u16 x, u16 y)
	{
		this->x = x;
		this->y = y;
	}
	u16 GetX()
	{
		return x;
	}
	u16 GetY()
	{
		return y;
	}
	void setLocation(u16 x, u16 y)
	{
		this->x = x;
		this->y = y;
	}
};
//---------------------------------------------------------------------------
#endif
