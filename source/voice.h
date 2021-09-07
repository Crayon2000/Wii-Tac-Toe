/**
 * @file voice.h
 * Contains the Voice class.
 */

#ifndef VOICE_H_
#define VOICE_H_

#include <gctypes.h>

// Forward declarations
class Sound;
struct aesndpb_t;

/**
 * This is a class used for voice.
 * @author Crayon
 */
class Voice
{
private:
    aesndpb_t *_Voice;
public:
    Voice();
    Voice(Voice const&) = delete;
    virtual ~Voice();
    Voice& operator=(Voice const&) = delete;

    void SetVolume(u16 Volume);
    void SetVolume(u16 LeftVolume, u16 RightVolume);
    void Play(Sound *sound, u32 delay = 0, bool looped = false);
    void Mute(bool mute);
};

#endif /* VOICE_H_ */
