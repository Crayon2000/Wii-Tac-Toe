#include <unistd.h> 		// usleep
#include <ogc/lwp.h>		// Thread
#include <wiiuse/wpad.h>	// Wiimote
#include <ogcsys.h>			// nanosleep

#include "tools.h"

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
 * Returns true if c is a white-space character (0x09 – 0x0D or 0x20).
 * @param[in] c Integer to test.
 * @return true if c is a white-space character (0x09 – 0x0D or 0x20).
 */
static bool isspace(int c)
{
    return (c == 0x09 || c == 0x0D || c == 0x20);
}

/**
 *
 * @param[in] string
 * @return
 */
static size_t get_word(char *string)
{
	int tab_size = 0;
    int i = 0, word_len = 0;

	if(!string[0])
		return 0;
	while(isspace(string[i]))
	{
		if('\t' == string[i])
		{
			if(0 == tab_size)
				string[i] = ' ';
			else
				word_len += tab_size-1;
		}
		else if('\n' == string[i])
		{
			string[i]=' ';
		}
		word_len++;
		i++;
	}
	while(string[i] && !isspace(string[i++]))
		word_len++;
	return word_len;

}

/**
 * Insert carriage return in string at line maximum lenght.
 * @param string
 * @param[in] line_len
 * @return
 */
char *word_wrap(char *string, size_t line_len)
{
	size_t len,				// length of current word
	current_len = 0;  		// current length of line
	size_t start_line = 0;	// index of beginning if line

	while(0 != (len = get_word(&string[current_len + start_line])))
	{
		if(current_len + len < line_len)
		{
			current_len += len;
		}
		else
		{
			string[start_line + current_len] = '\n';
			start_line += current_len + 1;
			current_len = 0;
		}
	}
	return string;
}

/**
 *
 */
int msleep(unsigned long milisec)
{
    struct timespec req={0};
    time_t sec = (int)(milisec / 1000);
    milisec -= (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = milisec * 1000000L;
    while(nanosleep(&req) == -1)
		continue;
    return 1;
}
