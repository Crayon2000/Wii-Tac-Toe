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

#include <iostream>

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
	// Video initialization
	VIDEO_Init();
	GRRLIB_InitVideo();
	GRRLIB_Start();

	// Wiimote initialization
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_0, 640, 480);

	// Game initialization
	Game *MyGame = new Game();

	while(1)
	{
		WPAD_ScanPads();
		if(MyGame->ControllerManager())
			break;

		MyGame->Paint();
		GRRLIB_Render();
	}

	delete MyGame;
	WPAD_Shutdown();
	GRRLIB_Stop();
	return 0;
}
