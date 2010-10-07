/**
 * @file audio.h
 * Contains the Audio class.
 */

#ifndef AudioH
#define AudioH
//---------------------------------------------------------------------------
// Forward declarations
struct _modplay;
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
    void LoadMusic(s32 Volume = 48);
    void PlaySoundScreenChange(u16 Volume);
    void PlaySoundButton(u16 Volume);
private:
    _modplay *ModTrack;
    aesndpb_t *ScreenVoice;
    aesndpb_t *ButtonVoice;
};
//---------------------------------------------------------------------------
#endif
