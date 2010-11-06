/**
 * @file audio.h
 * Contains the Audio class.
 */

#ifndef AudioH
#define AudioH
//---------------------------------------------------------------------------
// Forward declarations
struct aesndpb_t;

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
    void LoadMusic(s32 Volume = 255);
    void PlaySoundScreenChange(u16 Volume);
    void PlaySoundButton(u16 Volume);
private:
    aesndpb_t *ScreenVoice;
    aesndpb_t *ButtonVoice;
    bool Paused;
};
//---------------------------------------------------------------------------
#endif
