/**
 * @file audio.h
 * Contains the Audio class.
 */

#ifndef AudioH
#define AudioH
//---------------------------------------------------------------------------
// Forward declarations
class Voice;

/**
 * This is a class used for the game audio.
 * @author Crayon
 */
class Audio
{
public:
    Audio();
    Audio(Audio const&) = delete;
    ~Audio();
    Audio& operator=(Audio const&) = delete;

    void PauseMusic(bool Paused);
    void LoadMusic(s16 Volume = 255);
    void PlaySoundScreenChange(u16 Volume);
    void PlaySoundButton(u16 Volume);
private:
    bool Paused;
    Voice *ScreenVoice;
    Voice *ButtonVoice;
};
//---------------------------------------------------------------------------
#endif
