#include "player.h"

/**
 * Constructor for the Player class.
 */
Player::Player() :
    Score(0),
    Type(PLAYER_HUMAN)
{
}

/**
 * Set the player name.
 * @param[in] AName Give a name to the player.
 */
void Player::SetName(const std::wstring AName)
{
    Name = AName;
}

/**
 * Return the player name.
 * @return The name of the player.
 */
std::wstring Player::GetName()
{
    return Name;
}

/**
 * Set the player sign.
 * @param[in] ASign Give a sign to the player.
 */
void Player::SetSign(u8 ASign)
{
    Sign = ASign;
}

/**
 * Get the player sign.
 * @return Player sign.
 */
u8 Player::GetSign()
{
    return Sign;
}

/**
 * Get the player score.
 * @return Player score.
 */
unsigned int Player::GetScore()
{
    return Score;
}

/**
 * Increment score by one.
 */
void Player::IncScore()
{
    ++Score;
}

/**
 * Set scrore to 0.
 */
void Player::ResetScore()
{
    Score = 0;
}

/**
 * Set the player type.
 * @param[in] AType Give the type of player.
 */
void Player::SetType(u8 AType)
{
    Type = AType;
}

/**
 * Get the player type.
 * @return Player type.
 */
u8 Player::GetType()
{
    return Type;
}
