# Wii-Tac-Toe

[![Build Status](https://travis-ci.org/Crayon2000/Wii-Tac-Toe.svg?branch=master)](https://travis-ci.org/Crayon2000/Wii-Tac-Toe)

Wii-Tac-Toe is a Tic-Tac-Toe game for the Nintendo Wii. It was programmed in C++ using devkitPro along with GRRLIB.

The game can be played alone vs the CPU or with a friend using one Wii Remote controller. The game AI does not always do the same moves, so the fun never stop.

To win the game, you have to place three marks (O or X) in a horizontal, vertical or diagonal row. If the grid is filled completely and no one has won, well, it's a tie game. Start over and try to beat your opponent.

![http://wiibrew.org/w/images/e/e7/Wii-Tac-Toe-screenshot.png](http://wiibrew.org/w/images/e/e7/Wii-Tac-Toe-screenshot.png)

## Build
Prerequisites:
* devkitPro
  * devkitPPC (release 28 or later)
  * libogc
  * libfat

To compile:
```
> make
```