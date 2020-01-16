#include "voice.h"
#include "sound.h"
#include <aesndlib.h>

/**
 * Constructor for the Voice class.
 */
Voice::Voice()
{
    _Voice = AESND_AllocateVoice(nullptr);
}

/**
 * Destructor for the Voice class.
 */
Voice::~Voice()
{
    AESND_FreeVoice(_Voice);
}

/**
 * Set the volume. Volume must be between 0 and 255.
 * @param[in] Volume The volume of the voice.
 */
void Voice::SetVolume(u16 Volume)
{
    SetVolume(Volume, Volume);
}

/**
 * Set the volume. Volume must be between 0 and 255.
 * @param[in] LeftVolume The left volume of the voice.
 * @param[in] RightVolume The right volume of the voice.
 */
void Voice::SetVolume(u16 LeftVolume, u16 RightVolume)
{
    AESND_SetVoiceVolume(_Voice, LeftVolume, RightVolume);
}

/**
 * Play a sound.
 * @param[in] sound The sound to play.
 * @param[in] delay A delay.
 * @param[in] looped Set true to make the sound loop, false otherwise.
 */
void Voice::Play(Sound *sound, u32 delay, bool looped)
{
    AESND_PlayVoice(_Voice, sound->GetFormat(), sound->GetBuffer(), sound->GetLen(),
            sound->GetFrequency(), delay, looped);
}

/**
 * Mute the voice.
 * @param[in] mute Set true to mute the voice, false otherwise.
 */
void Voice::Mute(bool mute)
{
    AESND_SetVoiceMute(_Voice, mute);
}
