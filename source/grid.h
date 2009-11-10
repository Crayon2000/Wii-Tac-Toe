/**
 * @file grid.h
 * Contains the Grid class.
 */

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
    bool SetPlayer(u8, u8, u8);
    void SetPlayerAI(u8);
    u8 GetPlayerAtPos(u8, u8);
    u8 GetWinner();
    void Clear();
    bool IsFilled();
    bool WinningBoard[3][3];    /**< A board filled with the winning position. */
private:
    u8 Board[3][3];
    u8 Winner;

    bool IsPlayerWinning(u8);
    bool IsPlayerWinning(u8, u8 MyBord[3][3]);
};
//---------------------------------------------------------------------------
#endif
