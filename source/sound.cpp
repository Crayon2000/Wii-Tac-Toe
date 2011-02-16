#include "sound.h"

/**
 * Constructor for the Sound class.
 * @param[in] format The sound format.
 * @param[in] buffer The sound buffer
 * @param[in] len The sound buffer length.
 * @param[in] frequency The sound frequency.
 */
Sound::Sound(u32 format, const void *buffer, u32 len, u32 frequency) :
    _format(format), _buffer(buffer), _len(len), _freq(frequency)
{
}

/**
 * Get the sound buffer.
 * @return Return the sound buffer.
 */
const void *Sound::GetBuffer() const
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
u32 Sound::GetFrequency() const
{
    return _freq;
}

/**
 * Get the sound buffer length.
 * @return Return the sound buffer length.
 */
u32 Sound::GetLen() const
{
    return _len;
}

/**
 * Destructor for the Sound class.
 */
Sound::~Sound()
{
}
