/**
 * @file player.h
 * Contains the Player class.
 */

#ifndef PlayerH
#define PlayerH
//---------------------------------------------------------------------------

#include <gctypes.h>
#include <string>

/**
 * Types of player.
 */
enum class playerType : u8 {
    Human, /**< Identify a human player. */
    CPU    /**< Identify a computer player. */
};

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
    void SetType(playerType);
    playerType GetType();
private:
    unsigned int Score;
    std::string Name;
    u8 Sign;
    playerType Type;
};
//---------------------------------------------------------------------------
#endif
