/**
 * @file player.h
 * Contains the Player class.
 */

#ifndef PlayerH
#define PlayerH
//---------------------------------------------------------------------------

#include <gctypes.h>
#include <string>

#define PLAYER_HUMAN    0   /**< Indentify a human player. */
#define PLAYER_CPU      1   /**< Indentify a computer player. */

/**
 * This class is used managed players.
 * @author Crayon
 */
class Player
{
public:
    Player();
    virtual ~Player() {};
    void SetName(const std::wstring &AName);
    std::wstring GetName();
    void SetSign(u8);
    u8 GetSign();
    unsigned int GetScore();
    void IncScore();
    void ResetScore();
    void SetType(u8);
    u8 GetType();
private:
    unsigned int Score;
    std::wstring Name;
    u8 Sign;
    u8 Type;
};
//---------------------------------------------------------------------------
#endif
