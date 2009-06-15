/**
 * @file player.h
 * Contains the Player class.
 */

#ifndef PlayerH
#define PlayerH
//---------------------------------------------------------------------------

#include <string>
using namespace std;

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
    void SetName(const char *);
    const char *GetName();
    void SetSign(unsigned char);
    unsigned char GetSign();
    unsigned int GetScore();
    void IncScore();
    void ResetScore();
    void SetType(unsigned char);
    unsigned char GetType();
private:
    unsigned int Score;
    string Name;
    unsigned char Sign;
    unsigned char Type;
};
//---------------------------------------------------------------------------
#endif
