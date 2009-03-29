#include <stdlib.h>
#include <string.h> // For memcpy
#include "grid.h"

/**
 * Constructor for the Grid class.
 */
Grid::Grid()
{
    Clear();
}

/**
 * Destructor for the Grid class.
 */
Grid::~Grid()
{
}

/**
 * Set player X at a certain position.
 * @param[in] X X coordinate in the grid.
 * @param[in] Y Y coordinate in the grid.
 * @return Return false if a player already occupied this position.
 */
bool Grid::SetPlayerX(u8 X, u8 Y)
{
    if(Board[X][Y] == ' ')
    {
        Board[X][Y] = 'X';
        return true;
    }
    return false;
}

/**
 * Set player O at a certain position.
 * @param[in] X X coordinate in the grid.
 * @param[in] Y Y coordinate in the grid.
 * @return Return false if a player already occupied this position.
 */
bool Grid::SetPlayerO(u8 X, u8 Y)
{
    if(Board[X][Y] == ' ')
    {
        Board[X][Y] = 'O';
        return true;
    }
    return false;
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
    if(Player == 'X')
        return SetPlayerX(X, Y);
    else if(Player == 'O')
        return SetPlayerO(X, Y);
    else
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
    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            memcpy(TestBoard, Board, sizeof(TestBoard));
            if(TestBoard[x][y] == ' ')
            {
                TestBoard[x][y] = Player;
                if(IsPlayerWinning(Player, TestBoard))
                {
                    SetPlayer(Player, x, y);
                    return;
                }
            }
        }
    }

    // Test block
    u8 Opponent = (Player == 'X') ? 'O' : 'X';
    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            memcpy(TestBoard, Board, sizeof(TestBoard));
            if(TestBoard[x][y] == ' ')
            {
                TestBoard[x][y] = Opponent;
                if(IsPlayerWinning(Opponent, TestBoard))
                {
                    SetPlayer(Player, x, y);
                    return;
                }
            }
        }
    }

    // Play at random position
    SetPlayerRandom(Player);
}

/**
 * Set player at a random position.
 * @param[in] Player Player sign, either X or O.
 */
void Grid::SetPlayerRandom(u8 Player)
{
    bool OK = true;
    while(OK)
    {
        OK = !SetPlayer(Player, rand() % 3, rand() % 3);
    }
}

/**
 * Return the player at a certain position.
 * @param[in] X X coordinate in the grid.
 * @param[in] Y Y coordinate in the grid.
 * @return Player sign.
 */
u8 Grid::GetPlayerAtPos(u8 X, u8 Y)
{
    return Board[X][Y];
}

/**
 * Clear all the grid with default value.
 */
void Grid::Clear()
{
    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            Board[x][y] = ' ';
        }
    }
}

/**
 * Return the complete grid.
 */
/*
u8[][] Grid::GetGrid()
{
    return Board;
}
*/
/**
 * Return the winner.
 * @return Winning player.
 */
u8 Grid::GetWinner()
{
    if(IsPlayerWinning('X'))
        return 'X';
    else if(IsPlayerWinning('O'))
        return 'O';
    else
        return ' ';
}

/**
 * Check if a specific player is winning on a specific board.
 * @param[in] Player Player sign, either X or O.
 * @param[in] MyBord A specific board to test.
 * @return Return true if the player is winning, false otherwise.
 */
bool Grid::IsPlayerWinning(u8 Player, u8 MyBord[3][3])
{
    // Check rows
    if((MyBord[0][0] == Player) & (MyBord[1][0] == Player) & (MyBord[2][0] == Player))
    {
        return true;
    }
    if((MyBord[0][1] == Player) & (MyBord[1][1] == Player) & (MyBord[2][1] == Player))
    {
        return true;
    }
    if((MyBord[0][2] == Player) & (MyBord[1][2] == Player) & (MyBord[2][2] == Player))
    {
        return true;
    }
    // Check colums
    if((MyBord[0][0] == Player) & (MyBord[0][1] == Player) & (MyBord[0][2] == Player))
    {
        return true;
    }
    if((MyBord[1][0] == Player) & (MyBord[1][1] == Player) & (MyBord[1][2] == Player))
    {
        return true;
    }
    if((MyBord[2][0] == Player) & (MyBord[2][1] == Player) & (MyBord[2][2] == Player))
    {
        return true;
    }
    // Check diagonals
    if((MyBord[0][0] == Player) & (MyBord[1][1] == Player) & (MyBord[2][2] == Player))
    {
        return true;
    }
    if((MyBord[2][0] == Player) & (MyBord[1][1] == Player) & (MyBord[0][2] == Player))
    {
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
 */
bool Grid::IsFilled()
{
    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            if(Board[x][y] == ' ')
                return false;
        }
    }
    return true;
}

