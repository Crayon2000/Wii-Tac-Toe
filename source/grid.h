/**
 * @file grid.h
 * Contains the Grid class.
 */

#ifndef GridH
#define GridH
//---------------------------------------------------------------------------

#include <gctypes.h>

/**
 * Tic-Tac-Toe grid.
 * @author Crayon
 */
class Grid
{
public:
    Grid();
    Grid(Grid const&) = delete;
    ~Grid();
    Grid& operator=(Grid const&) = delete;
    bool SetPlayer(u8 Player, u8 X, u8 Y);
    void SetPlayerAI(u8 Player);
    u8 GetPlayerAtPos(u8 X, u8 Y) const;
    u8 GetWinner() const;
    void Clear();
    bool IsFilled();
    bool WinningBoard[3][3]; /**< A board filled with the winning position. */
private:
    u8 Board[3][3];
    u8 Winner;

    bool IsPlayerWinning(u8 Player);
    bool IsPlayerWinning(u8 Player, u8 MyBoard[3][3]);
};
//---------------------------------------------------------------------------
#endif
