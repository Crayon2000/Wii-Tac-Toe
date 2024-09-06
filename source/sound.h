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
    const u32 _format;
    const std::span<const u8> _buffer;
    const f32 _freq;
public:
    /**
     * Constructor for the Sound class.
     * @param[in] format The sound format.
     * @param[in] buffer The sound buffer
     * @param[in] frequency The sound frequency.
     */
    constexpr Sound(u32 format, std::span<const u8> buffer, f32 frequency) :
        _format(format), _buffer(buffer), _freq(frequency)
    {
    }

    Sound(Sound const&) = delete;
    Sound& operator=(Sound const&) = delete;

    /**
     * Get the sound buffer.
     * @return Return the sound buffer.
     */
    [[nodiscard]] constexpr std::span<const u8> GetBuffer() const
    {
        return _buffer;
    }

    /**
     * Get the sound format.
     * @return Return the sound format.
     */
    [[nodiscard]] constexpr u32 GetFormat() const
    {
        return _format;
    }

    /**
     * Get the sound frequency.
     * @return Return the sound frequency.
     */
    [[nodiscard]] constexpr f32 GetFrequency() const
    {
        return _freq;
    }
};

#endif /* SOUND_H_ */
