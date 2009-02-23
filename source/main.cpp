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
#include "grrlib/GRRLIB.h"
#include "game.h"
#include "main.h"

#include <iostream>

//------------------------------------------------------------------------------
// Externals
//------------------------------------------------------------------------------
Mtx GXmodelView2D;
u8 HWButton = 0;

/**
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed()
{
	HWButton = SYS_RETURNTOMENU;
}

/**
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF_STANDBY;
}

/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF_STANDBY;
	//SYS_POWEROFF
}

/**
 * Entry point.
 * @param[in] argc The number of arguments invoked with the program
 * @param[in] argv The array containing the arguments
 * @return 0 on clean exit, an error code otherwise
 */
int main(int argc, char **argv)
{
	// Video initialization
	GRRLIB_Init();
	GRRLIB_InitFreetype();

	// Wiimote initialization
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_0, 640, 480);

	// Game initialization
	Game *MyGame = new Game();

	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);

	while(1)
	{
		WPAD_ScanPads();
		if(MyGame->ControllerManager())
			break;
		if(HWButton)
			break;

		MyGame->Paint();
		GRRLIB_Render();
	}

	delete MyGame;
	WPAD_Shutdown();
	GRRLIB_Exit();

	if(HWButton)
	{
		SYS_ResetSystem(HWButton, 0, 0);
	}

	return 0;
}
