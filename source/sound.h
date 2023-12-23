/**
 * @file sound.h
 * Contains the Sound class.
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <gctypes.h>
#include <span>

/**
 * This is a class used for sound.
 * @author Crayon
 */
class Sound
{
private:
    u32 _format;
    const std::span<const u8> _buffer;
    f32 _freq;
public:
    Sound(u32 format, std::span<const u8> buffer, f32 frequency);
    Sound(Sound const&) = delete;
    virtual ~Sound();
    Sound& operator=(Sound const&) = delete;

    [[nodiscard]] const std::span<const u8> GetBuffer() const;
    [[nodiscard]] u32 GetFormat() const;
    [[nodiscard]] f32 GetFrequency() const;
};

#endif /* SOUND_H_ */
