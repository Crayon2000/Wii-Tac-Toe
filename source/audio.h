/**
 * @file audio.h
 * Contains the Audio class.
 */

#ifndef AudioH
#define AudioH
//---------------------------------------------------------------------------
// Forward declarations
struct _modplay;
class Sound;
class Voice;

/**
 * This is a class used for the game audio.
 * @author Crayon
 */
class Audio
{
public:
    Audio();
    ~Audio();

    void PauseMusic(bool Paused);
    void LoadMusic(s32 Volume = 48);
    void PlaySoundScreenChange(u16 Volume);
    void PlaySoundButton(u16 Volume);
private:
    _modplay *ModTrack;
    Voice *ScreenVoice;
    Voice *ButtonVoice;
    Sound *ChangeSound;
    Sound *RollOverSound;
};
//---------------------------------------------------------------------------
#endif
