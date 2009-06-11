//---------------------------------------------------------------------------
#ifndef GridH
#define GridH
//---------------------------------------------------------------------------

#include <gccore.h>

/**
 * Tic-Tac-Toe grid.
 * @author Crayon
 */
class Grid
{
public:
    Grid();
    ~Grid();
    bool SetPlayerX(u8, u8);
    bool SetPlayerO(u8, u8);
    bool SetPlayer(u8, u8, u8);
    void SetPlayerRandom(u8);
    void SetPlayerAI(u8);
    u8 GetPlayerAtPos(u8, u8);
    u8 GetWinner();
    void Clear();
    bool IsFilled();
private:
    u8 Board[3][3];

    bool IsPlayerWinning(u8);
    bool IsPlayerWinning(u8, u8 MyBord[3][3]);
};
//---------------------------------------------------------------------------
#endif
