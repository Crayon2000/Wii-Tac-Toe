/**
 * @file player.h
 * Contains the Player class.
 */

#ifndef PlayerH
#define PlayerH
//---------------------------------------------------------------------------

#include <gctypes.h>
#include <string>

#define PLAYER_HUMAN    0 /**< Identify a human player. */
#define PLAYER_CPU      1 /**< Identify a computer player. */

/**
 * This class is used managed players.
 * @author Crayon
 */
class Player
{
public:
    Player();
    virtual ~Player() {};
    void SetName(const std::string &AName);
    std::string GetName();
    void SetSign(u8);
    u8 GetSign();
    unsigned int GetScore();
    void IncScore();
    void ResetScore();
    void SetType(u8);
    u8 GetType();
private:
    unsigned int Score;
    std::string Name;
    u8 Sign;
    u8 Type;
};
//---------------------------------------------------------------------------
#endif
