#include <aesndlib.h>
#include <gcmodplay.h>
#include "audio.h"

// Audio files
#include "../audio/button_rollover.h"
#include "../audio/screen_change.h"
#include "../audio/tic-tac.h"

/**
 * Constructor for the Audio class.
 */
Audio::Audio()
{
    AESND_Init();
    AESND_Pause(false);
    ModTrack = new _modplay;

    ScreenVoice = AESND_AllocateVoice(NULL);
    ButtonVoice = AESND_AllocateVoice(NULL);
}

/**
 * Destructor for the Audio class.
 */
Audio::~Audio()
{
    MODPlay_Unload(ModTrack);
    delete ModTrack;
    AESND_Pause(true);
    AESND_Reset();

    AESND_FreeVoice(ScreenVoice);
    AESND_FreeVoice(ButtonVoice);
}

/**
 * Pause/unpause the music volume.
 * @param[in] Paused On or off.
 */
void Audio::PauseMusic(bool Paused)
{
    MODPlay_Pause(ModTrack, Paused);
}

/**
 * Load the music.
 * @param[in] Volume The music volume.
 */
void Audio::LoadMusic(s32 Volume)
{
    MODPlay_Init(ModTrack);
    MODPlay_SetMOD(ModTrack, tic_tac);
    MODPlay_SetVolume(ModTrack, Volume, Volume); // Maximum volume is 64
    MODPlay_SetStereo(ModTrack, true);
    MODPlay_Start(ModTrack);
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
