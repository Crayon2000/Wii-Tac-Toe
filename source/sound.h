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
    u32 _freq;
public:
    Sound(u32 format, const void *buffer, u32 len, u32 frequency);
    Sound(Sound const&) = delete;
    virtual ~Sound();
    Sound& operator=(Sound const&) = delete;

    const void *GetBuffer() const;
    u32 GetFormat() const;
    u32 GetFrequency() const;
    u32 GetLen() const;
};

#endif /* SOUND_H_ */
