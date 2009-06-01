#include "player.h"

/**
 * Constructor for the Player class.
 */
Player::Player()
{
	Score = 0;
	Type = PLAYER_HUMAN;
}

/**
 * Set the player name.
 * @param[in] Name Give a name to the player.
 */
void Player::SetName(const char *AName)
{
    Name = AName;
}

/**
 * Return the player name.
 * @return The name of the player.
 */
const char *Player::GetName()
{
    return Name.c_str();
}

/**
 * Set the player sign.
 * @param[in] Sign Give a sign to the player.
 */
void Player::SetSign(unsigned char ASign)
{
    Sign = ASign;
}

/**
 * Get the player sign.
 * @return Player sign.
 */
unsigned char Player::GetSign()
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
    Score++;
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
 * @param[in] Type Give the type of player.
 */
void Player::SetType(unsigned char AType)
{
    Type = AType;
}

/**
 * Get the player type.
 * @return Player type.
 */
unsigned char Player::GetType()
{
    return Type;
}
