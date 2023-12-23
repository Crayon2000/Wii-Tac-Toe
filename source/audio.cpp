#include <aesndlib.h>
#include <grrmod.h>
#include "voice.h"
#include "sound.h"
#include "audio.h"

// Audio files
#include "button_rollover_raw.h"
#include "screen_change_raw.h"
#include "tic_tac_mod.h"

static constexpr auto ScreenChangeBuffer = std::span{screen_change_raw, screen_change_raw_size};
static constexpr auto ButtonRolloverBuffer = std::span{button_rollover_raw, button_rollover_raw_size};

static const Sound ChangeSound = Sound(VOICE_MONO16, ScreenChangeBuffer, 44100.0f);
static const Sound RollOverSound = Sound(VOICE_MONO16, ButtonRolloverBuffer, 44100.0f);

/**
 * Constructor for the Audio class.
 */
Audio::Audio() :
    Paused(false)
{
    AESND_Init();
    AESND_Pause(false);

    GRRMOD_Init(true);
    GRRMOD_SetMOD(tic_tac_mod, tic_tac_mod_size);

    ScreenVoice = new Voice();
    ButtonVoice = new Voice();
}

/**
 * Destructor for the Audio class.
 */
Audio::~Audio()
{
    GRRMOD_Unload();
    GRRMOD_End();
    AESND_Pause(true);

    delete ScreenVoice;
    delete ButtonVoice;
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
void Audio::LoadMusic(s16 Volume)
{
    GRRMOD_Stop();
    GRRMOD_Start();
    GRRMOD_SetVolume(Volume, Volume); // Maximum volume is 255
    Paused = false;
}

/**
 * Play the screen change sound FX.
 * @param[in] Volume The sound volume.
 */
void Audio::PlaySoundScreenChange(u16 Volume)
{
    ScreenVoice->SetVolume(Volume);
    ScreenVoice->Play(&ChangeSound);
}

/**
 * Play the button sound FX.
 * @param[in] Volume The sound volume.
 */
void Audio::PlaySoundButton(u16 Volume)
{
    ButtonVoice->SetVolume(Volume);
    ButtonVoice->Play(&RollOverSound);
}
