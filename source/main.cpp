//==============================================================================
// Project:   Wii-Tac-Toe
//==============================================================================

/** \mainpage Wii-Tac-Toe Documentation
 */

//------------------------------------------------------------------------------
// Headers
//------------------------------------------------------------------------------
#include <stdlib.h>
#include <wiiuse/wpad.h>
extern "C"
{
	#include "grrlib/GRRLIB.h"
}
#include "game.h"
#include "main.h"
//------------------------------------------------------------------------------
// Externals
//------------------------------------------------------------------------------
Mtx GXmodelView2D;

/**
 * Entry point.
 * @param[in] argc The number of arguments invoked with the program
 * @param[in] argv The array containing the arguments
 * @return 0 on clean exit, an error code otherwise
 */
int main(int argc, char **argv)
{
	bool Quit = false;

	// Video initialization
	VIDEO_Init();
	GRRLIB_InitVideo();
	GRRLIB_Start();

	// Wiimote initialization
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS);

	// Game initialization
	Game *MyGame = new Game();

	while(!Quit)
	{
		WPAD_ScanPads();
		Quit = MyGame->ControllerManager(WPAD_ButtonsDown(WPAD_CHAN_0));

		MyGame->Paint();
		GRRLIB_Render();
	}

	delete MyGame;
	return 0;
}
