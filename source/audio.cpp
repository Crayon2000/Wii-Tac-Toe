#include <asndlib.h>
#include <gcmodplay.h>
#include "audio.h"

// Audio files
#include "../audio/button_rollover.h"
#include "../audio/screen_change.h"
#include "../audio/tic-tac.h"

#define MODPLAYER_VOICE 0
#define BUTTON_VOICE    1
#define SCREEN_VOICE    2

/**
 * Constructor for the Audio class.
 */
Audio::Audio()
{
    ASND_Init();
    ASND_Pause(0);
    ModTrack = new _modplay;
}

/**
 * Destructor for the Audio class.
 */
Audio::~Audio()
{
    MODPlay_Unload(ModTrack);
    delete ModTrack;
    ASND_Pause(1);
    ASND_End();
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
void Audio::PlaySoundScreenChange(s32 Volume)
{
    ASND_SetVoice(SCREEN_VOICE, VOICE_MONO_16BIT, 44100, 0,
        (void *)screen_change, screen_change_size, Volume, Volume, NULL);
}

/**
 * Play the button sound FX.
 * @param[in] Volume The sound volume.
 */
void Audio::PlaySoundButton(s32 Volume)
{
    ASND_SetVoice(BUTTON_VOICE, VOICE_MONO_16BIT, 44100, 0,
        (void *)button_rollover, button_rollover_size, Volume, Volume, NULL);
}
