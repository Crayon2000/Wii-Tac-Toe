#include <cstdlib>
#include <cstring> // For memcpy
#include "grid.h"

/**
 * Constructor for the Grid class.
 */
Grid::Grid()
{
    std::random_device RandomDevice;
    Generator = std::mt19937(RandomDevice());
    Distribution = std::uniform_int_distribution<u8>(0, 2);

    std::memset(WinningBoard, false, sizeof(WinningBoard));
    Clear();
}

/**
 * Destructor for the Grid class.
 */
Grid::~Grid()
{
}

/**
 * Set player at a certain position.
 * @param[in] Player Player sign, either X or O.
 * @param[in] X X coordinate in the grid.
 * @param[in] Y Y coordinate in the grid.
 * @return Return false if a player already occupied this position.
 */
bool Grid::SetPlayer(u8 Player, u8 X, u8 Y)
{
    if(Board[X][Y] == ' ' &&
        (Player == 'X' || Player == 'O'))
    {
        Board[X][Y] = Player;
        std::memset(WinningBoard, false, sizeof(WinningBoard));
        if(IsPlayerWinning(Player) == true)
        {
            Winner = Player;
        }
        return true;
    }
    return false;
}

/**
 * Set player at a good position.
 * @param[in] Player Player sign, either X or O.
 */
void Grid::SetPlayerAI(u8 Player)
{
    u8 TestBoard[3][3];

    // Test win
    for(u8 x = 0; x < 3; ++x)
    {
        for(u8 y = 0; y < 3; ++y)
        {
            std::memcpy(TestBoard, Board, sizeof(TestBoard));
            if(TestBoard[x][y] == ' ')
            {
                TestBoard[x][y] = Player;
                if(IsPlayerWinning(Player, TestBoard) == true)
                {
                    SetPlayer(Player, x, y);
                    return;
                }
            }
        }
    }

    // Test block
    const u8 Opponent = (Player == 'X') ? 'O' : 'X';
    for(u8 x = 0; x < 3; ++x)
    {
        for(u8 y = 0; y < 3; ++y)
        {
            std::memcpy(TestBoard, Board, sizeof(TestBoard));
            if(TestBoard[x][y] == ' ')
            {
                TestBoard[x][y] = Opponent;
                if(IsPlayerWinning(Opponent, TestBoard) == true)
                {
                    SetPlayer(Player, x, y);
                    return;
                }
            }
        }
    }

    // Play at random position
    while(SetPlayer(Player, Distribution(Generator), Distribution(Generator)) == false) {}
}

/**
 * Return the player at a certain position.
 * @param[in] X X coordinate in the grid.
 * @param[in] Y Y coordinate in the grid.
 * @return Player sign.
 */
u8 Grid::GetPlayerAtPos(u8 X, u8 Y) const
{
    return Board[X][Y];
}

/**
 * Clear all the grid with default value.
 */
void Grid::Clear()
{
    Winner = ' ';
    std::memset(Board, ' ', sizeof(Board));
}

/**
 * Return the winner.
 * @return Winning player.
 */
u8 Grid::GetWinner() const
{
    return Winner;
}

/**
 * Check if a specific player is winning on a specific board.
 * @param[in] Player Player sign, either X or O.
 * @param[in] MyBoard A specific board to test.
 * @return Return true if the player is winning, false otherwise.
 */
bool Grid::IsPlayerWinning(u8 Player, u8 MyBoard[3][3])
{
    // Check rows
    if((MyBoard[0][0] == Player) & (MyBoard[1][0] == Player) & (MyBoard[2][0] == Player))
    {
        WinningBoard[0][0] = true; WinningBoard[1][0] = true; WinningBoard[2][0] = true;
        return true;
    }
    if((MyBoard[0][1] == Player) & (MyBoard[1][1] == Player) & (MyBoard[2][1] == Player))
    {
        WinningBoard[0][1] = true; WinningBoard[1][1] = true; WinningBoard[2][1] = true;
        return true;
    }
    if((MyBoard[0][2] == Player) & (MyBoard[1][2] == Player) & (MyBoard[2][2] == Player))
    {
        WinningBoard[0][2] = true; WinningBoard[1][2] = true; WinningBoard[2][2] = true;
        return true;
    }
    // Check columns
    if((MyBoard[0][0] == Player) & (MyBoard[0][1] == Player) & (MyBoard[0][2] == Player))
    {
        WinningBoard[0][0] = true; WinningBoard[0][1] = true; WinningBoard[0][2] = true;
        return true;
    }
    if((MyBoard[1][0] == Player) & (MyBoard[1][1] == Player) & (MyBoard[1][2] == Player))
    {
        WinningBoard[1][0] = true; WinningBoard[1][1] = true; WinningBoard[1][2] = true;
        return true;
    }
    if((MyBoard[2][0] == Player) & (MyBoard[2][1] == Player) & (MyBoard[2][2] == Player))
    {
        WinningBoard[2][0] = true; WinningBoard[2][1] = true; WinningBoard[2][2] = true;
        return true;
    }
    // Check diagonals
    if((MyBoard[0][0] == Player) & (MyBoard[1][1] == Player) & (MyBoard[2][2] == Player))
    {
        WinningBoard[0][0] = true; WinningBoard[1][1] = true; WinningBoard[2][2] = true;
        return true;
    }
    if((MyBoard[2][0] == Player) & (MyBoard[1][1] == Player) & (MyBoard[0][2] == Player))
    {
        WinningBoard[2][0] = true; WinningBoard[1][1] = true; WinningBoard[0][2] = true;
        return true;
    }
    return false;
}

/**
 * Check if a specific player is winning.
 * @param[in] Player Player sign, either X or O.
 * @return Return true if the player is winning, false otherwise.
 */
bool Grid::IsPlayerWinning(u8 Player)
{
    return IsPlayerWinning(Player, Board);
}

/**
 * Check if the grid is completely filled.
 * @return Return true if the grid is completely filled, false otherwise.
 */
bool Grid::IsFilled()
{
    for(u8 x = 0; x < 3; ++x)
    {
        for(u8 y = 0; y < 3; ++y)
        {
            if(Board[x][y] == ' ')
            {
                return false;
            }
        }
    }
    return true;
}
