#include <unistd.h> 		// usleep
#include <ogc/lwp.h>		// Thread
#include <wiiuse/wpad.h>	// Wiimote
#include <ogcsys.h>			// nanosleep
#include <stdlib.h>
#include <string.h>

#include "tools.h"


static vu32 *_wiilight_reg = (u32*)0xCD0000C0;


/**
 *
 */
void *RumbleThread(void *RumbleTime)
{
	int *Time = (int *)RumbleTime;
	WPAD_Rumble(WPAD_CHAN_0, 1); // Rumble on
	usleep(*Time * 1000);
	WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off
	return 0;
}

/**
 *
 */
void Rumble_Wiimote(int RumbleTime)
{
	lwp_t thread;
	LWP_CreateThread(&thread, RumbleThread, &RumbleTime, NULL, 0, 80);
	LWP_ResumeThread(thread);
}

/**
 * Waits for an amount of time in msec.
 * @param[in] milisec Number of milliseconds to wait.
 */
void msleep(unsigned long milisec)
{
    struct timespec req={0};
    time_t sec = (int)(milisec / 1000);
    milisec -= (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = milisec * 1000000L;
    while(nanosleep(&req) == -1)
		continue;
}

/**
 * Turn Wii light off
 */
void WIILIGHT_TurnOff()
{
	*_wiilight_reg &= ~0x20;
}

/**
 * Turn Wii light on
 */
void WIILIGHT_TurnOn()
{
	*_wiilight_reg |= 0x20;
}

/**
 * Replace a string by another string in a string
 * @param[in] txt String to search.
 * @param[in] Avant String to be replaced.
 * @param[in] Apres Replacement string.
 * @return Pointer to the new string with replaced string, must be freed
 */
char *str_replace(const char *txt, const char *Avant, const char *Apres)
{
	const char *pos;
	char *TxtRetour;
	size_t PosTxtRetour;
	size_t Long;
	size_t TailleAllouee;

	pos = strstr(txt, Avant);

	if(pos == NULL)
	{
		return NULL;
	}

	Long = (size_t)pos - (size_t)txt;
	TailleAllouee = Long + strlen(Apres) + 1;
	TxtRetour = (char*)malloc(TailleAllouee);
	PosTxtRetour = 0;

	strncpy(TxtRetour + PosTxtRetour, txt, Long);
	PosTxtRetour += Long;
	txt = pos + strlen(Avant);

	Long = strlen(Apres);
	strncpy(TxtRetour + PosTxtRetour, Apres, Long);
	PosTxtRetour += Long;

	pos = strstr(txt, Avant);
	while(pos != NULL)
	{
		Long = (size_t)pos - (size_t)txt;
		TailleAllouee += Long + strlen(Apres);
		TxtRetour = (char *)realloc(TxtRetour, TailleAllouee);

		strncpy(TxtRetour + PosTxtRetour, txt, Long);
		PosTxtRetour += Long;

		txt = pos + strlen(Avant);

		Long = strlen(Apres);
		strncpy(TxtRetour + PosTxtRetour, Apres, Long);
		PosTxtRetour += Long;

		pos = strstr(txt, Avant);
	}

	Long = strlen(txt) + 1;
	TailleAllouee += Long;
	TxtRetour = (char*)realloc(TxtRetour, TailleAllouee);
	strncpy(TxtRetour + PosTxtRetour, txt, Long);
	return TxtRetour;
}
