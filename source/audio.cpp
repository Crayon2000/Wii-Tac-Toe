#include <aesndlib.h>
#include <grrmod.h>
#include "audio.h"

// Audio files
#include "../audio/button_rollover.h"
#include "../audio/screen_change.h"
#include "../audio/tic-tac.h"

/**
 * Constructor for the Audio class.
 */
Audio::Audio() :
    Paused(false)
{
    AESND_Init();
    AESND_Pause(false);

    GRRMOD_Init();
    GRRMOD_SetMOD(tic_tac, tic_tac_size);

    ScreenVoice = AESND_AllocateVoice(NULL);
    ButtonVoice = AESND_AllocateVoice(NULL);
}

/**
 * Destructor for the Audio class.
 */
Audio::~Audio()
{
    AESND_Pause(true);
    AESND_FreeVoice(ScreenVoice);
    AESND_FreeVoice(ButtonVoice);

    GRRMOD_Unload();
    GRRMOD_End();
}

/**
 * Pause/unpause the music volume.
 * @param[in] Paused On or off.
 */
void Audio::PauseMusic(bool Paused)
{
    if(this->Paused != Paused)
    {
        this->Paused = Paused;
        GRRMOD_Pause();
    }
}

/**
 * Load the music.
 * @param[in] Volume The music volume.
 */
void Audio::LoadMusic(s32 Volume)
{
    GRRMOD_Stop();
    GRRMOD_Start();
    GRRMOD_SetVolume(Volume, Volume);
    Paused = false;
}

/**
 * Play the screen change sound FX.
 * @param[in] Volume The sound volume.
 */
void Audio::PlaySoundScreenChange(u16 Volume)
{
    AESND_SetVoiceVolume(ScreenVoice, Volume, Volume);
    AESND_PlayVoice(ScreenVoice, VOICE_MONO16, (void *)screen_change, screen_change_size, 44100, 0, false);
}

/**
 * Play the button sound FX.
 * @param[in] Volume The sound volume.
 */
void Audio::PlaySoundButton(u16 Volume)
{
    AESND_SetVoiceVolume(ButtonVoice, Volume, Volume);
    AESND_PlayVoice(ButtonVoice, VOICE_MONO16, (void *)button_rollover, button_rollover_size, 44100, 0, false);
}
