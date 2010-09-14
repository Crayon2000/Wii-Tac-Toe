/**
 * @file audio.h
 * Contains the Audio class.
 */

#ifndef AudioH
#define AudioH
//---------------------------------------------------------------------------
// Forward declarations
struct _modplay;

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
    void PlaySoundScreenChange(s32 Volume);
    void PlaySoundButton(s32 Volume);
private:
    _modplay *ModTrack;
};
//---------------------------------------------------------------------------
#endif
