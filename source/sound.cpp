#include "sound.h"

/**
 * Constructor for the Sound class.
 * @param[in] format The sound format.
 * @param[in] buffer The sound buffer
 * @param[in] frequency The sound frequency.
 */
Sound::Sound(u32 format, std::span<const u8> buffer, f32 frequency) :
    _format(format), _buffer(buffer), _freq(frequency)
{
}

/**
 * Get the sound buffer.
 * @return Return the sound buffer.
 */
const std::span<const u8> Sound::GetBuffer() const
{
    return _buffer;
}

/**
 * Get the sound format.
 * @return Return the sound format.
 */
u32 Sound::GetFormat() const
{
    return _format;
}

/**
 * Get the sound frequency.
 * @return Return the sound frequency.
 */
f32 Sound::GetFrequency() const
{
    return _freq;
}

/**
 * Destructor for the Sound class.
 */
Sound::~Sound()
{
}
