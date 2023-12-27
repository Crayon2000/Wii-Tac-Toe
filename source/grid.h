/**
 * @file grid.h
 * Contains the Grid class.
 */

#ifndef GridH
#define GridH
//---------------------------------------------------------------------------

#include <gctypes.h>
#include <random>

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
    [[nodiscard]] u8 GetPlayerAtPos(u8 X, u8 Y) const;
    [[nodiscard]] u8 GetWinner() const;
    void Clear();
    [[nodiscard]] bool IsFilled();
    bool WinningBoard[3][3]; /**< A board filled with the winning position. */
private:
    u8 Board[3][3];
    u8 Winner;
    std::mt19937 Generator;
    std::uniform_int_distribution<u8> Distribution;

    [[nodiscard]] bool IsPlayerWinning(u8 Player);
    [[nodiscard]] bool IsPlayerWinning(u8 Player, const u8 MyBoard[3][3]);
};
//---------------------------------------------------------------------------
#endif
