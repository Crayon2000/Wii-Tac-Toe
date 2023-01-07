/**
 * @file sound.h
 * Contains the Sound class.
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <gctypes.h>

/**
 * This is a class used for sound.
 * @author Crayon
 */
class Sound
{
private:
    u32 _format;
    const void *_buffer;
    u32 _len;
    f32 _freq;
public:
    Sound(u32 format, const void *buffer, u32 len, f32 frequency);
    Sound(Sound const&) = delete;
    virtual ~Sound();
    Sound& operator=(Sound const&) = delete;

    [[nodiscard]] const void *GetBuffer() const;
    [[nodiscard]] u32 GetFormat() const;
    [[nodiscard]] f32 GetFrequency() const;
    [[nodiscard]] u32 GetLen() const;
};

#endif /* SOUND_H_ */
